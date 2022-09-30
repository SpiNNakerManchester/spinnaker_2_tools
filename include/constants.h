
#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ft_irq.h"
#include "gpio_cfg.h"

//! @ingroup qpe
//! @defgroup const Fixed constants
//! @brief AHB/NoC Addresses, routertable properties, etc.
//! @{

//------------------------------------------------------------------------------
//! @defgroup  ahb_addr Local AHB Addresses
//! @{


//! From datasheet
#define        NMU_KISS_BASE          0xE0100000
#define NMU_TRNG_CONFIG_BASE          0xE0100080
#define NMU_TRNG_RANDOM_BASE          0xE0100090
#define         NMU_EXP_BASE          0xE0100100

#define          TIMER1_BASE          0xE1000000
#define          TIMER2_BASE          0xE1000020

#define           COMMS_BASE          0xE2000000
#define          ML_ACC_BASE          0xE3000000

#define            DRAM_BASE          0x20000000

#define QUADPE_NOC_SRAM_BASE          0xF0000000

#define  QUADPE_REGFILE_BASE          0xF2000000
#define JIBMGMT_REGFILE_BASE          0xF6000000
#define   UART0_REGFILE_BASE          0xF6000400
#define   UART1_REGFILE_BASE          0xF6000500
#define JIBCORE_REGFILE_BASE          0xF6002000

#define      SRAM_LOCAL_BASE          0x00000000
#define       SRAM_QUAD_BASE          0x00080000


//! @}
//! @}

#ifdef __cplusplus
 }
#endif

#endif /* __CONSTANTS_H__ */

