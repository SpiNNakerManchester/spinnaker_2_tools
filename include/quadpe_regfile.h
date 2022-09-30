
#ifndef __QUADPE_REGFILE_H__
#define __QUADPE_REGFILE_H__

#include <stdint.h>

typedef struct quadpe_config_str {
	uint32_t debug[4]                       ; // @0x00000000
	uint32_t dft_ctrl                       ; // @0x00000010
	uint32_t dft_status                     ; // @0x00000014
	uint32_t __unused_0[2]                  ; // @0x00000018 - 0x0000001c
	uint32_t dft_stream[4]                  ; // @0x00000020
	uint32_t __unused_1[4]                  ; // @0x00000030 - 0x0000003c
	uint32_t irq_ft[4]                      ; // @0x00000040
	uint32_t __unused_2[4]                  ; // @0x00000050 - 0x0000005c
	uint32_t irq_rf[4]                      ; // @0x00000060
	uint32_t __unused_3[20]                 ; // @0x00000070 - 0x000000bc
	uint32_t cmgmt_src                      ; // @0x000000c0
	uint32_t cmgmt_div                      ; // @0x000000c4
	uint32_t cmgmt_dnoc                     ; // @0x000000c8
	uint32_t cmgmt_share                    ; // @0x000000cc
	uint32_t __unused_4[12]                 ; // @0x000000d0 - 0x000000fc
	uint32_t pmgt_param_pre_switch_count    ; // @0x00000100
	uint32_t pmgt_param_t_fdown2clkoff      ; // @0x00000104
	uint32_t pmgt_param_t_fdown2clkon       ; // @0x00000108
	uint32_t pmgt_param_t_fdown2freq        ; // @0x0000010c
	uint32_t pmgt_param_t_fup2clkoff        ; // @0x00000110
	uint32_t pmgt_param_t_fup2clkon         ; // @0x00000114
	uint32_t pmgt_param_t_fup2freq          ; // @0x00000118
	uint32_t pmgt_param_t_off2clk           ; // @0x0000011c
	uint32_t pmgt_param_t_off2iso           ; // @0x00000120
	uint32_t pmgt_param_t_off2reset         ; // @0x00000124
	uint32_t pmgt_param_t_off2vdd           ; // @0x00000128
	uint32_t pmgt_param_t_on2clk            ; // @0x0000012c
	uint32_t pmgt_param_t_on2iso            ; // @0x00000130
	uint32_t pmgt_param_t_on2reset          ; // @0x00000134
	uint32_t pmgt_param_t_on2vdd            ; // @0x00000138
	uint32_t pmgt_param_t_precharge_pu      ; // @0x0000013c
	uint32_t pmgt_param_t_precharge_sc      ; // @0x00000140
	uint32_t pmgt_param_t_vdown2clkoff      ; // @0x00000144
	uint32_t pmgt_param_t_vdown2clkon       ; // @0x00000148
	uint32_t pmgt_param_t_vdown2vdd         ; // @0x0000014c
	uint32_t pmgt_param_t_vfdown2clkoff     ; // @0x00000150
	uint32_t pmgt_param_t_vfdown2clkon      ; // @0x00000154
	uint32_t pmgt_param_t_vfdown2freq       ; // @0x00000158
	uint32_t pmgt_param_t_vfdown2vdd        ; // @0x0000015c
	uint32_t pmgt_param_t_vfup2clkoff       ; // @0x00000160
	uint32_t pmgt_param_t_vfup2clkon        ; // @0x00000164
	uint32_t pmgt_param_t_vfup2freq         ; // @0x00000168
	uint32_t pmgt_param_t_vfup2vdd          ; // @0x0000016c
	uint32_t pmgt_param_t_vup2clkoff        ; // @0x00000170
	uint32_t pmgt_param_t_vup2clkon         ; // @0x00000174
	uint32_t pmgt_param_t_vup2vdd           ; // @0x00000178
	uint32_t __unused_5[1]                  ; // @0x0000017c - 0x0000017c
	uint32_t pmgt_command_core0             ; // @0x00000180
	uint32_t pmgt_ctrl_core0                ; // @0x00000184
	uint32_t pmgt_command_core1             ; // @0x00000188
	uint32_t pmgt_ctrl_core1                ; // @0x0000018c
	uint32_t pmgt_command_core2             ; // @0x00000190
	uint32_t pmgt_ctrl_core2                ; // @0x00000194
	uint32_t pmgt_command_core3             ; // @0x00000198
	uint32_t pmgt_ctrl_core3                ; // @0x0000019c
	uint32_t pmgt_command_core01            ; // @0x000001a0
	uint32_t pmgt_ctrl_core01               ; // @0x000001a4
	uint32_t pmgt_command_core23            ; // @0x000001a8
	uint32_t pmgt_ctrl_core23               ; // @0x000001ac
	uint32_t pmgt_command_core02            ; // @0x000001b0
	uint32_t pmgt_ctrl_core02               ; // @0x000001b4
	uint32_t pmgt_command_core13            ; // @0x000001b8
	uint32_t pmgt_ctrl_core13               ; // @0x000001bc
	uint32_t pmgt_command_core_all          ; // @0x000001c0
	uint32_t pmgt_ctrl_core_all             ; // @0x000001c4
	uint32_t __unused_6[2]                  ; // @0x000001c8 - 0x000001cc
	uint32_t pmgt_status_core[4]            ; // @0x000001d0
	uint32_t __unused_7[1]                  ; // @0x000001e0 - 0x000001e0
	uint32_t pmgt_noc_mcread                ; // @0x000001e4
	uint32_t __unused_8[6]                  ; // @0x000001e8 - 0x000001fc
	uint32_t arm_ctrl[4]                    ; // @0x00000200
	uint32_t arm_ctrl_all                   ; // @0x00000210
	uint32_t __unused_9[1]                  ; // @0x00000214 - 0x00000214
	uint32_t arm_jtag                       ; // @0x00000218
	uint32_t __unused_10[1]                 ; // @0x0000021c - 0x0000021c
	uint32_t share_config                   ; // @0x00000220
	uint32_t __unused_11[7]                 ; // @0x00000224 - 0x0000023c
	uint32_t mbist_cmd_seq[2]               ; // @0x00000240
	uint32_t mbist_ctrl                     ; // @0x00000248
	uint32_t mbist_status                   ; // @0x0000024c
	uint32_t mbist_addr[2]                  ; // @0x00000250
	uint32_t __unused_12[2]                 ; // @0x00000258 - 0x0000025c
	uint32_t sram_delay_vhigh               ; // @0x00000260
	uint32_t sram_delay_vlow                ; // @0x00000264
	uint32_t sram_retention                 ; // @0x00000268
	uint32_t __unused_13[1]                 ; // @0x0000026c - 0x0000026c
	uint32_t sram_redsel_0_01               ; // @0x00000270
	uint32_t sram_redsel_0_23               ; // @0x00000274
	uint32_t sram_redsel_0_45               ; // @0x00000278
	uint32_t sram_redsel_0_67               ; // @0x0000027c
	uint32_t sram_redsel_1_01               ; // @0x00000280
	uint32_t sram_redsel_1_23               ; // @0x00000284
	uint32_t sram_redsel_1_45               ; // @0x00000288
	uint32_t sram_redsel_1_67               ; // @0x0000028c
	uint32_t sram_redsel_2_01               ; // @0x00000290
	uint32_t sram_redsel_2_23               ; // @0x00000294
	uint32_t sram_redsel_2_45               ; // @0x00000298
	uint32_t sram_redsel_2_67               ; // @0x0000029c
	uint32_t sram_redsel_3_01               ; // @0x000002a0
	uint32_t sram_redsel_3_23               ; // @0x000002a4
	uint32_t sram_redsel_3_45               ; // @0x000002a8
	uint32_t sram_redsel_3_67               ; // @0x000002ac
	uint32_t __unused_14[20]                ; // @0x000002b0 - 0x000002fc
	uint32_t dnoc_router_en                 ; // @0x00000300
	uint32_t dnoc_port_en                   ; // @0x00000304
	uint32_t dnoc_port_test[2]              ; // @0x00000308
	uint32_t dnoc_decode_mode               ; // @0x00000310
	uint32_t dnoc_route_lut[4]              ; // @0x00000314
	uint32_t __unused_15[4]                 ; // @0x00000324 - 0x00000330
	uint32_t dnoc_sel_fifo_en               ; // @0x00000334
	uint32_t dnoc_stat_en                   ; // @0x00000338
	uint32_t dnoc_stat_mod_ctrl             ; // @0x0000033c
	uint32_t dnoc_stat_top_ctrl             ; // @0x00000340
	uint32_t dnoc_stat_top_addr             ; // @0x00000344
	uint32_t __pad[130862];
} quadpe_config_str_t;

#endif /* __QUADPE_REGFILE_H__ */
