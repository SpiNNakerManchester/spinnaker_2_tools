#ifndef __SARK_2_H__
#define __SARK_2_H__

#include "device.h"
#include "irq.h"
#include "constants.h"
#include "communication.h"
#include "nmu.h"

#include "random.h"

/*!
@mainpage

@htmlinclude general/title.html

------------------------------------------------------------------

@section sec_overview General overview:
@subsection sec_armm4 Cortex M4 Processor
Each QuadPE Core consist of 4x **ARM Cortex M4 Processors** \n
ARM cortex M4 processor is included with the following features:
 - Single precision floating point (+50% area)
 - 3 AHB lite buses (not multi master bus)

@subsection sec_ahb AHB Extensions (NoC and CoProcessing):
 -# @ref nmu_wrap \n
  A math unit for calculation of **exponential functions**
  is included as AHB slave within the ARM processor sub-system. \n
  The unit employs the standard s16.15 fixed-point format for both operand and result.
  A pseudo- **random number generator** is included as AHB slave within the ARM processor sub-system.
  It provides pseudo random number generation and can optionally be randomized by a true random number signal

*/

// -------------------------------- Files ------------------------------------------
//! @file constants.h
//!  @brief  Hardware related constants (from datasheet)
//! @file device.h
//!  @brief  IRQ-Number definitions.
//! @file irq.h
//!  @brief  Avaible Interrupt-Handler declaration
//! @file nmu.h
//!  @brief  Functions to access the Neuromorphic Math Unit
//! @file qpe-types.h
//!  @brief  Hardware related datastructures (AHB, NoC)


//!@defgroup SARK2
//!@brief Lowlevel function for accessing the QuadPE Platform


// Flags to sark_xalloc() (etc)

#define ALLOC_LOCK      1       //!< Lock this operation
#define ALLOC_ID        2       //!< Use supplied AppID

//! This allows for a "retry" of the allocation request when an SDRAM tag is
//! specified.  If this flag is specified, and the tag in use, and the size of
//! the allocation requested is the same as the size currently allocated, just
//! return the address of the tag from the previous allocation.  Without this
//! flag, the previous behavior is maintained, meaning that each tag can only
//! be allocated once, and a re-allocation request will result in a NULL being
//! returned.
#define ALLOC_TAG_RETRY 4

/*!
Allocations of SysCtl Test & Set registers (locks)
*/
// Possible remove? Might do that differently

enum spin_lock_e {
    LOCK_MSG,           //!< Msg buffers in SysRAM
    LOCK_MBOX,          //!< Mailbox flag variable
    LOCK_ETHER,         //!< Ethernet Tx (unused...)
    LOCK_GPIO,          //!< GPIO port (unused...)
    LOCK_API_ROOT,      //!< Spin1 API
    LOCK_SEMA,          //!< Sema access
    LOCK_HEAP,          //!< Heap in System / SDRAM
    LOCK_RTR            //!< Router
};

typedef enum spin_lock_e spin_lock; //!< Typedef for enum spin_lock_e

//! Run time error codes
//! These are the same inherited from sark. We might want to change some in the future
enum rte_code_e {
    RTE_NONE,           //!< 0 No error
    RTE_RESET,          //!< 1 Branch through zero
    RTE_UNDEF,          //!< 2 Undefined instruction
    RTE_SVC,            //!< 3 Undefined SVC or no handler
    RTE_PABT,           //!< 4 Prefetch abort
    RTE_DABT,           //!< 5 Data abort
    RTE_IRQ,            //!< 6 Unhandled IRQ
    RTE_FIQ,            //!< 7 Unhandled FIQ
    RTE_VIC,            //!< 8 Unconfigured VIC vector

    RTE_ABORT,          //!< Generic user abort
    RTE_MALLOC,         //!< "malloc" failure
    RTE_DIV0,           //!< Divide by zero
    RTE_EVENT,          //!< Event startup failure
    RTE_SWERR,          //!< Fatal SW error
    RTE_IOBUF,          //!< Failed to allocate IO buffer
    RTE_ENABLE,         //!< Bad event enable
    RTE_NULL,           //!< Generic null pointer error
    RTE_PKT,            //!< Pkt startup failure
    RTE_TIMER,          //!< Timer startup failure
    RTE_API,            //!< API startup failure
    RTE_VER             //!< SW version conflict
};

typedef enum rte_code_e rte_code;   //!< Typedef for enum rte_code_e


// Heap data structures
// Defined according to sark from Spin1, as they represent mem blocks

/*!
\brief Heap data block.
One of these appears at the start of each
block in the heap so each allocation of N bytes in the heap
requires N+8
*/

typedef struct block {
    struct block *next;         //!< Chains all blocks together (in address order)
    struct block *free;         //!< Chains free blocks together (in address order)
} block_t;

/*!
\brief Heap root structure.
One of these appears at the start of the
heap area and maintains two lists, one containing all blocks in
the heap and one containing only free blocks. The heap always
contains a 'last' block, of zero size, which is used to mark the
end of the heap.
*/

typedef struct {
    block_t *free;              //!< Root of free block chain
    block_t *first;             //!< First block
    block_t *last;              //!< Last block (zero size, never used for storage)
    uint free_bytes;            //!< Number of free bytes left
    uchar buffer[];             //!< Buffer for blocks
} heap_t;


//------------------------------------------------------------------------------


#if 0
// Hack to stop simulation by branching to an undefined area of memory
// (after saving a result code)
void stop_sim (uint32_t code)
{
  __DSB ();

  asm ("movw r0, #0xfff0");
  asm ("bx r0");
}
#endif

/*!
\brief Allocate a memory block from the specified heap.
The block will be
tagged with the supplied "tag" which must be <= 255. Returns NULL on
failure (not enough memory, bad tag or tag in use).
The flag parameter contains two flags in the bottom two bits. If bit 0
is set (#ALLOC_LOCK), the heap manipulation is done behind a lock with
interrupts disabled. If bit 1 is set (#ALLOC_ID), the block is tagged
with the AppID provided in bits 15:8 of the flag, otherwise the AppID
of the current application is used.
The 8-bit \p tag is combined with the AppID and stored in the "free"
field while the block is allocated. If the \p tag is non-zero, the
block is only allocated if the tag is not in use.  The tag (and AppID)
is stored in the "alloc_tag" table while the block is allocated.
If this command is used to allocate for an app-id which isn't the
caller's, the user should set `sv->app_data[app_id].clean` to 0 for the
application concerned.  This ensures that the memory will be cleaned
up by a stop signal.
\param[in] heap: the heap from which the block should be allocated
\param[in] size: the size of the block in bytes
\param[in] tag: 8-bit tag
\param[in] flag: flags to control locking and AppID
\return pointer to allocated block or NULL on failure
*/

void *sark2_xalloc(heap_t *heap, uint size, uint tag, uint flag);

/*!
\brief Called to signal a fatal error.

\param[in] code: an error code
\param ... further (integer) arguments
*/

void rt_error(uint code, ...);

/*!
\brief Copy \p n bytes of memory from \p src to \p dest.
The memory is copied
byte by byte and so will be inefficient for large values of \p n.
Not intended for use where source and destination blocks overlap.
\param[in] dest: destination buffer address
\param[in] src: source buffer address
\param[in] n: number of bytes to copy
*/

void sark2_memcpy(void *dst, const void *src, uint32_t len);

/*!
\brief Enable the FIQ and IRQ interrupts in the core.
*/
void inline sark2_int_enable(void);

/*!
\brief Disable the FIQ and IRQ interrupts in the core.
*/
void inline sark2_int_disable(void);

/*!
\brief Disable the IRQ interrupt in the core.
*/
void inline sark2_irq_disable(void);

/*!
\brief Restore interrupts after sark2_irq_disable.
*/
void inline sark2_mode_restore(void);

/*!
\brief Puts the core into sleep mode until an interrupt occurs at which point
the function will return.
*/
void sark2_wfi (void);

/*!
\brief Puts the core into sleep mode until an event occurs at which point
the function will return.
*/
void sark2_wfe (void);

//! \brief Returns the core ID.
//! \return PE ID.
uint32_t inline sark2_pe_id(void);

//! \brief Returns the QPE ID without the PE ID. Y in bottom 3 bits, X in higher 3 bits
//! \return QPE ID.
uint32_t inline sark2_qpe_id(void);

//! \brief Returns the core ID. Includes the PE ID. PE ID in bottom 2 bits, then Y on 3 bits and X on 3 bits
//! \return QPE ID.
uint32_t inline sark2_node_id(void);

//! \brief Returns the (machine-wide) chip ID.
//! \return Chip ID in the bottom 16 bits.
uint32_t inline sark2_chip_id(void);

/*!
\brief Counts the number of characters in a zero terminated string.
The terminator is not included in the count.
\param string: zero terminated string
\return number of characters
*/

uint32_t sark2_str_len (char *string);


/*!
\brief Count the number of bits set in a word
\param word: whose bits are to be counted
\result number of bits set
*/

uint32_t sark_count_bits(uint32_t word);

/*!
\brief Copies a NULL terminated string from \p src to \p dest.
The string is copied byte by byte and so will be inefficient for long
strings. Not intended for use where source and destination strings overlap.
\param[in] dest: destination string address; must be enough space available
\param[in] src: source string address
*/

void sark2_str_cpy(char *dest, const char *src);

/*!
\brief Compares two strings.
\param[in] s1: first string
\param[in] s2: seocnd string
\return signed integer expressing the result of the comparison
*/
int sark2_str_cmp(const char* s1, const char* s2);

#endif /* __SARK_2_H__ */