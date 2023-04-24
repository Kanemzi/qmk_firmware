#include "kanemzi_utils.h"
#include "constants.h"

float g_brightness_value = 1.0f;

inline void update_global_brightness(void)
{
	g_brightness_value = (float) rgb_matrix_config.hsv.v / UINT8_MAX;
}