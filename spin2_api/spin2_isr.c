#include <sark2.h>

#include <spin2_api.h>
#include <spin2_api_params.h>

// @TODO: This is a copy paste from spin1_isr so far.

/*! \brief DMA complete interrupt handler.
*
*  This interrupt service routine is called upon completion of a DMA transfer.
*  A user callback is scheduled (with two parameters, the ID of the completed
*  transfer and the user-provided transfer tag) and the next DMA transfer
*  request is dequeued and fulfilled. The completion and subsequent scheduling
*  of transfers must be atomic (as they are in this uninterruptable ISR)
*  otherwise transfer requests may not be completed in the order they were
*  made.
*/
INT_HANDLER dma_done_isr(void) {
    // Clear transfer done interrupt in DMAC
    dma[DMA_CTRL] = 0x8;

    // Prepare data for callback before triggering a new DMA transfer

    copy_t *entry = &dma_queue.queue[dma_queue.start];
    uint completed_id  = entry->id;
    uint completed_tag = entry->tag;

    // Update queue pointer and trigger new transfer if queue not empty

    dma_queue.start = (dma_queue.start + 1) % DMA_QUEUE_SIZE;

    if (dma_queue.start != dma_queue.end) {
        entry = &dma_queue.queue[dma_queue.start];
        uint *system_address = entry->system_address;
        uint *tcm_address = entry->tcm_address;
        uint description = entry->description;

        dma[DMA_ADRS] = (uint) system_address;
        dma[DMA_ADRT] = (uint) tcm_address;
        dma[DMA_DESC] = description;
    }

    // If application callback registered schedule it

    if (callback[DMA_TRANSFER_DONE].cback != NULL) {
        schedule(DMA_TRANSFER_DONE, completed_id, completed_tag);
    }

    // Ack VIC
    vic[VIC_VADDR] = 1;
}

/*! \brief DMA error interrupt handler.
*
*  This interrupt service function is called when a DMA transfer error arises.
*  Currently, such an event causes termination of the simulation.
*/
INT_HANDLER dma_error_isr(void) {

    // deal with write buffer errors
#if (API_WARN == TRUE) && (USE_WRITE_BUFFER == TRUE)
    // increase error count
    wberrors++;
#endif

    // clear write buffer error interrupt in DMAC and ack VIC
    dma[DMA_CTRL]  = 0x20;
    vic[VIC_VADDR] = 1;
}

/*! \brief Timer 1 interrupt handler.
*
*  This interrupt service routine is called upon countdown of the processor's
*  primary timer to zero. In response, a callback is scheduled.
*/
INT_HANDLER timer1_isr(void) {

    // Clear timer interrupt
    tc[T1_INT_CLR] = 1;

    // Increment simulation "time"
    ticks++;

    // Update drift if needed
    time_to_next_drift_update -= timer_tick;
    if (time_to_next_drift_update <= 0) {
        drift = timer_tick * sv->clock_drift;
        drift_sign = 1;
        if (drift < 0) {
            drift = -drift;
            drift_sign = -1;
        }
        time_to_next_drift_update += TIME_BETWEEN_SYNC_US;
    }

    // Each timer tick we add on the integer number of clock cycles accumulated
    // and subtract this from the accumulator.
    drift_accum += drift;
    int drift_int = drift_accum & DRIFT_INT_MASK;
    drift_accum -= drift_int;
    drift_int = (drift_int >> DRIFT_FP_BITS) * drift_sign;
    tc[T1_BG_LOAD] = timer_tick_clocks + drift_int;

    // If application callback registered schedule it
    if (callback[TIMER_TICK].cback != NULL) {

        // check for timer tic overload and store in diagnostics
        if (diagnostics.in_timer_callback != 0) {

            // if in timer tic callback already, add to tracker for total failures
            diagnostics.total_times_tick_tic_callback_overran += 1;

            // if number of timer callbacks in queue is greater than previously seen
            if (diagnostics.number_timer_tic_in_queue >
                    diagnostics.largest_number_of_concurrent_timer_tic_overruns) {
                diagnostics.largest_number_of_concurrent_timer_tic_overruns =
                        diagnostics.number_timer_tic_in_queue;
            }
        }
        schedule(TIMER_TICK, ticks, NULL);
    }

    // Ack VIC
    vic[VIC_VADDR] = 1;
}

/*! \brief Software-initiated interrupt handler.
*
*  This interrupt service routine is called upon receipt of software
*  controller interrupt, triggered by a "USER EVENT".
*/
INT_HANDLER soft_int_isr(void) {

    uint arg0  = user_event_queue.queue[user_event_queue.start].arg0;
    uint arg1  = user_event_queue.queue[user_event_queue.start].arg1;

    // Update queue pointer
    user_event_queue.start = (user_event_queue.start + 1) % USER_EVENT_QUEUE_SIZE;

    // If the queue is now empty, clear the interrupt
    if (user_event_queue.start == user_event_queue.end) {
        vic[VIC_SOFT_CLR] = (1 << SOFTWARE_INT);
    }

    // If application callback registered schedule it
    if (callback[USER_EVENT].cback != NULL) {
        schedule(USER_EVENT, arg0, arg1);
    }

    // Ack VIC
    vic[VIC_VADDR] = 1;
}

