
#ifndef __QPE_TYPES_H__
#define __QPE_TYPES_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

#include "constants.h"
#include "fixedpoint.h"
#include "attributes.h"
#include "quadpe_regfile.h"
#include "jibcore_regfile.h"
#include "jibmgmt_regfile.h"

//! @ingroup qpe
//! @defgroup qpe_types Data structures
//! @brief Data structure for memory mapped features of the qpe.
//!
//! @note Bitfields are readonly. \n
//! The datatype of configuration words and bit is uint32_t/uint8_t.
//! They are replaced by doxygen in order to achieve a better readability
//!
//! @{

//! @brief Datastructure for AHB Network-on-Chip memory mapped DRAM access
//! @defgroup union_dram DRAM handling
//! @{
typedef union dram_union {
    uint32_t words[1];
    uint16_t half_words[2];
    uint8_t  bytes[4];
} dram_t;
//! Global Variable to access the DRAM. See @ref dram_union for details.
#define DRAM ((volatile dram_t *)DRAM_BASE)
//! @}

//! @brief Datastructure for AHB Network-on-Chip Access
//! @defgroup struct_noc AHB-Comms Handling
//! @{
typedef struct comms_str {
    _config_word (TCR0)
    _config_word (TDR03)
    _config_word (TDR02)
    _config_word (TDR01)
    _config_word (TDR00)
    _config_word (TKR0)
    _config_word (UNUSED0[2])
    _config_word (TCR1)
    _config_word (TDR13)
    _config_word (TDR12)
    _config_word (TDR11)
    _config_word (TDR10)
    _config_word (TKR1)
    _config_word (UNUSED1[2])
    _config_word (TCR2)
    _config_word (TDR23)
    _config_word (TDR22)
    _config_word (TDR21)
    _config_word (TDR20)
    _config_word (TKR2)
    _config_word (UNUSED2[2])
    _config_word (TCR3)
    _config_word (TDR33)
    _config_word (TDR32)
    _config_word (TDR31)
    _config_word (TDR30)
    _config_word (TKR3)
    _config_word (UNUSED3[1])
    _config_word (TCTL)
    _config_word (RCR)
    _config_word (RDR3)
    _config_word (RDR2)
    _config_word (RDR1)
    _config_word (RDR0)
    _config_word (RKR)
    _config_word (RDC)
    _config_word (RCTL)
    _config_word (UNUSED4[22])
    _begin_config_word (ID)
        //1st byte
        _field (MyPEID,    8)  //!< ID of PE
        //2nd byte
        _pad   (           8)
        //3rd/4th bytesCOMMS->ID_field.MyPEID
        _field (MyChipID, 16)  //!< ID of Chip
    _end_config_word (ID)
    _begin_config_word (GCTL)
        _field (unknown,  11)
        _field (buffered_write,  1)  //!< use buffered write
        _field (unknown2, 20)
    _end_config_word (GCTL)
    _config_word (DMAc0)
    _config_word (DMAs0)
    _config_word (DMAe0)
    _config_word (DMAr0)
    _config_word (DMAw0)
    _config_word (DMAl0)
    _config_word (DBO0)
    _config_word (DMax0)
    _config_word (DDC0)
    _config_word (UNUSED5[6])
    _config_word (DCTL0)
    _config_word (DMAc1)
    _config_word (DMAs1)
    _config_word (DMAe1)
    _config_word (DMAr1)
    _config_word (DMAw1)
    _config_word (DMAl1)
    _config_word (DBO1)
    _config_word (DMax1)
    _config_word (DDC1)
    _config_word (UNUSED6[6])
    _config_word (DCTL1)
    _config_word (LOCAL_ADDR)
    _config_word (REMOTE_ADDR)
    _config_word (DMA_LEN)
    _config_word (DMA_CFG)
    _config_word (DMA_DONE)
    _config_word (DMA_STATUS)
    _config_word (UNUSED7[9])
    _config_word (DMA_CMD)
} comms_str_t;
//! Global Variable to access the Comms Controller. See @ref comms_str for details.
#define COMMS ((volatile comms_str_t *)COMMS_BASE)
//! @}

//! @brief Datastructure for AHB-Generalpurpose interrupts
//! @defgroup struct_gpint AHB-Generalpurpose interrupts
//! @{
typedef struct gpint_str {
    //! Reads current interrupts / Disables current interrupts (Write '1' to reset)
    _config_word (INT0)
    //! Reads current interrupts / Disables current interrupts (Write '1' to reset)
    _config_word (INT1)
    //! Reads current interrupts / Disables current interrupts (Write '1' to reset)
    _config_word (INT2)
    //! Reads current interrupts / Disables current interrupts (Write '1' to reset)
    _config_word (INT3)
} gpint_str_t;
//! Global Variable to access Generalpurpose interrupts. See @ref gpint_str for details.
#define GP_INT ((volatile gpint_str_t *)GPINT_BASE)
//! @}

//! @brief Datastructure for Timer interrupt handling.
//! @defgroup struct_tc_str AHB-Additional Timers
//! @{
typedef struct tc_str {
  volatile uint32_t LOAD;     //
  volatile uint32_t COUNT;    //
  volatile uint32_t CONTROL;  //
  volatile uint32_t INT_CLR;  //
  volatile uint32_t RAW_INT;  //
  volatile uint32_t MASK_INT; //
  volatile uint32_t BG_LOAD;  //
} tc_str_t;
//! @brief Global Variable to access the Timer1.
#define TC1 ((volatile tc_str_t *)TIMER1_BASE)
//! @brief Global Variable to access the Timer2.
#define TC2 ((volatile tc_str_t *)TIMER2_BASE)
//! @}


//! @brief Datastructure for AHB ML Accelerator Configuration
//! @defgroup struct_ml_acc ML Config
//! @{
typedef struct ml_acc_struct {
    //! Source type (2 MSB) and address of operand A
    _config_word (source_a)
    //! Source type (2 MSB) and address of operand B
    _config_word (source_b)
    //! Address of result
    _config_word (target)
    //! Number of rows for operand A
    _config_word (rows_a)
    //! Number of columns for operand A, and number of rows of operand B
    _config_word (cols_a_rows_b)
    //! Number of columns for operand B
    _config_word (cols_b)
    //! Trigger command
    _config_word (trigger_command)
    //! Data for operand A, written via AHB
    _config_word (data_a0)
    _config_word (data_a1)
    _config_word (data_a2)
    _config_word (data_a3)
    //! Additional parameters for convolution
    _config_word (conv_params1)
    _config_word (conv_params2)
    _config_word (conv_params3)
    //! Noc dst
    _config_word (noc_src)
    _config_word (noc_dst)
} ml_acc_t;
//! Global Variable to access the ML accelerator. See @ref struct_ml_acc for details.
#define ML_ACC ((volatile ml_acc_t *)ML_ACC_BASE)
//! @}


//! @brief Datastructures for the AHB mapped NMU.
//!
//! The Neuromorphic Math Unit (NMU) provides acceleration for the exponential function
//! and for random number generation. There are 3 random number generators (2x KISS32, 1x KISS64).
//!
//! @defgroup arm_nmu  AHB-NMU (Exponential, Random)
//! @{

//! @name Exponential acceleration unit
//! @{
//
//! @brief Pipelined Memory access to the hardware exponential function accelerator. The pipeline deep is 4.
//!
//!  @htmlinclude general/exp.html
//!  @see nmu_wrap
//! S16.15 exp access
#define NMU_EXP ((volatile fixpt_s1615 *)NMU_EXP_BASE)
//! S0.31 exp access
#define NMU_EXP_FRAC ((volatile fixpt_s1615 *)NMU_EXP_BASE+1)
//! S16.15 log access
#define NMU_LOG ((volatile fixpt_s1615 *)NMU_EXP_BASE+3)
//! S0.31 log access
#define NMU_LOG_FRAC ((volatile fixpt_s1615 *)NMU_EXP_BASE+4)
//! Control register to configure iterations
#define NMU_EXP_LOG_CONTROL ((volatile fixpt_s1615 *)NMU_EXP_BASE+6)
//! @}


//! @name Random acceleration units
//! @{

//! @brief Datastructure for KISS32 PRNG
typedef struct kiss_str {
	//! 4x 32bit array seed
	_config_word (seed[5])
	//! readonly 32 bit integer random value
    _config_word_RO (rnd_value)
	//! 32 bit floatingpoint random value
    _config_float_RO (rnd_value_float)
    _config_word_RO (pad)
} kiss_str_t;

//! @brief Datastructure for True-Random-Bus configuration
#define TRB_WIDTH 2
typedef struct trb_config_str {
    _config_word (trb_config_sample_rate)
    _begin_config_word (trb_config_biw)
        _field (biw_decor_count, 8)
        _field (biw_en, TRB_WIDTH)
    _end_config_word (trb_config_biw)
    _begin_config_word (trb_config_ppmin)
        _field (pp_min_dte, 1)
        _field (pp_min_rotate, 4)
        _field (pp_min_en, TRB_WIDTH)
    _end_config_word (trb_config_ppmin)

    _begin_config_word (trb_config_scr_data)
        _field (biw_scr_sel, TRB_WIDTH)
        _field (pp_min_scr_sel, TRB_WIDTH)
    _end_config_word (trb_config_scr_data)
} trb_config_str_t;

//! @brief Datastructure for reading values of the True-Random-Bus
typedef struct trb_random_str {
    _config_word (biw[TRB_WIDTH])
    _config_word (pp_min[TRB_WIDTH])
} trb_random_str_t;

//! @brief Global Variable to access the first KISS32 PRNG
#define NMU_KISS ((volatile kiss_str_t *)NMU_KISS_BASE)
//! @brief Global Variable to access the TRB config
#define NMU_TRNG_CONFIG ((volatile trb_config_str_t *)NMU_TRNG_CONFIG_BASE)
//! @brief Global Variable to read the TRB random values
#define NMU_TRNG_RANDOM ((volatile trb_random_str_t*)NMU_TRNG_RANDOM_BASE)

//endname random
//  @}

//! @}


//endgroup qpe_types
//! @}

typedef struct uart_config_str {
    _begin_config_word (ra_config_status)
        _field (twostoppbits, 1)
        _field (ignore_errors, 1)
        _field (parity_en, 1)
        _field (rtr_cts_en, 1)
        _field (rtr_cts_lowactive, 1)
    _end_config_word (ra_config_status)
    _config_word (ra_clkdiv)
    _begin_config_word (ra_simple_read)
        _field (rd_byte, 8)
        _field (rd_valid, 1)
    _end_config_word (ra_simple_read)
    _begin_config_word (ra_simple_write)
        _field (wr_byte, 8)
        _field (wr_valid, 8)
    _end_config_word (ra_simple_write)

    _config_word (ra_buf_read_addr)
    _begin_config_word (ra_buf_read_ctrl)
        _field (rd_ctrl_len, 16)
        _field (rd_ctrl_enable, 1)
        _field (rd_index_reset, 1)
        _field (rd_stat_error, 1)
        _field (delim_irq_reset, 1)
        _field (unused, 4)
        _field (delim_char, 8)
    _end_config_word (ra_buf_read_ctrl)
    _config_word (ra_buf_read_ridx)
    _config_word (ra_buf_read_widx)

    _config_word (ra_buf_write_addr)
    _begin_config_word (ra_buf_write_ctrl)
        _field (wr_ctrl_len, 16)
        _field (wr_ctrl_en, 1)
        _field (wr_stat_done, 1)
        _field (wr_stat_error, 1)
    _end_config_word (ra_buf_write_ctrl)
    _config_word (ra_buf_write_reserved[2])

    _config_word (ra_sgmnt_ctrl_read)  // 2bit-segment
    _config_word (ra_sgmnt_ctrl_write) // 2bit-segment
}  uart_config_str_t;

#define UART0 ((volatile uart_config_str_t *)UART0_REGFILE_BASE)
#define UART1 ((volatile uart_config_str_t *)UART1_REGFILE_BASE)

#ifndef DOXYGEN
#define __compiletime_test_size(type, size) char type##_size_correct[sizeof(type)==size ? 1 : -1]
union union_size_test
{
	__compiletime_test_size(gpint_str_t,      4*4);
	__compiletime_test_size(kiss_str_t,       8*4);
	__compiletime_test_size(trb_config_str_t, 4*4);
	__compiletime_test_size(trb_random_str_t, 2*TRB_WIDTH*4);
    __compiletime_test_size(uart_config_str_t, 14*4);
    __compiletime_test_size(quadpe_config_str_t, 1<<19);
};

#endif

#define QUADPE_REGFILES ((volatile quadpe_config_str_t *)QUADPE_REGFILE_BASE)
#define JIBCORE_REGFILE ((volatile jibcore_config_str_t *)JIBCORE_REGFILE_BASE)
#define JIBMGMT_REGFILE ((volatile jibmgmt_config_str_t *)JIBMGMT_REGFILE_BASE)

#ifdef __cplusplus
 }
#endif

#endif /* __QPE_TYPES_H__ */

