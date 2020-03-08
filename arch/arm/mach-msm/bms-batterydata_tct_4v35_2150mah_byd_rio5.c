#include <linux/batterydata-lib.h>

static struct single_row_lut fcc_temp = {
	.x		= {-20, 0, 25, 40, 60},
	.y		= {2009, 2041, 2046, 2043, 2037},
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
				{1005, 241, 100, 85, 81},
				{1005, 241, 100, 85, 81},
				{911, 244, 101, 86, 82},
				{861, 247, 103, 88, 83},
				{821, 248, 106, 90, 85},
				{812, 240, 110, 92, 86},
				{813, 221, 116, 95, 88},
				{820, 215, 123, 100, 91},
				{832, 211, 125, 106, 95},
				{856, 211, 111, 102, 96},
				{894, 211, 102, 89, 86},
				{943, 212, 102, 89, 85},
				{1005, 216, 103, 90, 87},
				{1089, 225, 104, 93, 89},
				{1212, 239, 105, 93, 90},
				{1483, 254, 108, 90, 86},
				{2394, 268, 107, 90, 86},
				{3829, 298, 107, 91, 87},
				{5590, 352, 106, 90, 86},
				{6015, 384, 104, 88, 86},
				{5785, 428, 105, 90, 87},
				{7490, 493, 108, 91, 88},
				{10006, 589, 111, 94, 89},
				{14043, 721, 116, 95, 90},
				{20948, 871, 122, 98, 92},
				{32517, 1040, 129, 100, 91},
				{47789, 1332, 137, 100, 92},
				{62590, 2075, 154, 106, 96},
				{83297, 3644, 202, 120, 105},
				{162126, 13454, 374, 175, 118},
				{-1068449, 201156, 57864, 34160, 44285}
	}
};

static struct pc_temp_ocv_lut pc_temp_ocv = {
	.rows		= 31,
	.cols		= 5,
	.temp		= {-20, 0, 25, 40, 60},
	.percent	= {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 16, 13, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
	.ocv		= {
				{4291, 4304, 4311, 4307, 4301},
				{4193, 4230, 4243, 4242, 4238},
				{4119, 4171, 4186, 4185, 4182},
				{4057, 4117, 4131, 4131, 4128},
				{3986, 4067, 4080, 4079, 4076},
				{3934, 4009, 4032, 4032, 4029},
				{3893, 3952, 3987, 3988, 3985},
				{3856, 3912, 3949, 3950, 3947},
				{3823, 3876, 3910, 3912, 3910},
				{3800, 3847, 3865, 3871, 3871},
				{3783, 3821, 3831, 3831, 3830},
				{3768, 3801, 3808, 3809, 3807},
				{3754, 3785, 3791, 3791, 3789},
				{3740, 3774, 3777, 3777, 3775},
				{3725, 3763, 3768, 3764, 3759},
				{3706, 3747, 3758, 3749, 3736},
				{3680, 3721, 3739, 3730, 3716},
				{3642, 3699, 3713, 3705, 3692},
				{3600, 3689, 3685, 3678, 3666},
				{3563, 3682, 3678, 3671, 3660},
				{3542, 3678, 3675, 3669, 3658},
				{3516, 3673, 3673, 3667, 3656},
				{3484, 3667, 3670, 3665, 3654},
				{3445, 3659, 3666, 3661, 3650},
				{3401, 3644, 3660, 3654, 3643},
				{3352, 3617, 3644, 3638, 3621},
				{3301, 3574, 3607, 3596, 3576},
				{3251, 3512, 3546, 3535, 3516},
				{3203, 3422, 3465, 3455, 3435},
				{3139, 3280, 3344, 3331, 3309},
				{-999000, 3000, 3000, 3000, 3000}
	}
};

struct bms_battery_data tct_4v35_2150mah_byd_rio5 = {/*rio5 cucc and rio5 ctcc using same battery*/
	.fcc				= 2150,
	.fcc_temp_lut			= &fcc_temp,
	.fcc_sf_lut				= &fcc_sf,
	.pc_temp_ocv_lut		= &pc_temp_ocv,
	.rbatt_sf_lut			= &rbatt_sf,
	.default_rbatt_mohm	= 187
};
