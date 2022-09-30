
#ifndef __JIBMGMT_REGFILE_H__
#define __JIBMGMT_REGFILE_H__

#include <stdint.h>

typedef struct jibmgmt_config_str {
	uint32_t debug[4]              ; // @0x00000000
	uint32_t cuart_cfg             ; // @0x00000010
	uint32_t cuart_stat            ; // @0x00000014
	uint32_t clksystick            ; // @0x00000018
	uint32_t gpio_ctrl_jtag        ; // @0x0000001c
	uint32_t gpio_cfg[12]          ; // @0x00000020
	uint32_t __unused_0[4]         ; // @0x00000050 - 0x0000005c
	uint32_t gpio_reg_oe           ; // @0x00000060
	uint32_t gpio_reg_do_masked    ; // @0x00000064
	uint32_t gpio_reg_do           ; // @0x00000068
	uint32_t gpio_reg_di           ; // @0x0000006c
	uint32_t arm_dbg_rst           ; // @0x00000070
	uint32_t ft_irq                ; // @0x00000074
	uint32_t chipid                ; // @0x00000078
	uint32_t clkrefslow            ; // @0x0000007c
	uint32_t spi_slv_en            ; // @0x00000080
	uint32_t spi_slv_clk_div       ; // @0x00000084
	uint32_t spi_config_in         ; // @0x00000088
	uint32_t spi_config_out        ; // @0x0000008c
	uint32_t dft_config            ; // @0x00000090
	uint32_t dft_status            ; // @0x00000094
	uint32_t dft_patt_addr         ; // @0x00000098
	uint32_t dft_patt_size         ; // @0x0000009c
	uint32_t abb_ctrl[3]           ; // @0x000000a0
	uint32_t abb_filter[2]         ; // @0x000000ac
	uint32_t abb_pwm               ; // @0x000000b4
	uint32_t abb_status[2]         ; // @0x000000b8
	uint32_t abb_clkconfig         ; // @0x000000c0
	uint32_t __unused_1[3]         ; // @0x000000c4 - 0x000000cc
	uint32_t ospi_core_en          ; // @0x000000d0
	uint32_t ospi_frontend         ; // @0x000000d4
	uint32_t ospi_tx_buffer        ; // @0x000000d8
	uint32_t ospi_rx_buffer        ; // @0x000000dc
} jibmgmt_config_str_t;

#endif /* __JIBMGMT_REGFILE_H__ */
