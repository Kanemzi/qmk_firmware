#pragma once

extern float g_brightness_value;
void update_global_brightness(void);

// Sets a light color in the current render pass bounds, with the correct light value
#define RGB_MATRIX_INDICATOR_SET_COLOR_V(i, r, g, b)                                                        \
    if (i >= led_min && i < led_max) {                                                                      \
        rgb_matrix_set_color(i, (r) * g_brightness_value, (g) * g_brightness_value, (b) * g_brightness_value); \
    }
