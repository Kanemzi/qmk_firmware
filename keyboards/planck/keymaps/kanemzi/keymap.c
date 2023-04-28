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

#include "kanemzi_utils.h"

#include "sendstring_french.h"

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
	[L_MOUSE] = &layer_info_mouse,
	[L_MIDI] = &layer_info_midi,
	[L_CONFIG] = &layer_info_config
};

planck_layers predominant_layer = L_BASE;
user_config_t user_config;

const uint32_t PROGMEM unicode_map[] = {
    [GMTL] =    0x00ab, // «
    [GMTR] =    0x00bb, // »
    [HCADR] =   0x2013, // –
    [CADR] =    0x2014, // —
    [MIDDOT] =  0x00b7, // ·
    [EXP0] =    0x2070, // ⁰
    [EXP1] =    0x00b9, // ¹
    [EXP3] =    0x00b3, // ³
    [EXP4] =    0x2074, // ⁴
    [EXP5] =    0x2075, // ⁵
    [EXP6] =    0x2076, // ⁶
    [EXP7] =    0x2077, // ⁷
    [EXP8] =    0x2078, // ⁸
    [EXP9] =    0x2079, // ⁹
    [AGRVCAP] = 0x00c0, // À
    [EACUCAP] = 0x00c9, // É
    [EGRVCAP] = 0x00c8, // È
    [UGRVCAP] = 0x00d9, // Ù
    [CCEDCAP] = 0x00c7, // Ç
    [OE] =      0x0153, // œ
    [OECAP] =   0x0152, // Œ
    [TM] =      0x2122 // ™
};

const uint8_t PROGMEM windows_unicode_fallback_map[][3] = {
    [GMTL] =    {1, 7, 1}, // «
    [GMTR] =    {1, 8, 7}, // »
    [HCADR] =   {1, 5, 0}, // –
    [CADR] =    {1, 5, 1}, // —
    [MIDDOT] =  {1, 8, 3}, // ·
    [EXP0] =    ALT_NO_SEQ, // ⁰
    [EXP1] =    {1, 8, 5}, // ¹
    [EXP3] =    {1, 7, 9}, // ³
    [EXP4] =    ALT_NO_SEQ, // ⁴
    [EXP5] =    ALT_NO_SEQ, // ⁵
    [EXP6] =    ALT_NO_SEQ, // ⁶
    [EXP7] =    ALT_NO_SEQ, // ⁷
    [EXP8] =    ALT_NO_SEQ, // ⁸
    [EXP9] =    ALT_NO_SEQ, // ⁹
    [AGRVCAP] = {1, 9, 2}, // À
    [EACUCAP] = {2, 0, 1}, // É
    [EGRVCAP] = {2, 0, 0}, // È
    [UGRVCAP] = {2, 1, 7}, // Ù
    [CCEDCAP] = {1, 9, 9}, // Ç
    [OE] =      {1, 5, 6}, // œ
    [OECAP] =   {1, 4, 0}, // Œ
    [TM] =      {1, 5, 3} // ™
};

#define CUSTOM_SHIFT_KEYS_COUNT 19
const custom_shift_key_t PROGMEM custom_shift_keys[] = {
	{FR_AGRV, X(AGRVCAP)},
	{FR_EACU, X(EACUCAP)},
	{FR_EGRV, X(EGRVCAP)},
	{FR_UGRV, X(UGRVCAP)},
	{FR_CCED, X(CCEDCAP)},

	{FR_QUOT, X(TM)},
	{FR_LABK, X(GMTL)},
	{FR_RABK, X(GMTR)},

	{X(HCADR), X(CADR)},

	{KC_KP_0, X(EXP0)},
	{KC_KP_1, X(EXP1)},
	{KC_KP_2, FR_SUP2},
	{KC_KP_3, X(EXP3)},
	{KC_KP_4, X(EXP4)},
	{KC_KP_5, X(EXP5)},
	{KC_KP_6, X(EXP6)},
	{KC_KP_7, X(EXP7)},
	{KC_KP_8, X(EXP8)},
	{KC_KP_9, X(EXP9)}
};

static inline void set_layer_color(uint8_t led_min, uint8_t led_max, int layer)
{
	for (uint16_t i = led_min; i < led_max; i++)
	{
		const uint8_t r = pgm_read_byte(&ledmap[layer][i][0]);
		const uint8_t g = pgm_read_byte(&ledmap[layer][i][1]);
		const uint8_t b = pgm_read_byte(&ledmap[layer][i][2]);

		if (!r && !g && !b)
			rgb_matrix_set_color(i, 0, 0, 0);
		else
		    rgb_matrix_set_color(i, g_brightness_value * r, g_brightness_value * g, g_brightness_value * b);
	}
}

bool rgb_matrix_indicators_user(void)
{
	update_global_brightness();

    // Add a blinking led when the debug mode is enabled
    static bool debug_led_state = false;
    if (debug_enable)
    {
        uint16_t now = timer_read();
        bool expected_state = now % (DEBUG_LED_BLINK_SPEED * 2) <  DEBUG_LED_BLINK_SPEED;
        if (debug_led_state != expected_state)
        {
            debug_led_state = expected_state;
            if (debug_led_state) planck_ez_left_led_on();
            else planck_ez_left_led_off();
        }
    }
    else if (debug_led_state)
    {
        debug_led_state = false;
        planck_ez_left_led_off();
    }

	return true;
}

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

    if (user_config.use_alt_lower_layer && new_predominant_layer == L_LOWER)
    {
        new_predominant_layer = L_ALT_LOWER;
    }

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
	user_config.windows_unicode_fallback = true; // Enabled by default until the value is saved in EEPROM
    user_config.use_alt_lower_layer = true;
    rgb_matrix_enable();

	planck_ez_left_led_level(48);
	planck_ez_left_led_off();
	update_global_brightness();
}

void send_windows_alt_sequence(uint16_t keycode)
{
	const uint16_t index = unicodemap_index(keycode);
	const uint8_t first_digit = windows_unicode_fallback_map[index][0];
	if (first_digit == ALT_SEQ_END)
	{
		#ifdef CONSOLE_ENABLE
    		dprintf("No Alt Sequence fallback for index %d):", index);
		#endif
		return;
	}

	const uint8_t saved_mods = get_mods();
	clear_mods();
	register_code(KC_LALT);
	#ifdef CONSOLE_ENABLE
    	dprintf("Starting Alt Sequence (index: %d):", index);
	#endif
	tap_code(KC_KP_0); // First sequence digit is always 0
	for (size_t i = 0; i < 3 ; i++)
	{
		const uint8_t digit = windows_unicode_fallback_map[index][i];
		#ifdef CONSOLE_ENABLE
			dprintf("%d ", digit);
		#endif
		if (digit == ALT_SEQ_END)
			break;

		uint16_t keycode = KC_KP_0;
		if (digit > 0)
			keycode = KC_KP_1 + digit - 1;

		tap_code(keycode);
	}
	unregister_code(KC_LALT);
	#ifdef CONSOLE_ENABLE
    	dprintf("\n");
	#endif
	set_mods(saved_mods);
}

bool process_custom_shift_keys(uint16_t keycode, keyrecord_t* record)
{
	static uint16_t registered_keycode = KC_NO;

	if (registered_keycode != KC_NO)
	{
		unregister_code16(registered_keycode);
		registered_keycode = KC_NO;
	}

	if (!record->event.pressed)
		return true;

	const uint8_t mods = get_mods();

	if ((mods | get_weak_mods()) & MOD_MASK_SHIFT)
	{
		// if ((IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) && record->tap.count == 0)
        //		return true;

		for (uint8_t i = 0; i < CUSTOM_SHIFT_KEYS_COUNT; i++)
		{
			if (keycode == custom_shift_keys[i].keycode)
			{
				#ifdef CONSOLE_ENABLE
					dprintf("Detected custom shift key");
				#endif
				registered_keycode = custom_shift_keys[i].shifted_keycode;

				if (IS_QK_MODS(registered_keycode) && (QK_MODS_GET_MODS(registered_keycode) & MOD_LSFT) != 0)
				{
					register_code16(registered_keycode);
				}
				else
				{
					del_weak_mods(MOD_MASK_SHIFT);
					unregister_mods(MOD_MASK_SHIFT);
					if (IS_QK_UNICODEMAP(registered_keycode))
					{
						if (user_config.windows_unicode_fallback)
						{
							send_windows_alt_sequence(registered_keycode);
						}
						else
						{
							process_unicodemap(registered_keycode, record);
						}
					}
					else
					{
						register_code16(registered_keycode);
					}
					set_mods(mods);
				}
				return false;
			}
		}
	}
	return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
	if (!process_custom_shift_keys(keycode, record))
		return false;

	if (IS_QK_UNICODEMAP(keycode) && record->event.pressed)
	{
		if (user_config.windows_unicode_fallback)
		{
			send_windows_alt_sequence(keycode);
			return false;
		}
	}

	// Per layer custom record handling
	layer_info_t* predominant_layer_info = layers_info[predominant_layer];
	if (predominant_layer_info != NULL && predominant_layer_info->on_process_record != NULL)
	{
		if (!predominant_layer_info->on_process_record(keycode, record))
		{
			return false;
		}
	}

	switch (keycode)
	{
		case KZ_ARROW: if (record->event.pressed) SEND_STRING("->"); return false;
	}

	return true;
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record)
{
    // Per layer custom post record handling
	layer_info_t* predominant_layer_info = layers_info[predominant_layer];
	if (predominant_layer_info != NULL && predominant_layer_info->on_post_process_record != NULL)
	{
		predominant_layer_info->on_post_process_record(keycode, record);
	}
}

