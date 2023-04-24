#pragma once

extern float g_brightess_value;

// Sets a light color in the current render pass bounds, with the correct light value
#define RGB_MATRIX_INDICATOR_SET_COLOR_V(i, r, g, b)                                                  \
    if (i >= led_min && i < led_max) {                                                                \
        rgb_matrix_set_color(i, r * g_brightess_value, g * g_brightess_value, b * g_brightess_value); \
    }
