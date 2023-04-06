#include QMK_KEYBOARD_H

#include "layers/layers.h"
#include "layers/layer_base.h"
#include "layers/layer_lower.h"
#include "layers/layer_raise.h"
#include "layers/layer_hub.h"
#include "layers/layer_media.h"
#include "layers/layer_mouse.h"
#include "layers/layer_midi.h"
#include "layers/layer_config.h"

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

layer_info_t* layers_info[] = {
    [L_BASE] = NULL,
	[L_LOWER] = NULL,
	[L_RAISE] = NULL,
	[L_HUB] = NULL,
	[L_MEDIA] = NULL,
	[L_MOUSE] = NULL,
	[L_MIDI] = &layer_info_midi,
	[L_CONFIG] = &layer_info_config
};

planck_layers predominant_layer = L_BASE;
user_config_t user_config;

static inline void set_layer_color(uint8_t led_min, uint8_t led_max, int layer)
{
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

/*
void rgb_matrix_indicators_user(void)
{
    if (keyboard_config.disable_layer_led)
        return;
}
*/

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max)
{
	if (keyboard_config.disable_layer_led)
		return false;

	// Default layer coloration
	set_layer_color(led_min, led_max, predominant_layer);

	// Layers custom dynamic rendering
	layer_info_t* predominant_layer_info = layers_info[predominant_layer];
	if (predominant_layer_info != NULL && predominant_layer_info->on_layer_render != NULL)
	{
		predominant_layer_info->on_layer_render(led_min, led_max);
	}

	return true;
}

layer_state_t layer_state_set_user(layer_state_t state)
{
	layer_state_t new_state = update_tri_layer_state(state, L_LOWER, L_RAISE, L_HUB);
	planck_layers new_predominant_layer = get_highest_layer(new_state | default_layer_state);

	// If the current predominant layer changed
	if (new_predominant_layer != predominant_layer)
	{
		layer_info_t* const predominant_layer_info = layers_info[predominant_layer];
		if (predominant_layer_info != NULL)
		{
			predominant_layer_info->is_predominant = false;
			if (predominant_layer_info->on_layer_hide != NULL)
			{
				predominant_layer_info->on_layer_hide();
			}
		}

		layer_info_t* const new_predominant_layer_info = layers_info[new_predominant_layer];
		if (new_predominant_layer_info != NULL)
		{
			new_predominant_layer_info->is_predominant = true;
			if (new_predominant_layer_info->on_layer_show != NULL)
			{
				new_predominant_layer_info->on_layer_show();
			}
		}

		predominant_layer = new_predominant_layer;
	}

	return new_state;
}

void keyboard_post_init_user(void)
{
    user_config.raw = eeconfig_read_user();
    rgb_matrix_enable();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
	// Per layer custom record handling
	layer_info_t* predominant_layer_info = layers_info[predominant_layer];
	if (predominant_layer_info != NULL && predominant_layer_info->on_process_record != NULL)
	{
		if (!predominant_layer_info->on_process_record(keycode, record))
		{
			return false;
		}
	}

	return true;
}
