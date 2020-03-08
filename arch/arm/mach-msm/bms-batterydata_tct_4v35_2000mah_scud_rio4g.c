#include <linux/batterydata-lib.h>

static struct single_row_lut fcc_temp = {
	.x		= {-20, 0, 25, 40, 60},
	.y		= {2068, 2068, 2068, 2065, 2060},
	.cols	= 5
};

static struct single_row_lut fcc_sf = {
	.x		= {0},
	.y		= {100},
	.cols	= 1
};

static struct sf_lut rbatt_sf = {
	.rows		= 31,
	.cols		= 5,
	.row_entries		= {-20, 0, 25, 40, 60},
	.percent	= {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 16, 13, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
	.sf		= {
				{1157, 250, 100, 84, 78},
				{1154, 250, 100, 84, 78},
				{1062, 257, 102, 85, 79},
				{991, 266, 104, 87, 80},
				{968, 269, 107, 88, 81},
				{871, 278, 114, 92, 82},
				{860, 240, 116, 94, 84},
				{857, 226, 127, 100, 87},
				{852, 228, 123, 102, 89},
				{865, 224, 101, 87, 80},
				{896, 225, 99, 85, 79},
				{933, 226, 100, 87, 81},
				{973, 229, 103, 89, 84},
				{1015, 239, 106, 91, 85},
				{1062, 255, 108, 90, 84},
				{1119, 272, 107, 87, 80},
				{1185, 280, 104, 86, 80},
				{1236, 280, 103, 87, 80},
				{1157, 276, 96, 82, 77},
				{1203, 289, 98, 85, 79},
				{1238, 298, 100, 87, 80},
				{1262, 310, 102, 88, 84},
				{1300, 323, 106, 90, 87},
				{1342, 341, 109, 94, 92},
				{1376, 356, 112, 97, 89},
				{1442, 368, 112, 91, 83},
				{1615, 368, 105, 89, 84},
				{1865, 382, 109, 92, 88},
				{2262, 419, 117, 101, 95},
				{3659, 493, 146, 212, 108},
				{76427, 69943, 65953, 39152, 23517}
	}
};

static struct pc_temp_ocv_lut pc_temp_ocv = {
	.rows		= 31,
	.cols		= 5,
	.temp		= {-20, 0, 25, 40, 60},
	.percent	= {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 16, 13, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
	.ocv		= {
				{4326, 4327, 4324, 4317, 4313},
				{4224, 4258, 4263, 4259, 4256},
				{4156, 4203, 4208, 4205, 4202},
				{4093, 4153, 4156, 4153, 4149},
				{4049, 4105, 4106, 4102, 4099},
				{3958, 4059, 4061, 4054, 4051},
				{3913, 3975, 4004, 4005, 4007},
				{3877, 3924, 3964, 3969, 3967},
				{3842, 3896, 3924, 3925, 3924},
				{3815, 3866, 3874, 3875, 3875},
				{3798, 3838, 3844, 3845, 3845},
				{3783, 3815, 3822, 3822, 3821},
				{3769, 3796, 3803, 3803, 3802},
				{3756, 3783, 3788, 3786, 3785},
				{3744, 3772, 3775, 3770, 3763},
				{3732, 3759, 3762, 3752, 3738},
				{3719, 3737, 3741, 3731, 3715},
				{3707, 3714, 3714, 3704, 3688},
				{3695, 3703, 3692, 3683, 3671},
				{3683, 3698, 3690, 3682, 3669},
				{3677, 3696, 3689, 3681, 3668},
				{3668, 3694, 3688, 3680, 3668},
				{3658, 3692, 3687, 3679, 3666},
				{3646, 3689, 3685, 3677, 3663},
				{3631, 3684, 3680, 3671, 3652},
				{3611, 3671, 3662, 3649, 3618},
				{3583, 3639, 3620, 3602, 3568},
				{3545, 3584, 3559, 3539, 3503},
				{3484, 3505, 3477, 3455, 3412},
				{3376, 3382, 3342, 3313, 3258},
				{3000, 3000, 3000, 3000, 3000}
	}
};

struct bms_battery_data tct_4v35_2000mah_scud_rio4g = {
	.fcc				= 2000,
	.fcc_temp_lut			= &fcc_temp,
	.fcc_sf_lut				= &fcc_sf,
	.pc_temp_ocv_lut		= &pc_temp_ocv,
	.rbatt_sf_lut			= &rbatt_sf,
	.default_rbatt_mohm	= 163
};
