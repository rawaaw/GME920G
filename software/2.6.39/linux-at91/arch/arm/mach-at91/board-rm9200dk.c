/*
 * linux/arch/arm/mach-at91/board-rm9200dk.c
 *
 *  Copyright (C) 2005 SAN People
 *
 *  Epson S1D framebuffer glue code is:
 *     Copyright (C) 2005 Thibaut VARENE <varenet@parisc-linux.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/types.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/mtd/nand.h>
#include <linux/platform_device.h>
#include <linux/spi/flash.h>
#include <linux/spi/spi.h>
#include <linux/mtd/physmap.h>

#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/irq.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <mach/hardware.h>
#include <mach/board.h>
#include <mach/gpio.h>
#include <mach/at91rm9200_mc.h>

#include "generic.h"


static void __init dk_map_io(void)
{
	/* Initialize processor: 18.432 MHz crystal */
	at91rm9200_initialize(18432000, AT91RM9200_BGA);

	/* Setup the LEDs */
	/* CPU led   - X401_8 PA24/SCK2/PCK1 PA24 (CONTR) ???
	   Timer led - X402_2 PB28 - (reset of HC4520M. see reset_schematics.jpg)
	 */
        /* now not used. Done with hearbeat led. See definition dk_leds[] */
	at91_init_leds(AT91_PIN_PA24, AT91_PIN_PB28);

	/* DBGU on ttyS0. (Rx & Tx only) */
	at91_register_uart(0, 0, 0);

	/* USART1 on ttyS1. (Rx, Tx, CTS, RTS, DTR, DSR, DCD, RI) */
	at91_register_uart(AT91RM9200_ID_US1, 1, ATMEL_UART_CTS | ATMEL_UART_RTS
			   | ATMEL_UART_DTR | ATMEL_UART_DSR | ATMEL_UART_DCD
			   | ATMEL_UART_RI);
  
	/* USART2 on ttyS3.               CAN */
	at91_register_uart(AT91RM9200_ID_US2, 3, 0);

	/* USART3 on ttyS2. (Rx, Tx, RTS) TxD2/RxD2 */
	at91_register_uart(AT91RM9200_ID_US3, 2, ATMEL_UART_RTS);

	/* USART0 on ttyS4. (Rx, Tx, RTS) TxD1/RxD1 */
	at91_register_uart(AT91RM9200_ID_US0, 4, ATMEL_UART_RTS);

	/* set serial console to ttyS0 (ie, DBGU) */
	at91_set_serial_console(0);
}

static void __init dk_init_irq(void)
{
	at91rm9200_init_interrupts(NULL);
}

static struct at91_eth_data __initdata dk_eth_data = {
	.phy_irq_pin	= AT91_PIN_PC4,
	.is_rmii	= 0,
};

static struct at91_usbh_data __initdata dk_usbh_data = {
	.ports		= 1,
};

static struct at91_udc_data __initdata dk_udc_data = {
	.vbus_pin	= AT91_PIN_PD4,
	.pullup_pin	= AT91_PIN_PD5,
};


static struct mtd_partition m25_spi_flash_parts[] = {
	{
		.name = "full",
		.offset = 0,
		.size = MTDPART_SIZ_FULL,
	},
#if 0
	{
		.name = "little",
		.offset = 0,
		.size = 24 * SZ_1K,
	},
	{
		.name = "remaining",
		.offset = MTDPART_OFS_NXTBLK,
		.size = MTDPART_SIZ_FULL,
	},
#endif
};

static struct flash_platform_data m25p64 = {
		.type		= "m25p64",
		.name           = "spi_flash",
		.parts		= m25_spi_flash_parts,
		.nr_parts	= ARRAY_SIZE(m25_spi_flash_parts),
  
};


static struct spi_board_info dk_spi_devices[] = {
	{	/* serial flash chip */
		.modalias	= "m25p80",
		.chip_select	= 0,
		.max_speed_hz	= 15 * 1000 * 1000,
		.bus_num	= 0,
		.mode		= SPI_MODE_0,
		.platform_data  = &m25p64,
		.irq            = -1,
	},
	{	/* serial flash chip */
		.modalias	= "m25p80",
		.chip_select	= 1,
		.max_speed_hz	= 15 * 1000 * 1000,
		.bus_num	= 0,
		.mode		= SPI_MODE_0,
		.platform_data  = &m25p64,
		.irq            = -1,
	},
};

static struct i2c_board_info __initdata dk_i2c_devices[] = {
	{
		I2C_BOARD_INFO("pca9553", 0x62),
	},
	{
		I2C_BOARD_INFO("m41t00", 0x68),
	},
};

#define DK_FLASH_BASE	AT91_CHIPSELECT_0
#define DK_FLASH_SIZE	SZ_8M

static struct physmap_flash_data dk_flash_data = {
	.width		= 2,
};

static struct resource dk_flash_resource = {
	.start		= DK_FLASH_BASE,
	.end		= DK_FLASH_BASE + DK_FLASH_SIZE - 1,
	.flags		= IORESOURCE_MEM,
};

static struct platform_device dk_flash = {
	.name		= "physmap-flash",
	.id		= 0,
	.dev		= {
				.platform_data	= &dk_flash_data,
			},
	.resource	= &dk_flash_resource,
	.num_resources	= 1,
};

static struct gpio_led dk_leds[] = {
	{
		.name			= "red",
		.gpio			= AT91_PIN_PA27,
		.active_low		= 0,
		.default_trigger	= "timer",
	},
	{
		.name			= "yel",
		.gpio			= AT91_PIN_PA28,
		.active_low		= 0,
		.default_trigger	= "heartbeat",
	},
	{
		.name			= "watchdog",
		.gpio			= AT91_PIN_PB28, // PB28 initializes reset schematic (reset_schematics.jpg)
		.active_low		= 0,
		.default_trigger	= "heartbeat",
	},
	{
		.name			= "mdmrst",
		.gpio			= AT91_PIN_PB29, // PB29 modem reset
		.active_low		= 0,
		.default_trigger	= "none",
	}
};

static void __init dk_board_init(void)
{
	/* Serial */
	at91_add_device_serial();
	/* Ethernet */
	at91_add_device_eth(&dk_eth_data);
	/* USB Host */
	at91_add_device_usbh(&dk_usbh_data);
	/* USB Device */
	at91_add_device_udc(&dk_udc_data);
	at91_set_multi_drive(dk_udc_data.pullup_pin, 1);	/* pullup_pin is connected to reset */
	/* I2C */
	at91_add_device_i2c(dk_i2c_devices, ARRAY_SIZE(dk_i2c_devices));
	/* SPI */
	at91_add_device_spi(dk_spi_devices, ARRAY_SIZE(dk_spi_devices));
	/* NOR Flash */
	platform_device_register(&dk_flash);
	/* LEDs */
	at91_gpio_leds(dk_leds, ARRAY_SIZE(dk_leds));

}
MACHINE_START(AT91RM9200, "GME920G")
	/* Maintainer: SAN People/Atmel */
	.boot_params	= AT91_SDRAM_BASE + 0x100,
	.timer		= &at91rm9200_timer,
	.map_io		= dk_map_io,
	.init_irq	= dk_init_irq,
	.init_machine	= dk_board_init,
MACHINE_END
