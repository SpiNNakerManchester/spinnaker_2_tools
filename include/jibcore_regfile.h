
#ifndef __JIBCORE_REGFILE_H__
#define __JIBCORE_REGFILE_H__

#include <stdint.h>

typedef struct jibcore_config_str {
	uint32_t debug[4]            ; // @0x00000000
	uint32_t fbist_config        ; // @0x00000010
	uint32_t fbist_status        ; // @0x00000014
	uint32_t fbist_srcsel        ; // @0x00000018
	uint32_t __unused_0[1]       ; // @0x0000001c - 0x0000001c
	uint32_t pll0_cfg[2]         ; // @0x00000020
	uint32_t pll0_status         ; // @0x00000028
	uint32_t pll0_bisc_config    ; // @0x0000002c
	uint32_t pll0_bisc_status    ; // @0x00000030
	uint32_t __unused_1[3]       ; // @0x00000034 - 0x0000003c
	uint32_t pll1_cfg[2]         ; // @0x00000040
	uint32_t pll1_status         ; // @0x00000048
	uint32_t __unused_2[1]       ; // @0x0000004c - 0x0000004c
	uint32_t trng_cfg            ; // @0x00000050
	uint32_t trng_run_period     ; // @0x00000054
	uint32_t trng_firo_poly      ; // @0x00000058
	uint32_t trng_garo_poly      ; // @0x0000005c
	uint32_t trng_enable         ; // @0x00000060
	uint32_t trng_value          ; // @0x00000064
	uint32_t __unused_3[2]       ; // @0x00000068 - 0x0000006c
	uint32_t rterm_clk           ; // @0x00000070
	uint32_t rterm_cfg           ; // @0x00000074
	uint32_t rterm_status        ; // @0x00000078
	uint32_t __unused_4[1]       ; // @0x0000007c - 0x0000007c
	uint32_t lock[16]            ; // @0x00000080
	uint32_t pwm_cfg             ; // @0x000000c0
	uint32_t pwm_val[2]          ; // @0x000000c4
} jibcore_config_str_t;

#endif /* __JIBCORE_REGFILE_H__ */
