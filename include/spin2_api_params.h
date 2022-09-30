#ifndef __SPIN2_API_PARAM_H__
#define __SPIN2_API_PARAM_H__


/* internal error/warning return codes */
#define NO_ERROR              0
#define TASK_QUEUE_FULL       1
#define DMA_QUEUE_FULL        2
#define PACKET_QUEUE_FULL     4
#define WRITE_BUFFER_ERROR    8
#define SYNCHRO_ERROR         16
#define COULD_NOT_ADD_SPIKE   32
#define COULD_NOT_TRIGGER_USER_EVENT 64
#define SPIKE_BUFFER_OVERFLOW 128
#define NOT_DONE_IN_ONE_TIME_STEP 256

// --------------------------------

// ----------------
/* data transfer */
// ----------------
// DMA transfer parameters: 4-doubleword bursts
//TODO: may need adjustment for SpiNNaker
#define DMA_BURST_SIZE        4
#define DMA_WIDTH             1
// internal DMA queue size
#define DMA_QUEUE_SIZE        12
// select write buffer use
#define USE_WRITE_BUFFER      FALSE
// ---------------------------

// -----------------
// communications */
// -----------------
#define TX_PACKET_QUEUE_SIZE  16
// TX control register programming
#define TX_TCR_MCDEFAULT      0x00000000
#define TX_FULL_MASK          0x40000000
#define TX_EMPTY_MASK         0x80000000
#define RX_RECEIVED_MASK      0x80000000
// -----------------

// --------------------
/* memory allocation */
// --------------------
// memory space reserved for RTS stacks
#define RTS_STACKS            4096
// --------------------

// -----------------------
/* scheduler/dispatcher */
// -----------------------
// callback queue parameters
#define NUM_PRIORITIES    3
#define TASK_QUEUE_SIZE   16
#define N_TASK_QUEUES 2 // NUM_PRIORITIES-1
// -----------------------

// -----------------------
// Allocation of entries in MC table
// -----------------------

//#define SYS_MC_ENTRIES 24
#define SYS_MC_ENTRIES 1
#define APP_MC_ENTRIES (MC_TABLE_SIZE - SYS_MC_ENTRIES)


// -----------------------
// VIC priorities
// -----------------------
#define SARK_PRIORITY          0
#define RX_READY_PRIORITY      3
#define DMA_DONE_PRIORITY      2
#define TIMER1_PRIORITY        1
#define CC_TMT_PRIORITY        4
#define SOFT_INT_PRIORITY      5
#if USE_WRITE_BUFFER == TRUE
  #define DMA_ERR_PRIORITY     6
#endif

// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// helpful macros
// ------------------------------------------------------------------------
#define CHIP_ADDR(x, y)      ((x << 8) | y)
#define P2P_ROUTE(addr)      (1 << p2p_get(addr))
#define CORE_ROUTE(core)     (1 << (core + NUM_LINKS))
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// internal pre-defined types -- not visible in the API
// ------------------------------------------------------------------------
// ----------------
/* data transfer */
// ----------------
typedef struct
{
  uint32_t id;
  uint32_t tag;
  uintptr_t system_address;
  uintptr_t tcm_address;
  uint32_t description;
} copy_t;


typedef struct
{
  uint32_t start;
  uint32_t end;
  copy_t queue[DMA_QUEUE_SIZE];
} dma_queue_t;
// ----------------

// -----------------
/* communications */
// -----------------
typedef struct
{
  uint32_t key;
  uint32_t data;
  uint32_t load;
} packet_t;

typedef struct
{
  uint32_t start;
  uint32_t end;
  packet_t queue[TX_PACKET_QUEUE_SIZE];
} tx_packet_queue_t;
// -----------------

// -----------------------
/* scheduler/dispatcher */
// -----------------------
typedef struct
{
  callback_t cback;
  int priority;
} cback_t;

typedef struct
{
  callback_t cback;
  uint32_t arg0;
  //debug!
  //to save memory
  uint32_t arg1;
} task_t;

typedef struct
{
  uint32_t start;
  uint32_t end;
  task_t queue[TASK_QUEUE_SIZE];
} task_queue_t;

extern cback_t callback[NUM_EVENTS];

#endif /* __SPIN2_API_PARAM_H__ */