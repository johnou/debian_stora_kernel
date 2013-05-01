/*
 * arch/arm/mach-kirkwood/netgear_ms2110-setup.c 
 *
 * Netgear MS2110 (Stora) Board Setup
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mtd/partitions.h>
#include <linux/ata_platform.h>
#include <linux/mv643xx_eth.h>
#include <linux/i2c.h>
#include <linux/i2c/at24.h>
#include <linux/input.h>
#include <linux/gpio.h>
#include <linux/gpio_keys.h>
#include <linux/leds.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <mach/kirkwood.h>
#include <plat/orion-gpio.h>
#include "common.h"
#include "mpp.h"

#define MACH_TYPE_NETGEAR_MS2110        2743

#ifdef CONFIG_MACH_NETGEAR_MS2110
# ifdef machine_arch_type
#  undef machine_arch_type
#  define machine_arch_type	__machine_arch_type
# else
#  define machine_arch_type	MACH_TYPE_NETGEAR_MS2110
# endif
# define machine_is_netgear_ms2110()	(machine_arch_type == MACH_TYPE_NETGEAR_MS2110)
#else
# define machine_is_netgear_ms2110()	(0)
#endif

static struct mtd_partition netgear_ms2110_nand_parts[] = {
	{
		.name = "u-boot",
		.offset = 0,
		.size = SZ_1M
	}, {
		.name = "uImage",
		.offset = MTDPART_OFS_NXTBLK,
		.size = SZ_4M + SZ_2M
	}, {
		.name = "root",
		.offset = MTDPART_OFS_NXTBLK,
		.size = MTDPART_SIZ_FULL
	},
};

static struct mv643xx_eth_platform_data netgear_ms2110_ge00_data = {
	.phy_addr	= MV643XX_ETH_PHY_ADDR(8),
};

static struct mv_sata_platform_data netgear_ms2110_sata_data = {
	.n_ports	= 2,
};

static struct i2c_board_info  __initdata netgear_ms2110_i2c_info[] = {
   { I2C_BOARD_INFO("pcf8563", 0x51 ) },
   { I2C_BOARD_INFO("lm75", 0x48 ) }
}; 

#define NETGEAR_MS2110_POWER_BUTTON 36
#define NETGEAR_MS2110_RESET_BUTTON 38

static struct gpio_keys_button netgear_ms2110_buttons[] = {
        [0] = {
                .code           = KEY_POWER,
                .gpio           = NETGEAR_MS2110_POWER_BUTTON,
                .desc           = "Power push button",
                .active_low     = 1,
        },
        [1] = {
                .code           = KEY_POWER2,
                .gpio           = NETGEAR_MS2110_RESET_BUTTON,
                .desc           = "Reset push button",
                .active_low     = 1,
        },
};

static struct gpio_keys_platform_data netgear_ms2110_button_data = {
        .buttons        = netgear_ms2110_buttons,
        .nbuttons       = ARRAY_SIZE(netgear_ms2110_buttons),
};

static struct platform_device netgear_ms2110_gpio_buttons = {
        .name           = "gpio-keys",
        .id             = -1,
        .dev            = {
                .platform_data  = &netgear_ms2110_button_data,
        },
};

static unsigned int netgear_ms2110_mpp_config[] __initdata = {
	MPP0_NF_IO2,  /* nand controller */
	MPP1_NF_IO3,  /* nand controller */
	MPP2_NF_IO4,  /* nand controller */
	MPP3_NF_IO5,  /* nand controller */
	MPP4_NF_IO6,  /* nand controller */
	MPP5_NF_IO7,  /* nand controller */
	MPP6_SYSRST_OUTn, /* system reset out */
	MPP7_SPI_SCn, 
	MPP8_TW0_SDA,  /* TWSI data */
	MPP9_TW0_SCK,  /* TWSI clock */
	MPP10_UART0_TXD, /* ttyS0 tx */
	MPP11_UART0_RXD, /* ttys0 rx */
	MPP12_SD_CLK,
	MPP13_SD_CMD,
	MPP14_SD_D0,
	MPP15_SD_D1,
	MPP16_SD_D2,
	MPP17_SD_D3,
	MPP18_NF_IO0,  /* nand controller */
	MPP19_NF_IO1,  /* nand controller */
	MPP20_SATA1_ACTn, /* green led for drive 2 */
	MPP21_SATA0_ACTn, /* green led for drive 1 */
	MPP22_GPIO,   /* red led for drive 2 */
	MPP23_GPIO,   /* red led for drive 1 */
	MPP24_GE1_RXD0,
	MPP25_GE1_RXD1,
	MPP26_GE1_RXD2,
	MPP27_GE1_RXD3,
	MPP28_GPIO,
	MPP29_GPIO,
	MPP30_GPIO, 
	MPP31_GPIO,  /* blue led for power indicator 1 */
	MPP32_GPIO,  /* blue led for power indicator 2 */
	MPP33_GE1_TXCTL,
	MPP34_SATA1_ACTn, /* positively retarded, unused, and nonstandard */
	MPP35_GPIO,
	MPP36_GPIO,  /* power button input */
	MPP37_GPIO,  /* reset button input */
	MPP38_GPIO,
	MPP39_GPIO,
	MPP40_GPIO,  /* low output powers off board */
	MPP41_GPIO,  /* input from fan tachometer -- kind of pointless */
	MPP42_GPIO,
	MPP43_GPIO,  /* Green 1000M phy led enable */
	MPP44_GPIO,  /* Yellow 10M/100M phy led enable */
	MPP45_TDM_PCLK, /* tp11 on the board */
	MPP46_TDM_FS,   /* tp12 on the board */
	MPP47_TDM_DRX,  /* tp13 on the board */
	MPP48_TDM_DTX,  /* tp14 on the board */
	MPP49_GPIO,     /* tp15 on the board */
	0
};

#define NETGEAR_MS2110_GPIO_POWER_OFF 40

static void netgear_ms2110_power_off(void)
{
        gpio_set_value(NETGEAR_MS2110_GPIO_POWER_OFF, 1);
}

#define NETGEAR_MS2110_GPIO_BLUE1_LED     31
#define NETGEAR_MS2110_GPIO_BLUE2_LED     32
#define NETGEAR_MS2110_GPIO_RED1_LED     23
#define NETGEAR_MS2110_GPIO_RED2_LED     22

static int netgear_ms2110_gpio_blink_set(unsigned gpio, int state, unsigned long *delay_on, unsigned long *delay_off) {
	if (*delay_on == 0 && *delay_off == 1) { 
		/* this special case turns on hardware blinking */
		orion_gpio_set_blink(gpio,1);
		return 0;
	}
	orion_gpio_set_blink(gpio,0); /* turn off hardware blinking */
	if (*delay_off == 0 && *delay_off == 0)
		return 0; /* we're done */
	else
		return 1; /* fall back to the software blinking */
}

static struct gpio_led netgear_ms2110_gpio_led_pins[] = {
        {
                .name   = "blue1",
                .gpio   = NETGEAR_MS2110_GPIO_BLUE1_LED,
                .active_low     = 1,
		.default_trigger = "heartbeat",
		.default_state = LEDS_GPIO_DEFSTATE_KEEP,
        },
        {
                .name   = "blue2",
                .gpio   = NETGEAR_MS2110_GPIO_BLUE2_LED,
                .active_low     = 1,
		.default_trigger = "default-on",
		.default_state = LEDS_GPIO_DEFSTATE_KEEP,
        },
        {
                .name   = "red1",
                .gpio   = NETGEAR_MS2110_GPIO_RED1_LED,
                .active_low     = 1,
		.default_trigger = "none",
		.default_state = LEDS_GPIO_DEFSTATE_KEEP,
        },
        {
                .name   = "red2",
                .gpio   = NETGEAR_MS2110_GPIO_RED2_LED,
                .active_low     = 1,
		.default_trigger = "none",
		.default_state = LEDS_GPIO_DEFSTATE_KEEP,
        },
};

static struct gpio_led_platform_data netgear_ms2110_gpio_leds_data = {
        .num_leds       = ARRAY_SIZE(netgear_ms2110_gpio_led_pins),
        .leds           = netgear_ms2110_gpio_led_pins,
	.gpio_blink_set = netgear_ms2110_gpio_blink_set,
};


static struct platform_device netgear_ms2110_gpio_leds = {
        .name           = "leds-gpio",
        .id             = -1,
        .dev            = {
                .platform_data  = &netgear_ms2110_gpio_leds_data,
        },
};

static void __init netgear_ms2110_init(void)
{
	/*
	 * Basic setup. Needs to be called early.
	 */
	kirkwood_init();
	kirkwood_mpp_conf(netgear_ms2110_mpp_config);

	kirkwood_uart0_init();
	kirkwood_nand_init(ARRAY_AND_SIZE(netgear_ms2110_nand_parts), 25);

	kirkwood_ehci_init();

	kirkwood_ge00_init(&netgear_ms2110_ge00_data);
	kirkwood_sata_init(&netgear_ms2110_sata_data);

	kirkwood_i2c_init();
	i2c_register_board_info(0, netgear_ms2110_i2c_info,
                                ARRAY_SIZE(netgear_ms2110_i2c_info));
        
	platform_device_register(&netgear_ms2110_gpio_buttons);

	if (gpio_request(NETGEAR_MS2110_GPIO_POWER_OFF, "power-off") == 0 &&
            gpio_direction_output(NETGEAR_MS2110_GPIO_POWER_OFF, 0) == 0)
                pm_power_off = netgear_ms2110_power_off;
        else
                pr_err("netgear_ms2110: failed to configure power-off GPIO\n");

	if (gpio_request(NETGEAR_MS2110_GPIO_BLUE1_LED,"power-light1") == 0 &&
	    gpio_direction_output(NETGEAR_MS2110_GPIO_BLUE1_LED, 0) == 0) {
	        gpio_set_value(NETGEAR_MS2110_GPIO_BLUE1_LED,0);
	        orion_gpio_set_blink(NETGEAR_MS2110_GPIO_BLUE1_LED,0);
            } else
	        pr_err("netgear_ms2110: failed to configure blue LED1\n");
	if (gpio_request(NETGEAR_MS2110_GPIO_BLUE2_LED,"power-light2") == 0 &&
	    gpio_direction_output(NETGEAR_MS2110_GPIO_BLUE2_LED, 0) == 0) {
	        gpio_set_value(NETGEAR_MS2110_GPIO_BLUE2_LED,0);
	        orion_gpio_set_blink(NETGEAR_MS2110_GPIO_BLUE2_LED,0);
            } else
	        pr_err("netgear_ms2110: failed to configure blue LED2\n");
	gpio_free(NETGEAR_MS2110_GPIO_BLUE1_LED);
	gpio_free(NETGEAR_MS2110_GPIO_BLUE2_LED);
        platform_device_register(&netgear_ms2110_gpio_leds);

}

static int __init netgear_ms2110_pci_init(void)
{
	if (machine_is_netgear_ms2110())
	    kirkwood_pcie_init(KW_PCIE0);

	return 0;
 }
subsys_initcall(netgear_ms2110_pci_init);


MACHINE_START(NETGEAR_MS2110, "Netgear MS2110")
        .atag_offset	= 0x100,
	.init_machine	= netgear_ms2110_init,
	.map_io		= kirkwood_map_io,
	.init_early	= kirkwood_init_early,
	.init_irq	= kirkwood_init_irq,
	.timer		= &kirkwood_timer,
	.restart	= kirkwood_restart,
MACHINE_END
