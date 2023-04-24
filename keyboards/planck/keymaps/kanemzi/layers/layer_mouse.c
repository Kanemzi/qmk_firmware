#include "layer_mouse.h"

layer_info_t layer_info_mouse =
{
    .is_predominant = false,
    .on_layer_show = on_layer_show_mouse,
    .on_layer_hide = on_layer_hide_mouse,
    .on_layer_render = on_layer_render_mouse,
    .on_process_record = on_process_record_mouse,
    .on_post_process_record = NULL
};

#define WH_MODE_KEYCODE_OFFSET 0x0C

static bool _wheel_mode_enabled = false;

static void _unregister_mouse_keys(bool wheel_mode)
{
	uint8_t offset = wheel_mode ? WH_MODE_KEYCODE_OFFSET : 0;
	for (uint8_t i = 0; i < 4; i++)
		unregister_code16(KC_MS_UP + offset + i);
}

static void _set_wheel_mode(bool enabled)
{
	if (enabled == _wheel_mode_enabled) return;
	_wheel_mode_enabled = enabled;
	_unregister_mouse_keys(!enabled);
}

void on_layer_show_mouse(void)
{
	_wheel_mode_enabled = true;
}

void on_layer_hide_mouse(void)
{
	_wheel_mode_enabled = false;

	// Unregister all mouse keys when hiding the layer
	_unregister_mouse_keys(true);
	_unregister_mouse_keys(false);
}

void on_layer_render_mouse(uint8_t led_min, uint8_t led_max)
{
	if (_wheel_mode_enabled)
	{
		// Indicators for wheel enabled
		RGB_MATRIX_INDICATOR_SET_COLOR(40, 255, 255, 255);
		RGB_MATRIX_INDICATOR_SET_COLOR(42, 255, 255, 255);

		// Indicator for wheel direction keys
		RGB_MATRIX_INDICATOR_SET_COLOR(8,  255, 255, 255);
		RGB_MATRIX_INDICATOR_SET_COLOR(19, 255, 255, 255);
		RGB_MATRIX_INDICATOR_SET_COLOR(20, 255, 255, 255);
		RGB_MATRIX_INDICATOR_SET_COLOR(21, 255, 255, 255);
	}
}

bool on_process_record_mouse(uint16_t keycode, keyrecord_t *record)
{
	switch (keycode)
	{
		case KZ_MS_WHEEL_MODE:
			_set_wheel_mode(record->event.pressed);
			return false;
		case KC_MS_UP ... KC_MS_RIGHT:
			if (_wheel_mode_enabled)
			{
				uint16_t const new_keycode = keycode + WH_MODE_KEYCODE_OFFSET;
				if (record->event.pressed) register_code16(new_keycode);
				else unregister_code16(new_keycode);
				return false;
			}
			break;
	}
	return true;
}
