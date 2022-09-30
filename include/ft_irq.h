/*       _\|/_
         (o o)
 +----oOO-{_}-OOo-------------+
 |          _      ^..^       |
 |    _   _(_     ( oo )  )~  |
 |  _) /)(// (/     ,,  ,,    |
 |                2018-01-30  | 
 +---------------------------*/

#ifndef __FT_IRQ_H__
#define __FT_IRQ_H__

// s_jib1.icsng : 835
#define IRQ_FEEDTHROUGH_REG_0 0x0
#define IRQ_FEEDTHROUGH_REG_1 0x1
#define IRQ_FEEDTHROUGH_REG_2 0x2
#define IRQ_FEEDTHROUGH_REG_3 0x3
#define UART0_IRQ_RX_FULL_S   0x4
#define UART0_IRQ_RX_VALID_S  0x5
#define UART0_IRQ_TX_DONE_S   0x6
#define UART0_IRQ_TX_READY_S  0x7
#define UART1_IRQ_RX_FULL_S   0x8
#define UART1_IRQ_RX_VALID_S  0x9
#define UART1_IRQ_TX_DONE_S   0xA
#define UART1_IRQ_TX_READY_S  0xB
#define UART0_IRQ_RX_DELIM_S  0xC
#define UART1_IRQ_RX_DELIM_S  0xD
#define GPIO_IRQ_0            0xE
#define GPIO_IRQ_1            0xF


// jib1_mgmt_regfile.v : 636
// 4x config-values (upper 4 nibbles)
#define FT_IRQ_CONFIG(cfg, x) (((uint32_t)(x))<< (4*(cfg) + 16))
// explicit:
#define FT_IRQ_CONFIG0(x) FT_IRQ_CONFIG(0, x)
#define FT_IRQ_CONFIG1(x) FT_IRQ_CONFIG(1, x)
#define FT_IRQ_CONFIG2(x) FT_IRQ_CONFIG(2, x)
#define FT_IRQ_CONFIG3(x) FT_IRQ_CONFIG(3, x)

#endif

