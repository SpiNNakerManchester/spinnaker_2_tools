
#ifndef __DEVICE_H__
#define __DEVICE_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "attributes.h"

#define __CM4_REV                 0x0000
#define __MPU_PRESENT             1         // MPU present
#define __FPU_PRESENT             1         // FPU present
#define __NVIC_PRIO_BITS          5         // Bits used for Priority Levels
#define __Vendor_SysTickConfig    1

typedef enum IRQn
{
    // Cortex-M4 Processor Exception Numbers
    NonMaskableInt_IRQn         = -14, // !< 2 Non Maskable Interrupt
    MemoryManagement_IRQn       = -12, // !< 4 Memory Management Interrupt
    BusFault_IRQn               = -11, // !< 5 Bus Fault Interrupt
    UsageFault_IRQn             = -10, // !< 6 Usage Fault Interrupt
    SVCall_IRQn                 = -5,  // !< 11 SV Call Interrupt
    DebugMonitor_IRQn           = -4,  // !< 12 Debug Monitor Interrupt
    PendSV_IRQn                 = -2,  // !< 14 Pend SV Interrupt
    SysTick_IRQn                = -1,  // !< 15 System Tick Interrupt

    // Timer Interrupts
    Timer_Int1_IRQn              = 0,   // !< IRQ_00
    Timer_Int2_IRQn              = 1,   // !< IRQ_01
    Timer_IntC_IRQn              = 2,   // !< IRQ_02

    // Comms-Controller
    Comms_tx_empty_IRQn          = 3,   // !< IRQ_03
    Comms_tx_full_IRQn           = 4,   // !< IRQ_04
    Comms_rx_not_empty_IRQn      = 5,   // !< IRQ_05
    Comms_rx_not_empty_mc_IRQn   = 6,   // !< IRQ_06
    Comms_rx_not_empty_c2c_IRQn  = 7,   // !< IRQ_07
    Comms_rx_not_empty_nn_IRQn   = 8,   // !< IRQ_08
    Comms_dma_0_not_empty_IRQn   = 9,   // !< IRQ_09
    Comms_dma_0_full_IRQn        = 10,  // !< IRQ_10
    Comms_dma_0_limit_IRQn       = 11,  // !< IRQ_11
    Comms_dma_not_empty_IRQn     = 12,  // !< IRQ_12
    Comms_dma_full_IRQn          = 13,  // !< IRQ_13
    Comms_dma_limit_IRQn         = 14,  // !< IRQ_14
    Comms_dma_read_IRQn          = 15,  // !< IRQ_15
    Comms_dma_write_IRQn         = 16,  // !< IRQ_16
    Comms_ml_IRQn                = 17,  // !< IRQ_17
    Comms_rx_not_empty_info_IRQn = 18,  // !< IRQ_18
    Comms_rx_overrun_IRQn        = 19,  // !< IRQ_19
    Comms_dma_0_overrun_IRQn     = 20,  // !< IRQ_20
    Comms_dma_1_overrun_IRQn     = 21,  // !< IRQ_21
    Comms_int_msg_IRQn           = 22,  // !< IRQ_22

    // global feedthrough interrupts
    FT_INT_0_IRQn                = 23,  // !< IRQ_23
    FT_INT_1_IRQn                = 24,  // !< IRQ_24
    FT_INT_2_IRQn                = 25,  // !< IRQ_25
    FT_INT_3_IRQn                = 26,  // !< IRQ_26

    // regfile interrupt
    Regfile_INT_0_IRQn           = 27,  // !< IRQ_27
    Regfile_INT_1_IRQn           = 28,  // !< IRQ_28
    Regfile_INT_2_IRQn           = 29,  // !< IRQ_29
    Regfile_INT_3_IRQn           = 30,  // !< IRQ_30
    Regfile_INT_4_IRQn           = 31,  // !< IRQ_31
    Regfile_INT_5_IRQn           = 32,  // !< IRQ_32
    Regfile_INT_6_IRQn           = 33,  // !< IRQ_33
    Regfile_INT_7_IRQn           = 34,  // !< IRQ_34
    Regfile_INT_8_IRQn           = 35,  // !< IRQ_35
} IRQn_Type;

#include "core_cm4.h"

__static_inline uint32_t SysTick_Config(uint32_t ticks)
{
  if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
  {
    return (1UL);                                                   /* Reload value impossible */
  }

  SysTick->LOAD  = (uint32_t)(ticks - 1UL);                         /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
	  				SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
  return (0UL);                                                     /* Function successful */
}

__static_inline  void enable_fpu (void)
{
    SCB->CPACR |= (3 << (10 * 2)) | (3 << (11 * 2));
    //  __DSB (); // Not needed?
    //  __ISB ();
}

#ifdef __cplusplus
 }
#endif

#endif /* __DEVICE_H__ */
