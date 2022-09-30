
#include <stdint.h>
#include <stdbool.h>
#include "irq.h"
#include "device.h"
#include "attributes.h"
#include "random.h"

void record_init  (void)       __attribute__ ((weak, alias ("__do_nothing")));
void record_final (void)       __attribute__ ((weak, alias ("__do_nothing")));
void _monInit     (void)       __attribute__ ((weak, alias ("__do_nothing")));
void SystemInit   (void)       __attribute__ ((weak, alias ("__do_nothing")));
//void exit         (int status) __attribute__ ((weak, alias ("_exit")));

void __do_nothing (void)
{
}

//volatile uint32_t exit_code __attribute__ ((section(".exit_value")));

/*
void _exit (int status)
{
    record_final ();

    // Disable all interrupts:
    for (int i=0; i<31; i++) {
        NVIC_DisableIRQ ((IRQn_Type)i);
    }
    exit_code = status;
    for (;;) {
        __WFE ();
    }
}
*/

#include "nmu.h"

//! @name Linker generated symbols
//! @{

extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata, _edata;
extern uint32_t __bss_start__, __bss_end__;

//! @}

extern int main (void);
// Called from Cortex reset vector. Sets up RW and ZI variables,
// enables FPU and then calls "main"

uint8_t _MYCHIPID;
uint8_t _MYPEID;

void _monInit (void);

//! [__Reset_Handler]
void __attribute__((interrupt, noreturn)) __Reset_Handler (void)
{
    // Copy initialize data section from 'ITCM' to 'DTCM'
    uint32_t *init_data = (uint32_t *)&_sidata;
    uint32_t *data = (uint32_t *)&_sdata;
    //exit_code      = 0xdeaddead;

	/*
	 * this is commented out since there is on distinction between rom and ram
	 *
    while (data < &_edata) {
        *data++ = *init_data++;
    }
	*/




    // Initialize BSS section with zero's:
    uint32_t *bss = &__bss_start__;

    while (bss < &__bss_end__) {
        *bss++ = 0;
    }

    // Fill stack with 0xdeaddead
#if 0
    uint32_t *stack = STACK_BASE;
    uint32_t stack_size = STACK_SIZE;

    while (stack_size--) {
        *stack++ = 0xdeaddead;
    }

#endif

    _MYCHIPID = _getMyChipID ();
    _MYPEID   = _getMyPEID ();

    set_buffered_write (true);
    srand_soft ();
    srand_hard ();
    enable_fpu ();

    record_init ();

    _monInit ();
    SystemInit ();


    int exitval = main ();
    //_exit (exitval);
}
//! [__Reset_Handler]

//! [Default_Handler]
void __attribute__((interrupt, noreturn)) Default_Handler (void)
{
    while (true)
        continue;
}
//! [Default_Handler]
