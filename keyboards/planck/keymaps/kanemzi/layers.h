#pragma once

#include QMK_KEYBOARD_H

typedef void (*layer_show_callback_t) (void); // Called when the layer becomes the most significant layer
typedef void (*layer_hide_callback_t) (void); // Called when the layer is not the predominant layer anymore (another on top, or layer disabled)
typedef bool (*layer_render_callback_t) (uint8_t led_min, uint8_t led_max); // Called each render update on a part of the leds
typedef bool (*layer_process_record_callback_t) (uint16_t keycode, keyrecord_t *record); // Called when a key even occurs on the layer

typedef struct
{
    bool is_predominant : 1;
    layer_show_callback_t on_layer_show;
    layer_hide_callback_t on_layer_hide;
    layer_render_callback_t on_layer_render;
    layer_process_record_callback_t on_process_record;
} layer_info_t;
