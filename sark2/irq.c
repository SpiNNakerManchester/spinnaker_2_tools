
#include <stdint.h>
#include "irq.h"
#include "communication.h"

typedef void (*handler) (void);

#include "irq_handlers.inl"

// Defaults:
#define DEF_HAN(handler, default_handler) void handler (void) __attribute__ ((weak, alias (#default_handler)))

// Cortex M4 core interrupt handlers
DEF_HAN (Reset_Handler,                   __Reset_Handler);
DEF_HAN (NMI_Handler,                     Default_Handler);
DEF_HAN (HardFault_Handler,               Default_Handler);
DEF_HAN (MemManage_Handler,               Default_Handler);
DEF_HAN (BusFault_Handler,                Default_Handler);
DEF_HAN (UsageFault_Handler,              Default_Handler);
DEF_HAN (SVC_Handler,                     Default_Handler);
DEF_HAN (DebugMon_Handler,                Default_Handler);
DEF_HAN (PendSV_Handler,                  Default_Handler);
DEF_HAN (SysTick_Handler,                 Default_Handler);

// Timer Interrupts
DEF_HAN (Timer_Int1_Handler,              Default_Handler);
DEF_HAN (Timer_Int2_Handler,              Default_Handler);
DEF_HAN (Timer_IntC_Handler,              Default_Handler);

// Comms-Controller
DEF_HAN (Comms_tx_empty_Handler,          Default_Handler);
DEF_HAN (Comms_tx_full_Handler,           Default_Handler);
DEF_HAN (Comms_rx_not_empty_Handler,      Default_Handler);
DEF_HAN (Comms_rx_not_empty_mc_Handler,   Default_Handler);
DEF_HAN (Comms_rx_not_empty_c2c_Handler,  Default_Handler);
DEF_HAN (Comms_rx_not_empty_nn_Handler,   Default_Handler);
DEF_HAN (Comms_dma_0_not_empty_Handler,   Default_Handler);
DEF_HAN (Comms_dma_0_full_Handler,        Default_Handler);
DEF_HAN (Comms_dma_0_limit_Handler,       Default_Handler);
DEF_HAN (Comms_dma_not_empty_Handler,     Default_Handler);
DEF_HAN (Comms_dma_full_Handler,          Default_Handler);
DEF_HAN (Comms_dma_limit_Handler,         Default_Handler);
DEF_HAN (Comms_dma_read_Handler,          Default_Handler);
DEF_HAN (Comms_dma_write_Handler,         Default_Handler);
DEF_HAN (Comms_ml_Handler,                Default_Handler);
DEF_HAN (Comms_rx_not_empty_info_Handler, Default_Handler);
DEF_HAN (Comms_rx_overrun_Handler,        Default_Handler);
DEF_HAN (Comms_dma_0_overrun_Handler,     Default_Handler);
DEF_HAN (Comms_dma_1_overrun_Handler,     Default_Handler);
DEF_HAN (Comms_int_msg_Handler,           Default_Handler);

// global feedthrough interrupts
DEF_HAN (FT_INT_0_Handler,                Default_Handler);
DEF_HAN (FT_INT_1_Handler,                Default_Handler);
DEF_HAN (FT_INT_2_Handler,                Default_Handler);
DEF_HAN (FT_INT_3_Handler,                Default_Handler);

// regfile interrupt
DEF_HAN (Regfile_INT_0_Handler,           Default_Handler);
DEF_HAN (Regfile_INT_1_Handler,           Default_Handler);
DEF_HAN (Regfile_INT_2_Handler,           Default_Handler);
DEF_HAN (Regfile_INT_3_Handler,           Default_Handler);
DEF_HAN (Regfile_INT_4_Handler,           Default_Handler);
DEF_HAN (Regfile_INT_5_Handler,           Default_Handler);
DEF_HAN (Regfile_INT_6_Handler,           Default_Handler);
DEF_HAN (Regfile_INT_7_Handler,           Default_Handler);
DEF_HAN (Regfile_INT_8_Handler,           Default_Handler);

typedef struct cortex_vec_str {
	uint32_t *stack_top;    // 0
	handler Reset;
	handler NMI;
	handler HardFault;
	handler MemManage;
	handler BusFault;
	handler UsageFault;
	handler Rsvd_7;
	handler Rsvd_8;
	handler Rsvd_9;
	handler Rsvd_10;
	handler SVC;
	handler DebugMon;
	handler Rsvd_13;
	handler PendSV;
	handler SysTickH;

    // Timer Interrupts
    handler Timer_Int1;
    handler Timer_Int2;
    handler Timer_IntC;

    // Comms-Controller
    handler Comms_tx_empty;
    handler Comms_tx_full;
    handler Comms_rx_not_empty;
    handler Comms_rx_not_empty_mc;
    handler Comms_rx_not_empty_c2c;
    handler Comms_rx_not_empty_nn;
    handler Comms_dma_0_not_empty;
    handler Comms_dma_0_full;
    handler Comms_dma_0_limit;
    handler Comms_dma_not_empty;
    handler Comms_dma_full;
    handler Comms_dma_limit;
    handler Comms_dma_read;
    handler Comms_dma_write;
    handler Comms_ml;
    handler Comms_rx_not_empty_info;
    handler Comms_rx_overrun;
    handler Comms_dma_0_overrun;
    handler Comms_dma_1_overrun;
    handler Comms_int_msg;

    // global feedthrough interrupts
    handler FT_INT_0;
    handler FT_INT_1;
    handler FT_INT_2;
    handler FT_INT_3;

    // regfile interrupt
    handler Regfile_INT_0;
    handler Regfile_INT_1;
    handler Regfile_INT_2;
    handler Regfile_INT_3;
    handler Regfile_INT_4;
    handler Regfile_INT_5;
    handler Regfile_INT_6;
    handler Regfile_INT_7;
    handler Regfile_INT_8;
} cortex_vec_t;

// Linker generated symbols.
extern uint32_t _estack;

// Cortex boot vector definition
const cortex_vec_t cortex_vec __attribute__ ((section (".isr_vector"))) =
{
	&_estack,                 // 0:  Stack top
	Reset_Handler,            // 1:  Entry point

	NMI_Handler,              // 2:  NMI Handler
	HardFault_Handler,        // 3:  Hard Fault Handler
	MemManage_Handler,        // 4:  Memory Manager Handler
	BusFault_Handler,         // 5:  Bus Fault Handler
	UsageFault_Handler,       // 6:  Usage Fault Handler
	Default_Handler,          // 7:  (Reserved)
	Default_Handler,          // 8:  (Reserved)
	Default_Handler,          // 9:  (Reserved)
	Default_Handler,          // 10: (Reserved)
	SVC_Handler,              // 11: SVC Handler
	DebugMon_Handler,         // 12: Debug Monitor Handler
	Default_Handler,          // 13: (Reserved)
	PendSV_Handler,           // 14: Pending SV Handler
	SysTick_Handler,          // 15: SysTick Handler

    // Timer Interrupts
    Timer_Int1_Handler,
    Timer_Int2_Handler,
    Timer_IntC_Handler,

    // Comms-Controller
    Comms_tx_empty_Handler,
    Comms_tx_full_Handler,
    Comms_rx_not_empty_Handler,
    Comms_rx_not_empty_mc_Handler,
    Comms_rx_not_empty_c2c_Handler,
    Comms_rx_not_empty_nn_Handler,
    Comms_dma_0_not_empty_Handler,
    Comms_dma_0_full_Handler,
    Comms_dma_0_limit_Handler,
    Comms_dma_not_empty_Handler,
    Comms_dma_full_Handler,
    Comms_dma_limit_Handler,
    Comms_dma_read_Handler,
    Comms_dma_write_Handler,
    Comms_ml_Handler,
    Comms_rx_not_empty_info_Handler,
    Comms_rx_overrun_Handler,
    Comms_dma_0_overrun_Handler,
    Comms_dma_1_overrun_Handler,
    Comms_int_msg_Handler,

    // global feedthrough interrupts
    FT_INT_0_Handler,
    FT_INT_1_Handler,
    FT_INT_2_Handler,
    FT_INT_3_Handler,

    // regfile interrupt
    Regfile_INT_0_Handler,
    Regfile_INT_1_Handler,
    Regfile_INT_2_Handler,
    Regfile_INT_3_Handler,
    Regfile_INT_4_Handler,
    Regfile_INT_5_Handler,
    Regfile_INT_6_Handler,
    Regfile_INT_7_Handler,
    Regfile_INT_8_Handler,
};

// Fudge to force 4 byte alignment
const int pad __attribute__ ((section (".padding"))) = 0xdeaddead;
