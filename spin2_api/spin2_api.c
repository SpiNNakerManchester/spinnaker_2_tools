#include <spin2_api.h>
#include <spin2_api_params.h>

static volatile uint32_t run;           //!< controls simulation start/exit
static volatile uint32_t paused;        //!< indicates when paused
static volatile uint32_t resume_sync;   //!< controls re-synchronisation

//! The highest priority selected
//POTENTIAL DELETE
static uint highest_priority = 0;

//! Interrupt select to be used based on user selections
static uint user_int_select = 0;

static task_queue_t task_queue[N_TASK_QUEUES];
cback_t callback[NUM_EVENTS];

dma_queue_t dma_queue;

void spin2_dma_transfer(uint32_t tag, void *remote_address, void *local_address, uint32_t direction, uint32_t length) {

    uint id = 0;
    spin2_int_disable();

    uint new_end = (dma_queue.end + 1) % DMA_QUEUE_SIZE;

    if (new_end != dma_queue.start) {
        id = diagnostics.dma_transfers++;

        uint desc = DMA_WIDTH << 24 | DMA_BURST_SIZE << 21
                | direction << 19 | length;

        dma_queue.queue[dma_queue.end].id = id;
        dma_queue.queue[dma_queue.end].tag = tag;
        dma_queue.queue[dma_queue.end].system_address = remote_address;
        dma_queue.queue[dma_queue.end].tcm_address = local_address;
        dma_queue.queue[dma_queue.end].description = desc;

        /* if dmac is available and dma_queue empty trigger transfer now */
        if (!(dma[DMA_STAT] & 4) && (dma_queue.start == dma_queue.end)) {
            dma[DMA_ADRS] = (uint) remote_address;
            dma[DMA_ADRT] = (uint) local_address;
            dma[DMA_DESC] = desc;
        }

        dma_queue.end = new_end;
    }

    spin2_int_enable();
    return id;
}

void inline spin2_memcpy(void *dst, const void *src, uint32_t len) {

    sark2_memcpy();
}


uint32_t inline spin2_get_pe_id(void) {

    return sark2_pe_id();
}

uint32_t inline spin2_get_qpe_id(void) {

    return sark2_qpe_id();
}

uint32_t inline spin2_get_node_id(void) {

    return sark2_node_id();
}

uint32_t inline spin2_get_chip_id(void) {

    return sark2_chip_id();
}

void inline spin2_wfi(void) {

    sark2_wfi();
}

void inline spin2_wfe(void) {

    sark2_wfe();
}

//void spin2_sleep(void) {
//
//    asm volatile ("sleep:"
//                  "wfi;"
//                  "b sleep;");
//}

// Enable FIQ and IRQ interrupts
void inline spin2_int_enable(void) {

    sark2_int_enable();
}

// Disable all interrupts
void inline spin2_int_disable(void) {

    sark2_int_disable();
}

// Disable IRQ interrupts
void inline spin2_irq_disable(void) {

    sark2_irq_disable();
}

// Restore interrupts after spin2_irq_disable
void inline spin2_mode_restore(void) {

    sark2_mode_restore();
}


void spin2_callback_on(uint32_t event_id, callback_t cback, int priority){

    callback[event_id].cback = cback;
    callback[event_id].priority = priority;

    // Update which events we are to handle
    user_int_select |= NVIC_EVENTS[event_id];
    if (run) {
        vic[VIC_ENABLE] = NVIC_EVENTS[event_id];
    }
    if (priority > 0) {
        uint p = (uint) priority;
        if (p > highest_priority) {
            highest_priority = priority;
        }
    }
}

void spin2_callback_off(uint32_t event_id) {

    callback[event_id].cback = NULL;

    user_int_select &= ~NVIC_EVENTS[event_id];
}


uint32_t spin2_schedule_callback(callback_t cback, uint32_t arg0, uint32_t arg1, int priority) {

     uchar result = SUCCESS;

    /* disable interrupts for atomic access to task queues */
    spin2_int_disable();

    task_queue_t *tq = &task_queue[priority-1];
    if (priority > highest_priority) {
        highest_priority = priority;
    }

    if ((tq->end + 1) % TASK_QUEUE_SIZE != tq->start) {
        tq->queue[tq->end].cback = cback;
        tq->queue[tq->end].arg0 = arg0;
        tq->queue[tq->end].arg1 = arg1;

        tq->end = (tq->end + 1) % TASK_QUEUE_SIZE;
    } else {
        // queue is full
        result = FAILURE;
    }

    /* restore interrupt status */
    spin2_int_enable();

    return result;
}


/*! \brief Deschedules all callbacks for an event.
*
*  This function deschedules all callbacks corresponding to the given event
*  ID.
*
*  \note this function cannot guarantee
*  that all callbacks pertaining to the given event ID will be descheduled:
*  once a callback has been prepared for execution by the dispatcher it is
*  immune to descheduling and will be executed upon return to the dispatcher.
*
*  \param[in] event_id: event ID of the callbacks to be descheduled
*/

static void deschedule(uint event_id) {

    spin2_int_disable();

    task_queue_t *tq = &task_queue[callback[event_id].priority-1];

    for (uint i = 0; i < TASK_QUEUE_SIZE; i++) {
        if (tq->queue[i].cback == callback[event_id].cback) {
            tq->queue[i].cback = NULL;
        }
    }

    spin2_int_enable();

}

// ------------------------------------------------------------------------
// scheduler/dispatcher functions
// ------------------------------------------------------------------------
/*! \brief Main simulation event dispatch loop.
*
*  This function executes callbacks which are scheduled in response to events.
*  Callbacks are completed firstly in order of priority and secondly in the
*  order in which they were enqueued.
*
*  The dispatcher is the sole consumer of the scheduler queues and so can
*  safely run with interrupts enabled. Note that deschedule()
*  modifies the scheduler queues which naturally influences the callbacks
*  that are dispatched by this function but not in such a way as to allow the
*  dispatcher to move the processor into an invalid state such as calling a
*  NULL function.
*
*  Upon emptying the scheduling queues the dispatcher goes into wait for
*  interrupt mode.
*
*  Potential hazard: It is possible that an event will occur _and result in
*  a callback being scheduled_ AFTER the last check on the scheduler queues
*  and BEFORE the wait for interrupt call. In this case, the scheduled
*  callback would not be handled until the next event occurs and causes the
*  wait for interrupt call to return.
*
*  This hazard is avoided by calling wait for interrupt with interrupts
*  disabled! Any interrupt will still wake up the core and then
*  interrupts are enabled, allowing the core to respond to it.
*  Comes from Spin1_API
*/
static void dispatch(void) {

    uint32_t i;
    task_queue_t *tq;
    volatile callback_t cback;
    resume_sync = 0;

    // dispatch callbacks from queues until
    // spin1_exit () is called (run = 0)
    while (run) {
        i = 0;

        // disable interrupts to avoid concurrent
        // scheduler/dispatcher accesses to queues
        spin2_int_disable();

        while (run && i < highest_priority) {
            tq = &task_queue[i];

            i++;  // prepare for next priority queue

            if (tq->start != tq->end) {
                cback = tq->queue[tq->start].cback;
                uint arg0 = tq->queue[tq->start].arg0;
                uint arg1 = tq->queue[tq->start].arg1;

                tq->start = (tq->start + 1) % TASK_QUEUE_SIZE;

                if (cback != NULL) {
                    // run callback with interrupts enabled
                    spin1_mode_restore(cpsr);

                    // check for if its a timer callback, if it is, update
                    // tracker values
                    // NOT IMPLEMENTED YET
                    //if (cback == callback[TIMER_TICK].cback) {
                    //    diagnostics.in_timer_callback = 1;
                    //}

                    // execute callback
                    if (cback != callback[TIMER_TICK].cback || !paused) {
                        cback(arg0, arg1);
                    }

                    // update queue size
                    // NOT IMPLEMENTED YET
                    //if (cback == callback[TIMER_TICK].cback) {
                    //    if (diagnostics.number_timer_tic_in_queue > 0) {
                    //        diagnostics.number_timer_tic_in_queue -= 1;
                    //    }
                    //    diagnostics.in_timer_callback = 0;
                    //}

                    spin2_int_disable();

                    // re-start examining queues at highest priority
                    i = 0;
                }
            }
        }

        if (run) {
            // go to sleep with interrupts disabled to avoid hazard!
            // an interrupt will still wake up the dispatcher
            spin2_wfi();

            // Handle resume
            //if (resume_sync == 1) {
                // @TODO: Implement the resume_wait and the SYNC
                //if (!resume_wait()) {
                //    resume();
                //}
            //}

            spin2_int_enable();
        }
    }
}

//! \brief Begins a simulation by enabling the timer (if called for) and
//!     beginning the dispatcher loop.
//!     Only returns once told to exit (with spin1_exit()).
//! \param[in] sync: Whether to synchronise with other cores
//! \param[in] start_paused: Whether to start in a paused state
//! \return the exit code
static uint32_t start(sync_bool sync, uint start_paused) {
    // @TODO: missing implementation
}

uint32_t spin2_start(sync_bool sync)
{
    return start(sync, 0);
}

static inline void spin2_delay_us(uint32_t n) {

    // @TODO: ASM implementation, potentially in sark2
}

static inline void* spin2_malloc(uint32_t bytes) {

    // @TODO: Define the position of the memory heap. Will depend on the memory map
    sark2_xalloc(mem_heap, bytes, 0, 0);
}

void spin2_rte() {

    // @TODO: implement the cpu states
}

static inline void spin2_srand(void) {

    srand();
}

static inline unit32_t spin2_int_rand(void) {

    return uni_rand();
}

static inline float spin2_float_rand(void) {

    return uni_randf();
}

static inline float spin2_gauss_rand(void) {

    return gauss_randf();
}
