/* Copyright (c) 2013, The Linux Foundation. All rights reserved.
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
#define pr_fmt(fmt) "%s:%d,"fmt, __func__, __LINE__

#include <mach/gpiomux.h>
#include "msm_sensor.h"
#include "msm_cci.h"
#include "msm_camera_io_util.h"
#include "msm_camera_i2c_mux.h"

#define GC0339_SENSOR_NAME "gc0339_pop7lte"

DEFINE_MSM_MUTEX(gc0339_pop7lte_mut);

static uint8_t ewelley_module = 0;

static struct msm_sensor_ctrl_t gc0339_s_ctrl;

static struct msm_sensor_power_setting gc0339_power_setting[] = {
	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_RESET,
		.config_val = GPIO_OUT_LOW,
		.delay = 0,
	},
	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_STANDBY,
		.config_val = GPIO_OUT_HIGH,
		.delay = 0,
	},
	{
		.seq_type = SENSOR_VREG,
		.seq_val = CAM_VIO,
		.config_val = 0,
		.delay = 0,
	},
	{
		.seq_type = SENSOR_VREG,
		.seq_val = CAM_VDIG,
		.config_val = 0,
		.delay = 0,
	},
	{
		.seq_type = SENSOR_VREG,
		.seq_val = CAM_VANA,
		.config_val = 0,
		.delay = 0,
	},
	{
		.seq_type = SENSOR_CLK,
		.seq_val = SENSOR_CAM_MCLK,
		.config_val = 24000000,
		.delay = 5,
	},
	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_STANDBY,
		.config_val = GPIO_OUT_LOW,
		.delay = 0,
	},
	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_RESET,
		.config_val = GPIO_OUT_HIGH,
		.delay = 10,
	},
};

static struct v4l2_subdev_info gc0339_subdev_info[] = {
	{
		.code   = V4L2_MBUS_FMT_SBGGR8_1X8, //V4L2_MBUS_FMT_SRGGB8_1X8,
		.colorspace = V4L2_COLORSPACE_JPEG,
		.fmt    = 1,
		.order    = 0,
	},
};

static int32_t msm_gc0339_i2c_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	return msm_sensor_i2c_probe(client, id, &gc0339_s_ctrl);
}

static const struct i2c_device_id gc0339_i2c_id[] = {
	{GC0339_SENSOR_NAME, (kernel_ulong_t)&gc0339_s_ctrl},
	{ }
};

static struct i2c_driver gc0339_i2c_driver = {
	.id_table = gc0339_i2c_id,
	.probe  = msm_gc0339_i2c_probe,
	.driver = {
		.name = GC0339_SENSOR_NAME,
	},
};

static struct msm_camera_i2c_client gc0339_sensor_i2c_client = {
	.addr_type = MSM_CAMERA_I2C_BYTE_ADDR,
};

static int32_t gc0339_power_up(struct msm_sensor_ctrl_t *s_ctrl)
{
	int32_t rc = 0, index = 0;
	struct msm_sensor_power_setting_array *power_setting_array = NULL;
	struct msm_sensor_power_setting *power_setting = NULL;
	struct msm_camera_sensor_board_info *data = s_ctrl->sensordata;

	pr_err("enter\n");
	power_setting_array = &s_ctrl->power_setting_array;

	if (data->gpio_conf->cam_gpiomux_conf_tbl != NULL) {
		pr_err("%s:%d mux install\n", __func__, __LINE__);
		msm_gpiomux_install(
			(struct msm_gpiomux_config *)
			data->gpio_conf->cam_gpiomux_conf_tbl,
			data->gpio_conf->cam_gpiomux_conf_tbl_size);
	}

	rc = msm_camera_request_gpio_table(
		data->gpio_conf->cam_gpio_req_tbl,
		data->gpio_conf->cam_gpio_req_tbl_size, 1);
	if (rc < 0) {
		pr_err("%s: request gpio failed\n", __func__);
		return rc;
	}
	for (index = 0; index < power_setting_array->size; index++) {
		pr_err("%s index %d\n", __func__, index);
		power_setting = &power_setting_array->power_setting[index];
		pr_err("%s type %d\n", __func__, power_setting->seq_type);
		switch (power_setting->seq_type) {
		case SENSOR_CLK:
			if (power_setting->seq_val >= s_ctrl->clk_info_size) {
				pr_err("%s clk index %d >= max %d\n", __func__,
					power_setting->seq_val,
					s_ctrl->clk_info_size);
				goto power_up_failed;
			}
			if (power_setting->config_val)
				s_ctrl->clk_info[power_setting->seq_val].
					clk_rate = power_setting->config_val;

			rc = msm_cam_clk_enable(s_ctrl->dev,
				&s_ctrl->clk_info[0],
				(struct clk **)&power_setting->data[0],
				s_ctrl->clk_info_size,
				1);
			if (rc < 0) {
				pr_err("%s: clk enable failed\n",
					__func__);
				goto power_up_failed;
			}
			break;
		case SENSOR_GPIO:
			if (power_setting->seq_val >= SENSOR_GPIO_MAX ||
				!data->gpio_conf->gpio_num_info) {
				pr_err("%s gpio index %d >= max %d\n", __func__,
					power_setting->seq_val,
					SENSOR_GPIO_MAX);
				goto power_up_failed;
			}
			pr_debug("%s:%d gpio set val %d\n", __func__, __LINE__,
				data->gpio_conf->gpio_num_info->gpio_num
				[power_setting->seq_val]);
			if (data->gpio_conf->gpio_num_info->gpio_num
				[power_setting->seq_val])
				gpio_set_value_cansleep(
					data->gpio_conf->gpio_num_info->gpio_num
					[power_setting->seq_val],
					power_setting->config_val);
			break;
		case SENSOR_VREG:
			if (power_setting->seq_val >= CAM_VREG_MAX) {
				pr_err("%s vreg index %d >= max %d\n", __func__,
					power_setting->seq_val,
					SENSOR_GPIO_MAX);
				goto power_up_failed;
			}
			msm_camera_config_single_vreg(s_ctrl->dev,
				&data->cam_vreg[power_setting->seq_val],
				(struct regulator **)&power_setting->data[0],
				1);
			break;
		default:
			pr_err("%s error power seq type %d\n", __func__,
				power_setting->seq_type);
			break;
		}
		if (power_setting->delay > 20) {
			msleep(power_setting->delay);
		} else if (power_setting->delay) {
			usleep_range(power_setting->delay * 1000,
				(power_setting->delay * 1000) + 1000);
		}
	}

	if (s_ctrl->sensor_device_type == MSM_CAMERA_PLATFORM_DEVICE) {
		rc = s_ctrl->sensor_i2c_client->i2c_func_tbl->i2c_util(
			s_ctrl->sensor_i2c_client, MSM_CCI_INIT);
		if (rc < 0) {
			pr_err("%s cci_init failed\n", __func__);
			goto power_up_failed;
		}
	}

	s_ctrl->sensor_i2c_client->i2c_func_tbl->i2c_write(
		s_ctrl->sensor_i2c_client,
		0xfc,
		0x10, MSM_CAMERA_I2C_BYTE_DATA);

	if (s_ctrl->func_tbl->sensor_match_id)
		rc = s_ctrl->func_tbl->sensor_match_id(s_ctrl);
	else
		rc = msm_sensor_match_id(s_ctrl);
	if (rc < 0) {
		pr_err("%s:%d match id failed rc %d\n", __func__, __LINE__, rc);
		goto power_up_failed;
	}

	pr_err("%s exit\n", __func__);
	return 0;
power_up_failed:
	pr_err("%s:%d failed\n", __func__, __LINE__);
	if (s_ctrl->sensor_device_type == MSM_CAMERA_PLATFORM_DEVICE) {
		s_ctrl->sensor_i2c_client->i2c_func_tbl->i2c_util(
			s_ctrl->sensor_i2c_client, MSM_CCI_RELEASE);
	}

	for (index--; index >= 0; index--) {
		pr_err("%s index %d\n", __func__, index);
		power_setting = &power_setting_array->power_setting[index];
		pr_err("%s type %d\n", __func__, power_setting->seq_type);
		switch (power_setting->seq_type) {
		case SENSOR_CLK:
			msm_cam_clk_enable(s_ctrl->dev,
				&s_ctrl->clk_info[0],
				(struct clk **)&power_setting->data[0],
				s_ctrl->clk_info_size,
				0);
			break;
		case SENSOR_GPIO:
			if (data->gpio_conf->gpio_num_info->gpio_num
				[power_setting->seq_val])
				gpio_set_value_cansleep(
					data->gpio_conf->gpio_num_info->gpio_num
					[power_setting->seq_val],
					GPIOF_OUT_INIT_LOW);
			break;
		case SENSOR_VREG:
			msm_camera_config_single_vreg(s_ctrl->dev,
				&data->cam_vreg[power_setting->seq_val],
				(struct regulator **)&power_setting->data[0],
				0);
			break;
		default:
			pr_err("%s error power seq type %d\n", __func__,
				power_setting->seq_type);
			break;
		}
		if (power_setting->delay > 20) {
			msleep(power_setting->delay);
		} else if (power_setting->delay) {
			usleep_range(power_setting->delay * 1000,
				(power_setting->delay * 1000) + 1000);
		}
	}
	msm_camera_request_gpio_table(
		data->gpio_conf->cam_gpio_req_tbl,
		data->gpio_conf->cam_gpio_req_tbl_size, 0);

	return rc;
}

static int32_t gc0339_power_down(struct msm_sensor_ctrl_t *s_ctrl)
{
	int32_t index = 0;
	struct msm_sensor_power_setting_array *power_setting_array = NULL;
	struct msm_sensor_power_setting *power_setting = NULL;
	struct msm_camera_sensor_board_info *data = s_ctrl->sensordata;

	pr_err("%s:%d\n", __func__, __LINE__);
	power_setting_array = &s_ctrl->power_setting_array;

	s_ctrl->sensor_i2c_client->i2c_func_tbl->i2c_write(
		s_ctrl->sensor_i2c_client,
		0xfc,
		0x01, MSM_CAMERA_I2C_BYTE_DATA);
		
	if (s_ctrl->sensor_device_type == MSM_CAMERA_PLATFORM_DEVICE) {
		s_ctrl->sensor_i2c_client->i2c_func_tbl->i2c_util(
			s_ctrl->sensor_i2c_client, MSM_CCI_RELEASE);
	}

	for (index = (power_setting_array->size - 1); index >= 0; index--) {
		pr_err("%s index %d\n", __func__, index);
		power_setting = &power_setting_array->power_setting[index];
		pr_err("%s type %d\n", __func__, power_setting->seq_type);
		switch (power_setting->seq_type) {
		case SENSOR_CLK:
			msm_cam_clk_enable(s_ctrl->dev,
				&s_ctrl->clk_info[0],
				(struct clk **)&power_setting->data[0],
				s_ctrl->clk_info_size,
				0);
			break;
		case SENSOR_GPIO:
			if (power_setting->seq_val >= SENSOR_GPIO_MAX ||
				!data->gpio_conf->gpio_num_info) {
				pr_err("%s gpio index %d >= max %d\n", __func__,
					power_setting->seq_val,
					SENSOR_GPIO_MAX);
				continue;
			}
			if (data->gpio_conf->gpio_num_info->gpio_num
				[power_setting->seq_val])
				gpio_set_value_cansleep(
					data->gpio_conf->gpio_num_info->gpio_num
					[power_setting->seq_val],
					GPIOF_OUT_INIT_LOW);
			break;
		case SENSOR_VREG:
			if (power_setting->seq_val >= CAM_VREG_MAX) {
				pr_err("%s vreg index %d >= max %d\n", __func__,
					power_setting->seq_val,
					SENSOR_GPIO_MAX);
				continue;
			}
			msm_camera_config_single_vreg(s_ctrl->dev,
				&data->cam_vreg[power_setting->seq_val],
				(struct regulator **)&power_setting->data[0],
				0);
			break;
		default:
			pr_err("%s error power seq type %d\n", __func__,
				power_setting->seq_type);
			break;
		}
		if (power_setting->delay > 20) {
			msleep(power_setting->delay);
		} else if (power_setting->delay) {
			usleep_range(power_setting->delay * 1000,
				(power_setting->delay * 1000) + 1000);
		}
	}
	msm_camera_request_gpio_table(
		data->gpio_conf->cam_gpio_req_tbl,
		data->gpio_conf->cam_gpio_req_tbl_size, 0);
	pr_err("%s exit\n", __func__);
	
	return 0;
}

static int32_t gc0339_match_id(struct msm_sensor_ctrl_t *s_ctrl)
{
	int32_t rc = 0;
	uint16_t chipid = 0;
	uint32_t gpio_ctype = 0;

	rc = s_ctrl->sensor_i2c_client->i2c_func_tbl->i2c_read(
			s_ctrl->sensor_i2c_client,
			s_ctrl->sensordata->slave_info->sensor_id_reg_addr,
			&chipid, MSM_CAMERA_I2C_BYTE_DATA);
	if (rc < 0) {
		pr_err("%s: %s: read id failed\n", __func__,
			s_ctrl->sensordata->sensor_name);
		return rc;
	}

	pr_err("expect id 0x%x, read id:0x%x\n", 
		s_ctrl->sensordata->slave_info->sensor_id, 
		chipid);
	if (chipid != s_ctrl->sensordata->slave_info->sensor_id) {
		pr_err("msm_sensor_match_id chip id doesnot match\n");
		return -ENODEV;
	}

	if (ewelley_module)
		return rc;
	
	rc = of_property_read_u32(s_ctrl->pdev->dev.of_node, "tct,ctype", &gpio_ctype);
	if (0 > rc) {
		pr_err("read ctype fail, rc:%d\n", rc);
		return rc;
	}
	rc = gpio_request(gpio_ctype, "gc0339_ctype");
	pr_err("gpio request %d, rc:%d\n", gpio_ctype, rc);
	rc = gpio_tlmm_config(GPIO_CFG(gpio_ctype, 0, 
		GPIO_CFG_INPUT, GPIO_CFG_NO_PULL,
		GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	pr_err("gpio tlmm config %d, rc:%d\n", gpio_ctype, rc);
	rc = gpio_direction_input(gpio_ctype);
	pr_err("gpio direction input %d, rc:%d\n", gpio_ctype, rc);
	rc = gpio_get_value(gpio_ctype);
	pr_err("gpio get value %d, rc:%d\n", gpio_ctype, rc);
	gpio_free(gpio_ctype);
	if (0 != rc) {
		pr_err("inappropriate driver for not ewelley module.\n");
		return -1;
	}
	ewelley_module = 1;

	return rc;
}

static int32_t gc0339_config(struct msm_sensor_ctrl_t *s_ctrl,
	void __user *argp)
{
	struct sensorb_cfg_data *cdata = (struct sensorb_cfg_data *)argp;
	long rc = 0;
	int32_t i = 0;
	mutex_lock(s_ctrl->msm_sensor_mutex);
	switch (cdata->cfgtype) {
	case CFG_GET_SENSOR_INFO:
		memcpy(cdata->cfg.sensor_info.sensor_name,
			s_ctrl->sensordata->sensor_name,
			sizeof(cdata->cfg.sensor_info.sensor_name));
		cdata->cfg.sensor_info.session_id =
			s_ctrl->sensordata->sensor_info->session_id;
		for (i = 0; i < SUB_MODULE_MAX; i++)
			cdata->cfg.sensor_info.subdev_id[i] =
				s_ctrl->sensordata->sensor_info->subdev_id[i];
		pr_err("%s:%d sensor name %s\n", __func__, __LINE__,
			cdata->cfg.sensor_info.sensor_name);
		pr_err("%s:%d session id %d\n", __func__, __LINE__,
			cdata->cfg.sensor_info.session_id);
		for (i = 0; i < SUB_MODULE_MAX; i++)
			pr_err("%s:%d subdev_id[%d] %d\n", __func__, __LINE__, i,
				cdata->cfg.sensor_info.subdev_id[i]);

		break;
	case CFG_GET_SENSOR_INIT_PARAMS:
		cdata->cfg.sensor_init_params =
			*s_ctrl->sensordata->sensor_init_params;
		pr_err("%s:%d init params mode %d pos %d mount %d\n", __func__,
			__LINE__,
			cdata->cfg.sensor_init_params.modes_supported,
			cdata->cfg.sensor_init_params.position,
			cdata->cfg.sensor_init_params.sensor_mount_angle);
		break;
	case CFG_SET_SLAVE_INFO: {
		struct msm_camera_sensor_slave_info sensor_slave_info;
		struct msm_sensor_power_setting_array *power_setting_array;
		int slave_index = 0;
		if (copy_from_user(&sensor_slave_info,
			(void *)cdata->cfg.setting,
			sizeof(struct msm_camera_sensor_slave_info))) {
			pr_err("%s:%d failed\n", __func__, __LINE__);
			rc = -EFAULT;
			break;
		}
		/* Update sensor slave address */
		if (sensor_slave_info.slave_addr) {
			s_ctrl->sensor_i2c_client->cci_client->sid =
				sensor_slave_info.slave_addr >> 1;
		}

		/* Update sensor address type */
		s_ctrl->sensor_i2c_client->addr_type =
			sensor_slave_info.addr_type;

		/* Update power up / down sequence */
		s_ctrl->power_setting_array =
			sensor_slave_info.power_setting_array;
		power_setting_array = &s_ctrl->power_setting_array;
		power_setting_array->power_setting = kzalloc(
			power_setting_array->size *
			sizeof(struct msm_sensor_power_setting), GFP_KERNEL);
		if (!power_setting_array->power_setting) {
			pr_err("%s:%d failed\n", __func__, __LINE__);
			rc = -ENOMEM;
			break;
		}
		if (copy_from_user(power_setting_array->power_setting,
			(void *)
			sensor_slave_info.power_setting_array.power_setting,
			power_setting_array->size *
			sizeof(struct msm_sensor_power_setting))) {
			pr_err("%s:%d failed\n", __func__, __LINE__);
			rc = -EFAULT;
			break;
		}
		s_ctrl->free_power_setting = true;
		pr_err("%s sensor id %x\n", __func__,
			sensor_slave_info.slave_addr);
		pr_err("%s sensor addr type %d\n", __func__,
			sensor_slave_info.addr_type);
		pr_err("%s sensor reg %x\n", __func__,
			sensor_slave_info.sensor_id_info.sensor_id_reg_addr);
		pr_err("%s sensor id %x\n", __func__,
			sensor_slave_info.sensor_id_info.sensor_id);
		for (slave_index = 0; slave_index <
			power_setting_array->size; slave_index++) {
			pr_err("%s i %d power setting %d %d %ld %d\n", __func__,
				slave_index,
				power_setting_array->power_setting[slave_index].
				seq_type,
				power_setting_array->power_setting[slave_index].
				seq_val,
				power_setting_array->power_setting[slave_index].
				config_val,
				power_setting_array->power_setting[slave_index].
				delay);
		}
		break;
	}
	case CFG_WRITE_I2C_ARRAY: {
		struct msm_camera_i2c_reg_setting conf_array;
		struct msm_camera_i2c_reg_array *reg_setting = NULL;

		if (copy_from_user(&conf_array,
			(void *)cdata->cfg.setting,
			sizeof(struct msm_camera_i2c_reg_setting))) {
			pr_err("%s:%d failed\n", __func__, __LINE__);
			rc = -EFAULT;
			break;
		}

		if (conf_array.addr_type == MSM_CAMERA_I2C_WORD_ADDR
			|| conf_array.data_type == MSM_CAMERA_I2C_WORD_DATA
			|| !conf_array.size)
			break;

		reg_setting = kzalloc(conf_array.size *
			(sizeof(struct msm_camera_i2c_reg_array)), GFP_KERNEL);
		if (!reg_setting) {
			pr_err("%s:%d failed\n", __func__, __LINE__);
			rc = -ENOMEM;
			break;
		}
		if (copy_from_user(reg_setting, (void *)conf_array.reg_setting,
			conf_array.size *
			sizeof(struct msm_camera_i2c_reg_array))) {
			pr_err("%s:%d failed\n", __func__, __LINE__);
			kfree(reg_setting);
			rc = -EFAULT;
			break;
		}

		conf_array.reg_setting = reg_setting;
		rc = s_ctrl->sensor_i2c_client->i2c_func_tbl->i2c_write_table(
			s_ctrl->sensor_i2c_client, &conf_array);
		kfree(reg_setting);
		break;
	}
	case CFG_WRITE_I2C_SEQ_ARRAY: {
		struct msm_camera_i2c_seq_reg_setting conf_array;
		struct msm_camera_i2c_seq_reg_array *reg_setting = NULL;

		if (copy_from_user(&conf_array,
			(void *)cdata->cfg.setting,
			sizeof(struct msm_camera_i2c_seq_reg_setting))) {
			pr_err("%s:%d failed\n", __func__, __LINE__);
			rc = -EFAULT;
			break;
		}

		reg_setting = kzalloc(conf_array.size *
			(sizeof(struct msm_camera_i2c_seq_reg_array)),
			GFP_KERNEL);
		if (!reg_setting) {
			pr_err("%s:%d failed\n", __func__, __LINE__);
			rc = -ENOMEM;
			break;
		}
		if (copy_from_user(reg_setting, (void *)conf_array.reg_setting,
			conf_array.size *
			sizeof(struct msm_camera_i2c_seq_reg_array))) {
			pr_err("%s:%d failed\n", __func__, __LINE__);
			kfree(reg_setting);
			rc = -EFAULT;
			break;
		}

		conf_array.reg_setting = reg_setting;
		rc = s_ctrl->sensor_i2c_client->i2c_func_tbl->
			i2c_write_seq_table(s_ctrl->sensor_i2c_client,
			&conf_array);
		kfree(reg_setting);
		break;
	}

	case CFG_POWER_UP:
		if (s_ctrl->func_tbl->sensor_power_up)
			rc = s_ctrl->func_tbl->sensor_power_up(s_ctrl);
		else
			rc = -EFAULT;
		break;

	case CFG_POWER_DOWN:
		if (s_ctrl->func_tbl->sensor_power_down)
			rc = s_ctrl->func_tbl->sensor_power_down(
				s_ctrl);
		else
			rc = -EFAULT;
		break;

	case CFG_SET_STOP_STREAM_SETTING: {
		struct msm_camera_i2c_reg_setting *stop_setting =
			&s_ctrl->stop_setting;
		struct msm_camera_i2c_reg_array *reg_setting = NULL;
		if (copy_from_user(stop_setting,
			(void *)cdata->cfg.setting,
			sizeof(struct msm_camera_i2c_reg_setting))) {
			pr_err("%s:%d failed\n", __func__, __LINE__);
			rc = -EFAULT;
			break;
		}

		reg_setting = stop_setting->reg_setting;
		stop_setting->reg_setting = kzalloc(stop_setting->size *
			(sizeof(struct msm_camera_i2c_reg_array)), GFP_KERNEL);
		if (!stop_setting->reg_setting) {
			pr_err("%s:%d failed\n", __func__, __LINE__);
			rc = -ENOMEM;
			break;
		}
		if (copy_from_user(stop_setting->reg_setting,
			(void *)reg_setting,
			stop_setting->size *
			sizeof(struct msm_camera_i2c_reg_array))) {
			pr_err("%s:%d failed\n", __func__, __LINE__);
			kfree(stop_setting->reg_setting);
			stop_setting->reg_setting = NULL;
			stop_setting->size = 0;
			rc = -EFAULT;
			break;
		}
		break;
	}
	default:
		rc = -EFAULT;
		break;
	}

	mutex_unlock(s_ctrl->msm_sensor_mutex);
	return rc;
}

static struct msm_sensor_fn_t gc0339_sensor_fn_t = {
	.sensor_power_up = gc0339_power_up,
	.sensor_power_down = gc0339_power_down,
	.sensor_match_id = gc0339_match_id,
	.sensor_config = gc0339_config,
};


static struct msm_sensor_ctrl_t gc0339_s_ctrl = {
	.sensor_i2c_client = &gc0339_sensor_i2c_client,
	.power_setting_array.power_setting = gc0339_power_setting,
	.power_setting_array.size = ARRAY_SIZE(gc0339_power_setting),
	.msm_sensor_mutex = &gc0339_pop7lte_mut,
	.sensor_v4l2_subdev_info = gc0339_subdev_info,
	.sensor_v4l2_subdev_info_size = ARRAY_SIZE(gc0339_subdev_info),
	.func_tbl = &gc0339_sensor_fn_t,
};

static const struct of_device_id gc0339_dt_match[] = {
	{.compatible = "qcom,gc0339_pop7lte", .data = &gc0339_s_ctrl},
	{}
};

MODULE_DEVICE_TABLE(of, gc0339_dt_match);

static struct platform_driver gc0339_platform_driver = {
	.driver = {
		.name = "qcom,gc0339_pop7lte",
		.owner = THIS_MODULE,
		.of_match_table = gc0339_dt_match,
	},
};

static int32_t gc0339_platform_probe(struct platform_device *pdev)
{
	int32_t rc = 0;
	const struct of_device_id *match;

	match = of_match_device(gc0339_dt_match, &pdev->dev);
	rc = msm_sensor_platform_probe(pdev, match->data);
	return rc;
}

static int __init gc0339_init_module(void)
{
	int32_t rc = 0;

	pr_err("name:%s\n", gc0339_platform_driver.driver.name);
	rc = platform_driver_probe(&gc0339_platform_driver,
		gc0339_platform_probe);
	if (!rc)
		return rc;

	pr_err("platform probe fail, then i2c probe\n");
	return i2c_add_driver(&gc0339_i2c_driver);
}

static void __exit gc0339_exit_module(void)
{
	if (gc0339_s_ctrl.pdev) {
		msm_sensor_free_sensor_data(&gc0339_s_ctrl);
		platform_driver_unregister(&gc0339_platform_driver);
	} else
		i2c_del_driver(&gc0339_i2c_driver);
	return;
}

module_init(gc0339_init_module);
module_exit(gc0339_exit_module);
MODULE_DESCRIPTION("gc0339");
MODULE_LICENSE("GPL v2");
