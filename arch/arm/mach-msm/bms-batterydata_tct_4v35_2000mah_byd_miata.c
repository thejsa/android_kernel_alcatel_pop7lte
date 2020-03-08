#include <linux/batterydata-lib.h>

static struct single_row_lut fcc_temp = {
	.x		= {-20, 0, 25, 40, 60},
	.y		= {2008, 2026, 2040, 2023, 2015},
	.cols	= 5
};

static struct single_row_lut fcc_sf = {
	.x		= {0},
	.y		= {100},
	.cols	= 1
};

static struct sf_lut rbatt_sf = {
	.rows		= 29,
	.cols		= 5,
	.row_entries		= {-20, 0, 25, 40, 60},
	.percent	= {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
	.sf		= {
				{698, 226, 100, 81, 78},
				{698, 226, 100, 81, 78},
				{655, 236, 104, 84, 78},
				{653, 242, 108, 86, 80},
				{703, 254, 113, 88, 83},
				{698, 248, 119, 92, 85},
				{692, 221, 125, 96, 88},
				{678, 221, 127, 101, 90},
				{673, 219, 115, 103, 94},
				{696, 219, 103, 90, 88},
				{745, 218, 103, 84, 80},
				{808, 218, 104, 86, 82},
				{873, 229, 107, 88, 84},
				{951, 255, 107, 90, 88},
				{1062, 297, 111, 90, 88},
				{1300, 350, 116, 90, 82},
				{1741, 406, 119, 88, 82},
				{2580, 470, 125, 88, 82},
				{3441, 523, 127, 89, 84},
				{2625, 549, 131, 90, 84},
				{2870, 584, 139, 93, 86},
				{3243, 618, 146, 96, 87},
				{3702, 658, 149, 96, 86},
				{4161, 698, 154, 94, 84},
				{4984, 739, 170, 95, 85},
				{6664, 800, 198, 100, 89},
				{13993, 937, 258, 108, 96},
				{46641, 1692, 3550, 146, 107},
				{124031, 16214, 72994, 13663, 2511}
	}
};

static struct pc_temp_ocv_lut pc_temp_ocv = {
	.rows		= 29,
	.cols		= 5,
	.temp		= {-20, 0, 25, 40, 60},
	.percent	= {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
	.ocv		= {
				{4328, 4325, 4322, 4318, 4311},
				{4229, 4243, 4250, 4249, 4245},
				{4161, 4181, 4192, 4192, 4188},
				{4089, 4127, 4137, 4137, 4134},
				{4039, 4078, 4085, 4085, 4082},
				{3956, 4018, 4037, 4037, 4034},
				{3913, 3951, 3987, 3991, 3990},
				{3873, 3913, 3948, 3952, 3950},
				{3831, 3877, 3900, 3914, 3912},
				{3806, 3846, 3857, 3867, 3868},
				{3791, 3820, 3829, 3832, 3832},
				{3778, 3797, 3807, 3810, 3809},
				{3765, 3783, 3790, 3792, 3791},
				{3753, 3775, 3776, 3777, 3776},
				{3741, 3767, 3767, 3764, 3761},
				{3730, 3754, 3756, 3751, 3737},
				{3716, 3731, 3733, 3729, 3716},
				{3694, 3702, 3700, 3699, 3687},
				{3652, 3686, 3679, 3675, 3664},
				{3640, 3682, 3677, 3674, 3663},
				{3624, 3678, 3673, 3672, 3661},
				{3602, 3672, 3664, 3669, 3658},
				{3578, 3663, 3639, 3660, 3647},
				{3545, 3643, 3593, 3634, 3616},
				{3500, 3605, 3531, 3585, 3569},
				{3437, 3547, 3449, 3523, 3506},
				{3345, 3463, 3332, 3438, 3425},
				{3220, 3332, 3187, 3305, 3299},
				{3000, 3000, 3000, 3000, 3000}
	}
};

struct bms_battery_data tct_4v35_2000mah_byd_miata = {
	.fcc				= 2000,
	.fcc_temp_lut			= &fcc_temp,
	.fcc_sf_lut				= &fcc_sf,
	.pc_temp_ocv_lut		= &pc_temp_ocv,
	.rbatt_sf_lut			= &rbatt_sf,
	.default_rbatt_mohm	= 151
};

