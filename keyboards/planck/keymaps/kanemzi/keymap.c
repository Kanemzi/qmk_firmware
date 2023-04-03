#include QMK_KEYBOARD_H

#include "layer_base.h"
#include "layer_lower.h"
#include "layer_raise.h"
#include "layer_hub.h"
#include "layer_media.h"
#include "layer_mouse.h"
#include "layer_midi.h"
#include "layer_config.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[L_BASE] = LAYER_BASE_GRID,
	[L_LOWER] = LAYER_LOWER_GRID,
	[L_RAISE] = LAYER_RAISE_GRID,
	[L_HUB] = LAYER_HUB_GRID,
	[L_MEDIA] = LAYER_MEDIA_GRID,
	[L_MOUSE] = LAYER_MOUSE_GRID,
	[L_MIDI] = LAYER_MIDI_GRID,
	[L_CONFIG] = LAYER_CONFIG_GRID
};

const uint8_t PROGMEM ledmap[][RGB_MATRIX_LED_COUNT][3] = {
	[L_BASE] = LAYER_BASE_LEDMAP,
	[L_LOWER] = LAYER_LOWER_LEDMAP,
	[L_RAISE] = LAYER_RAISE_LEDMAP,
	[L_HUB] = LAYER_HUB_LEDMAP,
	[L_MEDIA] = LAYER_MEDIA_LEDMAP,
	[L_MOUSE] = LAYER_MOUSE_LEDMAP,
	[L_MIDI] = LAYER_MIDI_LEDMAP,
	[L_CONFIG] = LAYER_CONFIG_LEDMAP
};


static inline void set_layer_color(uint8_t led_min, uint8_t led_max, int layer)
{
	// RGB_MATRIX_INDICATOR_SET_COLOR(index, red, green, blue)
	for (uint16_t i = led_min; i < led_max; i++)
	{
		const uint8_t r = pgm_read_byte(&ledmap[layer][i][0]);
		const uint8_t g = pgm_read_byte(&ledmap[layer][i][1]);
		const uint8_t b = pgm_read_byte(&ledmap[layer][i][2]);

		if (!r && !g && !b)
		{
		    rgb_matrix_set_color(i, 0, 0, 0);
		}
		else
		{
		    float lum = (float) rgb_matrix_config.hsv.v / UINT8_MAX;
		    rgb_matrix_set_color(i, lum * r, lum * g, lum * b);
		}
	}
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
	set_layer_color(led_min, led_max, get_highest_layer(layer_state | default_layer_state));
    return false;
}

layer_state_t layer_state_set_user(layer_state_t state)
{
	return update_tri_layer_state(state, L_LOWER, L_RAISE, L_HUB);
}

void keyboard_post_init_user(void)
{
    rgb_matrix_enable();

}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	/*witch (keycode)
	{

	}*/
	return true;
}
