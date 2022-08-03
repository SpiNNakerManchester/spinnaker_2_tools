#ifndef __SPIN2_API_H__
#define __SPIN2_API_H__

#endif /* __SPIN2_API_H__ */

/*
typedef uint32_t uint;
*/
typedef unsigned char uchar;


volatile bool API_BURST_FINISHED ;

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

#define NUM_EVENTS            6
#define MC_PACKET_RECEIVED    0
#define DMA_TRANSFER_DONE     1
#define TIMER_TICK            2
#define SDP_PACKET_RX         3
#define USER_EVENT            4
#define MCPL_PACKET_RECEIVED  5

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

// ------------------------------------------------------------------------
// callback and task functions
// ------------------------------------------------------------------------
void spin2_callback_on(uint32_t event_id, callback_t cback, int priority);
void spin2_callback_off(uint32_t event_id);
uint32_t spin2_schedule_callback(callback_t cback, uint32_t arg0, uint32_t arg1, int priority);

// ------------------------------------------------------------------------
//  data transfer functions
// ------------------------------------------------------------------------

void spin2_dma_transfer(uint32_t tag, void *system_address, void *tcm_address, uint32_t direction, uint32_t length);
void spin2_memcpy(void *dst, void const *src, uint32_t len);
// ------------------------------------------------------------------------


uint32_t spin2_send_packet(uint32_t key, uint32_t data, uint32_t TCR);

// ------------------------------------------------------------------------
//  system resources access functions
// ------------------------------------------------------------------------
//! \brief Returns a global (machine-wide) ID for the processor.
//! \return Chip ID in bits [20:5], core ID in bits [4:0].
uint32_t  spin2_get_id(void);
//! \brief Returns the core ID.
//! \return Core ID in the bottom 5 bits.
uint32_t  spin2_get_core_id(void);
//! \brief Returns the (machine-wide) chip ID.
//! \return Chip ID in the bottom 16 bits.
uint32_t  spin2_get_chip_id(void);
