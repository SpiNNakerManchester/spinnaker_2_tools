/*       _\|/_
         (o o)
 +----oOO-{_}-OOo-------------+
 |          _      ^..^       |
 |    _   _(_     ( oo )  )~  |
 |  _) /)(// (/     ,,  ,,    |
 |                2018-01-30  |
 +---------------------------*/

#ifndef __GPIO_CFG_H__
#define __GPIO_CFG_H__

#ifdef __cplusplus
extern "C" {
#endif


enum gpio_config {
    GPIO_CFG_INACTIVE        = 0x00,

    GPIO_CFG_CUART_TX        = 0x01,
    GPIO_CFG_CUART_RX        = 0x02,

    GPIO_CFG_UART0_TX        = 0x03,
    GPIO_CFG_UART0_RX        = 0x04,
    GPIO_CFG_UART0_CTS       = 0x05,
    GPIO_CFG_UART0_RTR       = 0x06,

    GPIO_CFG_UART1_TX        = 0x07,
    GPIO_CFG_UART1_RX        = 0x08,
    GPIO_CFG_UART1_CTS       = 0x09,
    GPIO_CFG_UART1_RTR       = 0x0a,

    GPIO_CFG_SPI_INTR        = 0x0b,
    GPIO_CFG_SPI_MISO        = 0x0c,
    GPIO_CFG_SPI_MOSI        = 0x0d,
    GPIO_CFG_SPI_NSS         = 0x0e,
    GPIO_CFG_SPI_SCLK        = 0x0f,

    GPIO_CFG_GPIOREG_0       = 0x10,
    GPIO_CFG_GPIOREG_1       = 0x11,
    GPIO_CFG_GPIOREG_2       = 0x12,
    GPIO_CFG_GPIOREG_3       = 0x13,
    GPIO_CFG_GPIOREG_4       = 0x14,
    GPIO_CFG_GPIOREG_5       = 0x15,
    GPIO_CFG_GPIOREG_6       = 0x16,
    GPIO_CFG_GPIOREG_7       = 0x17,
    GPIO_CFG_GPIOREG_8       = 0x18,
    GPIO_CFG_GPIOREG_9       = 0x19,
    GPIO_CFG_GPIOREG_A       = 0x1a,
    GPIO_CFG_GPIOREG_B       = 0x1b,
    GPIO_CFG_GPIOREG_C       = 0x1c,
    GPIO_CFG_GPIOREG_D       = 0x1d,
    GPIO_CFG_GPIOREG_E       = 0x1e,
    GPIO_CFG_GPIOREG_F       = 0x1f,

    GPIO_CFG_ARM_JTAG1_TCK   = 0x20,
    GPIO_CFG_ARM_JTAG1_TMS   = 0x21,
    GPIO_CFG_ARM_JTAG1_TRSTN = 0x22,
    GPIO_CFG_ARM_JTAG1_TDI   = 0x23,
    GPIO_CFG_ARM_JTAG1_TDO   = 0x24,

    GPIO_CFG_ARM_JTAG2_TCK   = 0x25,
    GPIO_CFG_ARM_JTAG2_TMS   = 0x26,
    GPIO_CFG_ARM_JTAG2_TRSTN = 0x27,
    GPIO_CFG_ARM_JTAG2_TDI   = 0x28,
    GPIO_CFG_ARM_JTAG2_TDO   = 0x29,

    GPIO_CFG_ARM_DBG_RST     = 0x2a,

    GPIO_CFG_FBIST_MEAS      = 0x2b,

    GPIO_CFG_PLL_C0_PFD      = 0x2c,
    GPIO_CFG_PLL_C2C_0_PFD   = 0x2d,
    GPIO_CFG_PLL_C2C_1_PFD   = 0x2e,

    GPIO_CFG_PWM0            = 0x2f,
    GPIO_CFG_PWM1            = 0x30,

    GPIO_CFG_OSPI_CLK        = 0x31,
    GPIO_CFG_OSPI_NSS0       = 0x32,
    GPIO_CFG_OSPI_NSS1       = 0x33,
    GPIO_CFG_OSPI_NSS2       = 0x34,
    GPIO_CFG_OSPI_NSS3       = 0x35,

    GPIO_CFG_OSPI_IO0        = 0x36,
    GPIO_CFG_OSPI_IO1        = 0x37,
    GPIO_CFG_OSPI_IO2        = 0x38,
    GPIO_CFG_OSPI_IO3        = 0x39,
    GPIO_CFG_OSPI_IO4        = 0x3a,
    GPIO_CFG_OSPI_IO5        = 0x3b,
    GPIO_CFG_OSPI_IO6        = 0x3c,
    GPIO_CFG_OSPI_IO7        = 0x3d,

    GPIO_CFG_ARM_FT_IRQ_0    = 0x3e,
    GPIO_CFG_ARM_FT_IRQ_1    = 0x3f,

    GPIO_CFG_TRNG_OSC_MEAS_0 = 0x40,
    GPIO_CFG_TRNG_OSC_MEAS_1 = 0x41,

    GPIO_CFG_LVDS_EIDLE      = 0x42,
};

#if 0
#define GPIO_CFG_INACTIVE        0x00

#define GPIO_CFG_CUART_TX        0x01
#define GPIO_CFG_CUART_RX        0x02
#define GPIO_CFG_UART0_TX        0x03
#define GPIO_CFG_UART0_RX        0x04
#define GPIO_CFG_UART0_CTS       0x05
#define GPIO_CFG_UART0_RTR       0x06
#define GPIO_CFG_UART1_TX        0x07
#define GPIO_CFG_UART1_RX        0x08
#define GPIO_CFG_UART1_CTS       0x09
#define GPIO_CFG_UART1_RTR       0x0a
#define GPIO_CFG_SPI_INTR        0x0b
#define GPIO_CFG_SPI_MISO        0x0c
#define GPIO_CFG_SPI_MOSI        0x0d
#define GPIO_CFG_SPI_NSS         0x0e
#define GPIO_CFG_SPI_SCLK        0x0f
#define GPIO_CFG_GPIOREG_0       0x10
#define GPIO_CFG_GPIOREG_1       0x11
#define GPIO_CFG_GPIOREG_2       0x12
#define GPIO_CFG_GPIOREG_3       0x13
#define GPIO_CFG_GPIOREG_4       0x14
#define GPIO_CFG_GPIOREG_5       0x15
#define GPIO_CFG_GPIOREG_6       0x16
#define GPIO_CFG_GPIOREG_7       0x17
#define GPIO_CFG_GPIOREG_8       0x18
#define GPIO_CFG_GPIOREG_9       0x19
#define GPIO_CFG_GPIOREG_A       0x1a
#define GPIO_CFG_GPIOREG_B       0x1b
#define GPIO_CFG_GPIOREG_C       0x1c
#define GPIO_CFG_GPIOREG_D       0x1d
#define GPIO_CFG_GPIOREG_E       0x1e
#define GPIO_CFG_GPIOREG_F       0x1f
#define GPIO_CFG_ARM_JTAG1_TCK   0x20
#define GPIO_CFG_ARM_JTAG1_TMS   0x21
#define GPIO_CFG_ARM_JTAG1_TRSTN 0x22
#define GPIO_CFG_ARM_JTAG1_TDI   0x23
#define GPIO_CFG_ARM_JTAG1_TDO   0x24
#define GPIO_CFG_ARM_JTAG2_TCK   0x25
#define GPIO_CFG_ARM_JTAG2_TMS   0x26
#define GPIO_CFG_ARM_JTAG2_TRSTN 0x27
#define GPIO_CFG_ARM_JTAG2_TDI   0x28
#define GPIO_CFG_ARM_JTAG2_TDO   0x29
#define GPIO_CFG_ARM_DBG_RST     0x2a
#define GPIO_CFG_FBIST_MEAS      0x2b

#define GPIO_PLL_C0_PFD          0x2c
#define GPIO_PLL_C2C_0_PFD       0x2d
#define GPIO_PLL_C2C_1_PFD       0x2e
#endif

#ifdef __cplusplus
}
#endif

#endif

