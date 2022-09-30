#ifndef __SPIN2_API_H__
#define __SPIN2_API_H__

#include <sark2.h>

/*
typedef uint32_t uint;
*/
typedef unsigned char uchar;


volatile bool API_BURST_FINISHED;

// ------------------------------------------------------------------------
// general parameters and definitions
// ------------------------------------------------------------------------
/* boolean constants */
#define TRUE               (0 == 0)
#define FALSE              (0 != 0)
/* function results   */
#define SUCCESS            (uint32_t) 1
#define FAILURE            (uint32_t) 0

#ifndef NULL
/* Null pointer value */
#define NULL               0
#endif // NULL

// ------------------------------------------------------------------------
// event definitions
// ------------------------------------------------------------------------
// event-related parameters
// Potentially add more or remove according to what we need

#define NUM_EVENTS            6
#define MC_PACKET_RECEIVED    0
#define DMA_TRANSFER_DONE     1
#define TIMER_TICK            2
#define SDP_PACKET_RX         3 // Are we keeping this?
#define USER_EVENT            4
#define MCPL_PACKET_RECEIVED  5

//! Match events above to their NVIC interrupts.  Indices must match!
// Comes from spin1_API
static const uint NVIC_EVENTS[] = {
    (1 << CC_MC_INT),         //!< 0. Multicast packet received
    (1 << DMA_DONE_INT),      //!< 1. DMA transfer complete
    (1 << TIMER1_INT),        //!< 2. Regular timer tick
    0,                        //!< 3. SDP message received; handled elsewhere
    (1 << SOFTWARE_INT),      //!< 4. User-triggered interrupt
    (1 << CC_MC_INT)         //!< 5. Mulitcast packet with payload received
};

// ------------------------------------------------------------------------
// DMA transfer parameters
// ------------------------------------------------------------------------
// DMA transfer direction (from core point of view)
#define DMA_READ           0
#define DMA_WRITE          1


// ------------------------------------------------------------------------
// packet parameters
// ------------------------------------------------------------------------
// payload presence
#define NO_PAYLOAD         0
#define WITH_PAYLOAD       1


// ------------------------------------------------------------------------
// type definitions
// ------------------------------------------------------------------------

typedef void (*callback_t) (uint32_t, uint32_t);  // callbacks
// ------------------------------------------------------------------------

//! \brief Wait for interrupt.
//! \details Puts the CPU to sleep until an interrupt occurs.
void inline spin2_wfi(void);

//! \brief Wait for event.
//! \details Puts the CPU to sleep until an event occurs.
void inline spin2_wfe(void);

// ------------------------------------------------------------------------
// callback and task functions
// ------------------------------------------------------------------------
//! \brief Enables a callback for a class of event
//! \details
//!     This function sets the given callback to be scheduled on occurrence
//!     of the specified event. The priority argument dictates the order in
//!     which callbacks are executed by the scheduler.
//! \param[in] event_id: Which event to enable
//! \param[in] cback: The callback handler
//! \param[in] priority: The priority of interrupt handling.
//!      0 = non-queueable callback (associated to irq)
//!    > 0 = queueable callback
//!    < 0 = preeminent callback (associated to fiq)
void spin2_callback_on(uint32_t event_id, callback_t cback, int priority);

//! \brief Disables a callback for a class of event
//! \details This function disables the callback for the specified event.
//! \param[in] event_id: Which event to disable
void spin2_callback_off(uint32_t event_id);

//! \brief This function places a cback into the scheduling queue
//!     corresponding to its priority
//! \param[in] cback: callback to be scheduled
//! \param[in] arg0: argument to be passed to the callback
//! \param[in] arg1: argument to be passed to the callback
//! \param[in] priority: cback priority
//! \return #SUCCESS or #FAILURE
uint32_t spin2_schedule_callback(callback_t cback, uint32_t arg0, uint32_t arg1, int priority);

// ------------------------------------------------------------------------
//  data transfer functions
// ------------------------------------------------------------------------

//! \brief Initiates a DMA transfer.
//! \param[in] tag: A label for the transfer. Can be used by DMA complete
//! 	callback to work out what to do.
//! \param remote_address: Remote address. May be source or destination.
//! 	Must be word-aligned.
//! \param local_address:Local address. May be source or destination.
//! 	Must be word-aligned.
//! \param[in] direction: Which direction is data being moved in.
//! \param[in] length: How much data is being moved.
//! 	Must be a whole number of words.
//! \return a locally-unique DMA transfer ID.
//! 	Can be used by DMA complete callback to identify what finished.
void spin2_dma_transfer(uint32_t tag, void *remote_address, void *local_address, uint32_t direction, uint32_t length);

//! \brief Directly copies data from \p src to \p dst byte by byte.
//! \details Does not require alignment.
//! \param[out] dst: Where to copy to
//! \param[in] src: Where to copy from
//! \param[in] len: Number of bytes to copy
void spin2_memcpy(void *dst, void const *src, uint32_t len);
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//  interrupt control functions
// ------------------------------------------------------------------------

/*!
* \brief
*  Enables IRQ and FIQ interrupts to the processor.
*/
void inline spin2_int_enable(void);
/*!
* \brief
*  Disables IRQ and FIQ interrupts to the processor.
*/
void inline spin2_int_disable(void);
/*!
* \brief
*  Disables IRQ interrupts to the processor.
*/
void inline spin2_irq_disable(void);
/*!
* \brief
*  Enables IRQ interrupts to the processor, restoring
*  the state following a spin2_irq_disable() call.
*/
void inline spin2_mode_restore(void);

// ------------------------------------------------------------------------

uint32_t spin2_send_packet(uint32_t key, uint32_t data, uint32_t TCR);

// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// simulation control functions
// ------------------------------------------------------------------------

//! \brief Begins a simulation by enabling the timer (if called for) and
//!     beginning the dispatcher loop.
//!     Only returns once told to exit (with spin1_exit()).
//! \param[in] sync: Whether to synchronise with other cores
//! \return the exit code
uint32_t spin2_start(sync_bool sync);

// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//  system resources access functions
// ------------------------------------------------------------------------

//! \brief Returns the core ID.
//! \return PE ID.
uint32_t inline spin2_get_pe_id(void);
//! \brief Returns the QPE ID without the PE ID. The 3 LSBs are the y coordinate, the following 3 the x coordinate
//! \return QPE ID.
uint32_t inline spin2_get_qpe_id(void);
//! \brief Returns the core ID. Includes the PE ID. the 2 LSBs are the PE ID, 3 bits are y coordinate and 3 the x coordinate
//! \return QPE ID.
uint32_t inline spin2_get_node_id(void);
//! \brief Returns the (machine-wide) chip ID.
//! \return Chip ID in the bottom 16 bits.
uint32_t inline spin2_get_chip_id(void);

/*!
* \brief
*  Returns a pointer to a newly-allocated block of memory of size "bytes" in
*  DTCM.
*
* \param[in] bytes: size, in bytes, of the requested memory block
*
* \return
*  pointer to the requested memory block, or 0 if unavailable
*/
static inline void* spin2_malloc(uint32_t bytes);

// ------------------------------------------------------------------------

// ----------------------------------
// pseudo-random number generation
// ----------------------------------

/*!
* \brief
*  This function is used to initialize the seed for the
*  pseudo-random number generator.
*/
static inline void spin2_srand(uint seed);

/*!
* \brief
*  This function generates a pseudo-random 32-bit integer,
*  drawing from uniform distribution.
*
* \return
*  32-bit pseudo-random integer
*/
static inline uint spin2_int_rand(void);

/*!
* \brief
*  This function generates a pseudo-random floating point number,
*  drawing from uniform distribution.
*
* \return
*  32-bit pseudo-random integer
*/
static inline float spin2_float_rand(void);

/*!
* \brief
*  This function generates a pseudo-random floating point number,
*  drawing from gaussian distribution.
*
* \return
*  32-bit pseudo-random integer
*/
static inline float spin2_gauss_rand(void) {

// ------------------------------------------------------------------------

#endif /* __SPIN2_API_H__ */