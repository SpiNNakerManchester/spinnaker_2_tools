
#ifndef __IRQ_H__
#define __IRQ_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

//! @ingroup santos
//! @defgroup irq_handlers Interrupt Handlers
//! @brief Declaration of available Interrupt Handlers.
//!
//! Many can be redefined by the user. \n
//! If there is no user definition the corresponding \b __<i>IRQNAME</i>_Handler or @ref Default_Handler implementation will be used. \n
//! In order to switch interrupts on and off use the IRQ Number (@ref IRQn) with the
//! Function @ref NVIC_EnableIRQ and @ref NVIC_DisableIRQ.
//! 
//! @see device.h @see core_cm4.h
//! @{

//! @name Default interrupt handlers (can be overriden by the user)
//! @{


//! @brief Default Reset_Handler. Sets-up stuff like enabling FPU and calls main() for you.
//!
//! @snippet irq_handlers.inl __Reset_Handler
void __Reset_Handler (void);

//! @brief Default Handler, since it is used by the NoC-Burst Interface.
//!
//! @snippet irq_handlers.inl __NoCDoneTxBurst_Handler
void __NoCDoneTxBurst_Handler (void);
//! @brief Default Handler, since it is used by the NoC-Burst Interface.
//!
//! @snippet irq_handlers.inl __NoCDoneRxBurst_Handler
void __NoCDoneRxBurst_Handler (void);
//! @}

//! @brief forever
//!
//! @snippet irq_handlers.inl Default_Handler
void Default_Handler (void);

//! @name Cortex M4 core interrupt handlers
//! @{

void Reset_Handler (void);

void NMI_Handler (void);
void HardFault_Handler (void);
void MemManage_Handler (void);
void BusFault_Handler (void);
void UsageFault_Handler (void);
void SVC_Handler (void);
void DebugMon_Handler (void);
void PendSV_Handler (void);
void SysTick_Handler (void);
//! @}

//! @name Specific interrupt handlers
//! @{
void IRQ_00_Handler (void);
void IRQ_01_Handler (void);
void IRQ_02_Handler (void);
void IRQ_03_Handler (void);
void IRQ_04_Handler (void);
void IRQ_05_Handler (void);
void IRQ_06_Handler (void);
void IRQ_07_Handler (void);
void IRQ_08_Handler (void);
void IRQ_09_Handler (void);
void IRQ_10_Handler (void);
void IRQ_11_Handler (void);
void IRQ_12_Handler (void);
void IRQ_13_Handler (void);
void IRQ_14_Handler (void);
void IRQ_15_Handler (void);

void IRQ_16_Handler (void);
void IRQ_17_Handler (void);
void IRQ_18_Handler (void);
void IRQ_19_Handler (void);
void IRQ_20_Handler (void);
void IRQ_21_Handler (void);
void IRQ_22_Handler (void);
void IRQ_23_Handler (void);
void IRQ_24_Handler (void);
void IRQ_25_Handler (void);
void IRQ_26_Handler (void);
void IRQ_27_Handler (void);
void IRQ_28_Handler (void);
void IRQ_29_Handler (void);
void IRQ_30_Handler (void);
void IRQ_31_Handler (void);
//! @}

//! @}

#ifdef __cplusplus
 }
#endif
#endif /* __IRQ_H__ */

