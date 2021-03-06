/* Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/init.h>
#include <linux/ioport.h>
#include <mach/board.h>
#include <mach/gpio.h>
#include <mach/gpiomux.h>
#include <mach/socinfo.h>

#define KS8851_IRQ_GPIO 115

#if defined(CONFIG_KS8851) || defined(CONFIG_KS8851_MODULE)
static struct gpiomux_setting gpio_eth_config = {
	.pull = GPIOMUX_PULL_UP,
	.drv = GPIOMUX_DRV_2MA,
	.func = GPIOMUX_FUNC_GPIO,
};

static struct msm_gpiomux_config msm_eth_configs[] = {
	{
		.gpio = KS8851_IRQ_GPIO,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_eth_config,
		}
	},
};
#endif

static struct gpiomux_setting synaptics_int_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting synaptics_int_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting synaptics_reset_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting synaptics_reset_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting gpio_keys_active = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting gpio_keys_suspend = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gpio_spi_config = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gpio_spi_cs_config = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting wcnss_5wire_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting wcnss_5wire_active_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv  = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting gpio_i2c_config = {
	.func = GPIOMUX_FUNC_3,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct msm_gpiomux_config msm_keypad_configs[] __initdata = {
	{
		.gpio = 106,
		.settings = {
			[GPIOMUX_ACTIVE]    = &gpio_keys_active,
			[GPIOMUX_SUSPENDED] = &gpio_keys_suspend,
		},
	},
	{
		.gpio = 107,
		.settings = {
			[GPIOMUX_ACTIVE]    = &gpio_keys_active,
			[GPIOMUX_SUSPENDED] = &gpio_keys_suspend,
		},
	},
	{
		.gpio = 108,
		.settings = {
			[GPIOMUX_ACTIVE]    = &gpio_keys_active,
			[GPIOMUX_SUSPENDED] = &gpio_keys_suspend,
		},
	},
/* [PLATFORM]-Modified-BEGIN by TCTNB.WJ, 2013/11/27, FR-534823, dev for Haptic */
#if defined(CONFIG_TCT_8X26_RIO6)||defined(CONFIG_TCT_8X26_RIO6_TF)
    {
		.gpio = 27,
		.settings = {
			[GPIOMUX_ACTIVE]    = &gpio_keys_active,
			[GPIOMUX_SUSPENDED] = &gpio_keys_suspend,
		},
	},
#endif
/* [PLATFORM]-Modified-END by TCTNB.WJ */
};

static struct gpiomux_setting lcd_rst_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting lcd_rst_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct msm_gpiomux_config msm_lcd_configs[] __initdata = {
	{
		.gpio = 25,
		.settings = {
			[GPIOMUX_ACTIVE]    = &lcd_rst_act_cfg,
			[GPIOMUX_SUSPENDED] = &lcd_rst_sus_cfg,
		},
	}
};

static struct msm_gpiomux_config msm_blsp_configs[] __initdata = {
	{
		.gpio      = 0,		/* BLSP1 QUP1 SPI_DATA_MOSI */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_spi_config,
		},
	},
	{
		.gpio      = 1,		/* BLSP1 QUP1 SPI_DATA_MISO */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_spi_config,
		},
	},
	{
		.gpio      = 2,		/* BLSP1 QUP1 SPI_CS1 */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_spi_cs_config,
		},
	},
	{
		.gpio      = 3,		/* BLSP1 QUP1 SPI_CLK */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_spi_config,
		},
	},
	{
		.gpio      = 14,	/* BLSP1 QUP4 I2C_SDA */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_i2c_config,
		},
	},
	{
		.gpio      = 15,	/* BLSP1 QUP4 I2C_SCL */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_i2c_config,
		},
	},
	{
		.gpio      = 18,		/* BLSP1 QUP5 I2C_SDA */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_i2c_config,
		},
	},
	{
		.gpio      = 19,		/* BLSP1 QUP5 I2C_SCL */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_i2c_config,
		},
	},
	{					/*  NFC   */
		.gpio      = 10,		/* BLSP1 QUP3 I2C_DAT */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_i2c_config,
		},
	},
	{					/*  NFC   */
		.gpio      = 11,		/* BLSP1 QUP3 I2C_CLK */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_i2c_config,
		},
	},
};

static struct msm_gpiomux_config msm_synaptics_configs[] __initdata = {
	{
		.gpio = 16,
		.settings = {
			[GPIOMUX_ACTIVE] = &synaptics_reset_act_cfg,
			[GPIOMUX_SUSPENDED] = &synaptics_reset_sus_cfg,
		},
	},
	{
		.gpio = 17,
		.settings = {
			[GPIOMUX_ACTIVE] = &synaptics_int_act_cfg,
			[GPIOMUX_SUSPENDED] = &synaptics_int_sus_cfg,
		},
	},
};

static struct gpiomux_setting gpio_nc_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting goodix_ldo_en_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting goodix_ldo_en_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting goodix_int_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting goodix_int_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting goodix_reset_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting goodix_reset_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct msm_gpiomux_config msm_qrd_blsp_configs[] __initdata = {
	{
		.gpio      = 2,		/* NC */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_nc_cfg,
		},
	},
	{
		.gpio      = 3,		/* NC */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_nc_cfg,
		},
	},
	{
		.gpio      = 4,		/* NC */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_nc_cfg,
		},
	},
	{
		.gpio      = 14,	/* NC */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_nc_cfg,
		},
	},
};

static struct msm_gpiomux_config msm_qrd_goodix_configs[] __initdata = {
	{
		.gpio = 15,		/* LDO EN */
		.settings = {
			[GPIOMUX_ACTIVE] = &goodix_ldo_en_act_cfg,
			[GPIOMUX_SUSPENDED] = &goodix_ldo_en_sus_cfg,
		},
	},
	{
		.gpio = 16,		/* RESET */
		.settings = {
			[GPIOMUX_ACTIVE] = &goodix_reset_act_cfg,
			[GPIOMUX_SUSPENDED] = &goodix_reset_sus_cfg,
		},
	},
	{
		.gpio = 17,		/* INT */
		.settings = {
			[GPIOMUX_ACTIVE] = &goodix_int_act_cfg,
			[GPIOMUX_SUSPENDED] = &goodix_int_sus_cfg,
		},
	},
	{
		.gpio      = 18,		/* BLSP1 QUP5 I2C_SDA */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_i2c_config,
		},
	},
	{
		.gpio      = 19,		/* BLSP1 QUP5 I2C_SCL */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_i2c_config,
		},
	},
};

static struct gpiomux_setting nfc_ldo_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting nfc_ldo_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting nfc_regc_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting nfc_regc_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting nfc_wake_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting nfc_wake_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct msm_gpiomux_config msm_qrd_nfc_configs[] __initdata = {
	{					/*  NFC  LDO EN */
		.gpio      = 0,
		.settings = {
			[GPIOMUX_ACTIVE] = &nfc_ldo_act_cfg,
			[GPIOMUX_SUSPENDED] = &nfc_ldo_sus_cfg,
		},
	},
	{					/*  NFC  REGC*/
		.gpio      = 1,
		.settings = {
			[GPIOMUX_ACTIVE] = &nfc_regc_act_cfg,
			[GPIOMUX_SUSPENDED] = &nfc_regc_sus_cfg,
		},
	},
	{					/*  NFC   WAKE */
		.gpio      = 5,
		.settings = {
			[GPIOMUX_ACTIVE] = &nfc_wake_act_cfg,
			[GPIOMUX_SUSPENDED] = &nfc_wake_sus_cfg,
		},
	},
	{					/*  NFC   */
		.gpio      = 10,		/* BLSP1 QUP3 I2C_DAT */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_i2c_config,
		},
	},
	{					/*  NFC   */
		.gpio      = 11,		/* BLSP1 QUP3 I2C_CLK */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_i2c_config,
		},
	},
};

static struct gpiomux_setting sd_card_det_active_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting sd_card_det_sleep_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_IN,
};

static struct msm_gpiomux_config sd_card_det __initdata = {
	.gpio = 38,
	.settings = {
		[GPIOMUX_ACTIVE]    = &sd_card_det_active_config,
		[GPIOMUX_SUSPENDED] = &sd_card_det_sleep_config,
	},
};

static struct msm_gpiomux_config wcnss_5wire_interface[] = {
	{
		.gpio = 40,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 41,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 42,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 43,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 44,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
};

static struct gpiomux_setting gpio_suspend_config[] = {
	{
		.func = GPIOMUX_FUNC_GPIO,  /* IN-NP */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},
	{
		.func = GPIOMUX_FUNC_GPIO,  /* O-LOW */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
		.dir = GPIOMUX_OUT_LOW,
	},
};

static struct gpiomux_setting cam_settings[] = {
	{
		.func = GPIOMUX_FUNC_1, /*active 1*/ /* 0 */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},

	{
		.func = GPIOMUX_FUNC_1, /*suspend*/ /* 1 */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},

	{
		.func = GPIOMUX_FUNC_1, /*i2c suspend*/ /* 2 */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_KEEPER,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*active 0*/ /* 3 */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*suspend 0*/ /* 4 */
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},
};


static struct msm_gpiomux_config msm_sensor_configs[] __initdata = {
	{
		.gpio = 26, /* CAM_MCLK0 */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[0],
			[GPIOMUX_SUSPENDED] = &cam_settings[1],
		},
	},
/* [PLATFORM]-Modified-BEGIN by TCTNB.WJ, 2013/11/27, FR-534823, dev for Haptic */
#if !defined(CONFIG_TCT_8X26_RIO6) && !defined(CONFIG_TCT_8X26_RIO6_TF)
	{
		.gpio = 27, /* CAM_MCLK1 */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[0],
			[GPIOMUX_SUSPENDED] = &cam_settings[1],
		},
	},
#endif
/* [PLATFORM]-Modified-END by TCTNB.WJ */
	{
		.gpio = 29, /* CCI_I2C_SDA0 */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[0],
			[GPIOMUX_SUSPENDED] = &gpio_suspend_config[0],
		},
	},
	{
		.gpio = 30, /* CCI_I2C_SCL0 */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[0],
			[GPIOMUX_SUSPENDED] = &gpio_suspend_config[0],
		},
	},
	{
		.gpio = 36, /* CAM1_STANDBY_N */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[4],
		},
	},
	{
		.gpio = 37, /* CAM1_RST_N */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[4],
		},
	},
/* [PLATFORM]-Mod-BEGIN by TCTNB.HJ, 2013/12/30, del CAM pin cfg*/
#if 0
	{
		.gpio = 22, /* CAM1_VDD */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[4],
		},
	},
	{
		.gpio = 34, /* CAM1 VCM_PWDN */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[4],
		},
	},
#endif
/* [PLATFORM]-Mod-END by TCTNB.HJ*/
	{
		.gpio = 35, /* CAM2_STANDBY_N */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[4],
		},
	},
	{
		.gpio = 28, /* CAM2_RST_N */
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[4],
		},
	},

};

static struct gpiomux_setting auxpcm_act_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting auxpcm_sus_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct msm_gpiomux_config msm_auxpcm_configs[] __initdata = {
	{
		.gpio = 63,
		.settings = {
			[GPIOMUX_SUSPENDED] = &auxpcm_sus_cfg,
			[GPIOMUX_ACTIVE] = &auxpcm_act_cfg,
		},
	},
	{
		.gpio = 64,
		.settings = {
			[GPIOMUX_SUSPENDED] = &auxpcm_sus_cfg,
			[GPIOMUX_ACTIVE] = &auxpcm_act_cfg,
		},
	},
	{
		.gpio = 65,
		.settings = {
			[GPIOMUX_SUSPENDED] = &auxpcm_sus_cfg,
			[GPIOMUX_ACTIVE] = &auxpcm_act_cfg,
		},
	},
	{
		.gpio = 66,
		.settings = {
			[GPIOMUX_SUSPENDED] = &auxpcm_sus_cfg,
			[GPIOMUX_ACTIVE] = &auxpcm_act_cfg,
		},
	},
};

/* [PLATFORM]-Add-BEGIN by TCTNB.YJ, 2013/8/1, FR-487741, config msm8226 gpios */
static struct gpiomux_setting unused_gpio_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_IN,
};

static struct msm_gpiomux_config msm8226_unused_gpio_config[] __initdata = {
	{
		.gpio = 51,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 75,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 76,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 77,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 78,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 79,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 80,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 81,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 82,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 85,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 86,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 88,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 89,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 90,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 91,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 92,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 93,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 94,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 97,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 98,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 103,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 104,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 108,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 112,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 115,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 116,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 117,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 118,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 119,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
	{
		.gpio = 120,
		.settings = {
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
		},
	},
};

static struct gpiomux_setting speaker_au_pa_ctrl[] = {
	/* Suspended state */
	{
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},
	/* Active state */
	{
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_8MA,
		.pull = GPIOMUX_PULL_UP,
	},
};

static struct msm_gpiomux_config msm8226_speaker_au_pa_ctrl_config[] __initdata = {
	{
		.gpio = 3,
		.settings = {
			[GPIOMUX_SUSPENDED] = &speaker_au_pa_ctrl[0],
			[GPIOMUX_ACTIVE] = &speaker_au_pa_ctrl[1],
		},
	},
};

static struct gpiomux_setting lcd_id_pin[] = {
	/* Suspended state */
	{
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},
	/* Active state */
	{
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},
};

static struct msm_gpiomux_config msm8226_lcd_id_pin_configs[] __initdata = {
	{
		.gpio = 2,
		.settings = {
			[GPIOMUX_SUSPENDED] = &lcd_id_pin[0],
			[GPIOMUX_ACTIVE] = &lcd_id_pin[1],
		},
	},
};

/* [PLATFORM]-Mod-BEGIN by TCTNB.YuBin, 2013/11/28, refer to bug562529,case01362220 Enable external ovp */
static struct gpiomux_setting for_gpio109_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_OUT_LOW,
};

static struct msm_gpiomux_config msm8226_for_gpio109_pin_config[] __initdata = {
	{
		.gpio = 109,
		.settings = {
			[GPIOMUX_SUSPENDED] = &for_gpio109_cfg,
			[GPIOMUX_ACTIVE] = &for_gpio109_cfg,
		},
	},
};
/* [PLATFORM]-Mod-END by TCTNB.YuBin */

/* [PLATFORM]-Mod-BEGIN by TCTNB.Alvin, PR-570229, 2014/01/27, enable powerIC for rio6 */
static struct gpiomux_setting boardid_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting lcd_pwric_en_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct msm_gpiomux_config msm8226_boardid_config[] __initdata = {
	{
		.gpio = 52,
		.settings = {
			[GPIOMUX_SUSPENDED] = &boardid_cfg,
			[GPIOMUX_ACTIVE] = &boardid_cfg,
		},
	},
	{
		.gpio = 66,
		.settings = {
			[GPIOMUX_SUSPENDED] = &boardid_cfg,
			[GPIOMUX_ACTIVE] = &boardid_cfg,
		},
	},
	{
		.gpio = 67,
		.settings = {
			[GPIOMUX_SUSPENDED] = &boardid_cfg,
			[GPIOMUX_ACTIVE] = &boardid_cfg,
		},
	},
};

static struct msm_gpiomux_config msm8226_lcd_pwric_en_config[] __initdata = {
	{
		.gpio = 110,
		.settings = {
			[GPIOMUX_SUSPENDED] = &lcd_pwric_en_cfg,
			[GPIOMUX_ACTIVE] = &lcd_pwric_en_cfg,
		},
	},
	{
		.gpio = 111,
		.settings = {
			[GPIOMUX_SUSPENDED] = &lcd_pwric_en_cfg,
			[GPIOMUX_ACTIVE] = &lcd_pwric_en_cfg,
		},
	},
};
/* [PLATFORM]-Mod-END by TCTNB.Alvin */
/* [PLATFORM]-Mod-BEGIN by wangjie@tcl.com, PR-570229, 2014/02/21, change NPI_DOWM pin for rio6 */
static struct gpiomux_setting NPI_DOWN_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_IN,
};
static struct msm_gpiomux_config msm8226_NPI_DOWN_config[] __initdata = {
	{
		.gpio = 50,
		.settings = {
			[GPIOMUX_SUSPENDED] = &NPI_DOWN_cfg,
			[GPIOMUX_ACTIVE] = &NPI_DOWN_cfg,
		},
	},
};
/* [PLATFORM]-Mod-END by wangjie@tcl.com */
/* [PLATFORM]-Mod-BEGIN by TCTNB.YuBin, 2013/12/17, make nfc gpio low power for nfc is control by perso */
static struct msm_gpiomux_config msm_pn547_configs[] __initdata = {
	{
		.gpio      = 20,
		.settings = {
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
		},
	},
	{
		.gpio      = 21,
		.settings = {
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
		},
	},
	{
		.gpio      = 22,
		.settings = {
			[GPIOMUX_ACTIVE] = &unused_gpio_cfg,
			[GPIOMUX_SUSPENDED] = &unused_gpio_cfg,
		},
	},
};
/* [PLATFORM]-Mod-END by TCTNB.YuBin */

/* [PLATFORM]-Mod-BEGIN by TCTNB.HJ, 2013/08/28, CAM pins*/
static struct gpiomux_setting cam_af_en_pin[] = {
	/* Suspended state */
	{
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},
	/* Active state */
	{
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},
};

static struct msm_gpiomux_config msm8226_cam_af_en_pin_config[] __initdata = {
	{
		.gpio = 23,
		.settings = {
			[GPIOMUX_SUSPENDED] = &cam_af_en_pin[0],
			[GPIOMUX_ACTIVE] = &cam_af_en_pin[1],
		},
	},
};

static struct gpiomux_setting cam_avdd_en_pin[] = {
	/* Suspended state */
	{
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},
	/* Active state */
	{
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},
};

static struct msm_gpiomux_config msm8226_cam_avdd_en_pin_config[] __initdata = {
	{
		.gpio = 31,
		.settings = {
			[GPIOMUX_SUSPENDED] = &cam_avdd_en_pin[0],
			[GPIOMUX_ACTIVE] = &cam_avdd_en_pin[1],
		},
	},
};
/* [PLATFORM]-Mod-END by TCTNB.HJ*/
static struct gpiomux_setting cam_id_pin[] = {
	/* Suspended state */
	{
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},
	/* Active state */
	{
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},
};

static struct msm_gpiomux_config msm8226_cam_id_pin_configs[] __initdata = {
	{
		.gpio = 33,
		.settings = {
			[GPIOMUX_SUSPENDED] = &cam_id_pin[0],
			[GPIOMUX_ACTIVE] = &cam_id_pin[1],
		},
	},
	{
		.gpio = 34,
		.settings = {
			[GPIOMUX_SUSPENDED] = &cam_id_pin[0],
			[GPIOMUX_ACTIVE] = &cam_id_pin[1],
		},
	},
};

static struct gpiomux_setting hac_sd_pin[] = {
	/* Suspended state */
	{
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},
	/* Active state */
	{
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_8MA,
		.pull = GPIOMUX_PULL_UP,
	},
};

static struct msm_gpiomux_config msm8226_hac_sd_pin_config[] __initdata = {
	{
		.gpio = 69,
		.settings = {
			[GPIOMUX_SUSPENDED] = &hac_sd_pin[0],
			[GPIOMUX_ACTIVE] = &hac_sd_pin[1],
		},
	},
};
/* [PLATFORM]-Add-END by TCTNB.YJ */

/* [PLATFORM]-Del-BEGIN by TCTNB.Alvin, 2014/01/27, Unuse pin */
#if 0
static struct gpiomux_setting usb_otg_sw_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.dir = GPIOMUX_OUT_LOW,
};

static struct msm_gpiomux_config usb_otg_sw_configs[] __initdata = {
	{
		.gpio = 67,
		.settings = {
			[GPIOMUX_SUSPENDED] = &usb_otg_sw_cfg,
		},
	},
};
#endif
/* [PLATFORM]-Del-END by TCTNB.Alvin */

#ifdef CONFIG_MMC_MSM_SDC3_SUPPORT
static struct gpiomux_setting sdc3_clk_actv_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting sdc3_cmd_data_0_3_actv_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting sdc3_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting sdc3_data_1_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct msm_gpiomux_config msm8226_sdc3_configs[] __initdata = {
	{
		/* DAT3 */
		.gpio      = 39,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdc3_cmd_data_0_3_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdc3_suspend_cfg,
		},
	},
	{
		/* DAT2 */
		.gpio      = 40,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdc3_cmd_data_0_3_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdc3_suspend_cfg,
		},
	},
	{
		/* DAT1 */
		.gpio      = 41,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdc3_cmd_data_0_3_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdc3_data_1_suspend_cfg,
		},
	},
	{
		/* DAT0 */
		.gpio      = 42,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdc3_cmd_data_0_3_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdc3_suspend_cfg,
		},
	},
	{
		/* CMD */
		.gpio      = 43,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdc3_cmd_data_0_3_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdc3_suspend_cfg,
		},
	},
	{
		/* CLK */
		.gpio      = 44,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sdc3_clk_actv_cfg,
			[GPIOMUX_SUSPENDED] = &sdc3_suspend_cfg,
		},
	},
};

static void msm_gpiomux_sdc3_install(void)
{
	msm_gpiomux_install(msm8226_sdc3_configs,
			    ARRAY_SIZE(msm8226_sdc3_configs));
}
#else
static void msm_gpiomux_sdc3_install(void) {}
#endif /* CONFIG_MMC_MSM_SDC3_SUPPORT */

void __init msm8226_init_gpiomux(void)
{
	int rc;

	rc = msm_gpiomux_init_dt();
	if (rc) {
		pr_err("%s failed %d\n", __func__, rc);
		return;
	}

#if defined(CONFIG_KS8851) || defined(CONFIG_KS8851_MODULE)
	msm_gpiomux_install(msm_eth_configs, ARRAY_SIZE(msm_eth_configs));
#endif
	msm_gpiomux_install(msm_keypad_configs,
			ARRAY_SIZE(msm_keypad_configs));

	if (of_board_is_qrd())
		msm_gpiomux_install(msm_qrd_blsp_configs,
			ARRAY_SIZE(msm_qrd_blsp_configs));
	else
		msm_gpiomux_install(msm_blsp_configs,
			ARRAY_SIZE(msm_blsp_configs));

	msm_gpiomux_install(wcnss_5wire_interface,
				ARRAY_SIZE(wcnss_5wire_interface));

	msm_gpiomux_install(&sd_card_det, 1);
	if (of_board_is_qrd())
		msm_gpiomux_install(msm_qrd_goodix_configs,
				ARRAY_SIZE(msm_qrd_goodix_configs));
	else
		msm_gpiomux_install(msm_synaptics_configs,
				ARRAY_SIZE(msm_synaptics_configs));

	if (of_board_is_qrd())
		msm_gpiomux_install(msm_qrd_nfc_configs,
				ARRAY_SIZE(msm_qrd_nfc_configs));

	msm_gpiomux_install_nowrite(msm_lcd_configs,
			ARRAY_SIZE(msm_lcd_configs));
	msm_gpiomux_install(msm_sensor_configs, ARRAY_SIZE(msm_sensor_configs));
	msm_gpiomux_install(msm_auxpcm_configs,
			ARRAY_SIZE(msm_auxpcm_configs));

/* [PLATFORM]-Add-BEGIN by TCTNB.YJ, 2013/8/1, FR-487741, config msm8226 gpios */
    msm_gpiomux_install(msm8226_unused_gpio_config,
            ARRAY_SIZE(msm8226_unused_gpio_config));

    msm_gpiomux_install(msm8226_speaker_au_pa_ctrl_config,
            ARRAY_SIZE(msm8226_speaker_au_pa_ctrl_config));

    msm_gpiomux_install(msm8226_lcd_id_pin_configs,
            ARRAY_SIZE(msm8226_lcd_id_pin_configs));

    msm_gpiomux_install(msm8226_cam_af_en_pin_config,
            ARRAY_SIZE(msm8226_cam_af_en_pin_config));

    msm_gpiomux_install(msm8226_cam_avdd_en_pin_config,
            ARRAY_SIZE(msm8226_cam_avdd_en_pin_config));

    msm_gpiomux_install(msm8226_cam_id_pin_configs,
            ARRAY_SIZE(msm8226_cam_id_pin_configs));

    msm_gpiomux_install(msm8226_hac_sd_pin_config,
            ARRAY_SIZE(msm8226_hac_sd_pin_config));
/* [PLATFORM]-Add-END by TCTNB.YJ */

/* [PLATFORM]-Mod-BEGIN by TCTNB.YuBin, 2013/11/28, refer to bug562529,case01362220 Enable external ovp */
    msm_gpiomux_install(msm8226_for_gpio109_pin_config,
            ARRAY_SIZE(msm8226_for_gpio109_pin_config));
/* [PLATFORM]-Mod-END by TCTNB.YuBin */
/* [PLATFORM]-Mod-BEGIN by TCTNB.Alvin, PR-570229, 2014/01/27, enable powerIC for rio6 */
    msm_gpiomux_install(msm8226_boardid_config,
            ARRAY_SIZE(msm8226_boardid_config));
    msm_gpiomux_install(msm8226_lcd_pwric_en_config,
            ARRAY_SIZE(msm8226_lcd_pwric_en_config));
/* [PLATFORM]-Add-END by TCTNB.Alvin */

/* [PLATFORM]-Mod-BEGIN by wangjie@tcl.com, PR-570229, 2014/02/21, change NPI_DOWM pin for rio6 */
    msm_gpiomux_install(msm8226_NPI_DOWN_config,
            ARRAY_SIZE(msm8226_NPI_DOWN_config));
/* [PLATFORM]-Add-END by  wangjie@tcl.com */

/* [PLATFORM]-ADD-BEGIN by TCTNB.YuBin, 2013/12/17, make nfc gpio low power for nfc is control by perso */
    msm_gpiomux_install(msm_pn547_configs,
            ARRAY_SIZE(msm_pn547_configs));
/* [PLATFORM]-ADD-END by TCTNB.YuBin */

/* [PLATFORM]-Del-BEGIN by TCTNB.Alvin, 2014/01/27, Unuse pin */
#if 0
	if (of_board_is_cdp() || of_board_is_mtp() || of_board_is_xpm())
		msm_gpiomux_install(usb_otg_sw_configs,
					ARRAY_SIZE(usb_otg_sw_configs));
#endif
/* [PLATFORM]-Del-END by TCTNB.Alvin */

	msm_gpiomux_sdc3_install();
}
