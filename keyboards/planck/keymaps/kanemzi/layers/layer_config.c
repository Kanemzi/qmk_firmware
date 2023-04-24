#include "layer_config.h"

layer_info_t layer_info_config =
{
    .is_predominant = false,
    .on_layer_show = on_layer_show_config,
    .on_layer_hide = on_layer_hide_config,
    .on_layer_render = on_layer_render_config,
    .on_process_record = on_process_record_config,
    .on_post_process_record = on_post_process_record_config
};

typedef enum
{
    ConfigRGB =     1 << 0,
    ConfigNKRO =    1 << 1,
    ConfigUnicode = 1 << 2,
} config_space_t;

static uint8_t _config_space_dirty_flags;

static RBG _preview_colors[3];

void on_layer_show_config(void)
{
    dprintf("[Config] Show layer");
}

void on_layer_hide_config(void)
{
    dprintf("[Config] Hide layer");
}

void on_layer_render_config(uint8_t led_min, uint8_t led_max)
{
	if (keymap_config.nkro)
	{
		RGB_MATRIX_INDICATOR_SET_COLOR(26, 0, 255, 0); // NKRO Enabled indicator
	}

	if (debug_enable)
	{
		RGB_MATRIX_INDICATOR_SET_COLOR(27, 0, 255, 0); // Debug Enabled indicator
	}

    if (user_config.windows_unicode_fallback)
    {
        RGB_MATRIX_INDICATOR_SET_COLOR(28, 255, 0, 0); // Unicode fallback indicator
    }
    else switch (get_unicode_input_mode()) // Display unicode input method
	{
		case UNICODE_MODE_WINDOWS:
			RGB_MATRIX_INDICATOR_SET_COLOR(28, 0, 0, 255);
			break;
		case UNICODE_MODE_WINCOMPOSE:
			RGB_MATRIX_INDICATOR_SET_COLOR(28, 0, 255, 255);
			break;
		case UNICODE_MODE_LINUX:
			RGB_MATRIX_INDICATOR_SET_COLOR(28, 255, 124, 77);
			break;
		case UNICODE_MODE_MACOS:
			RGB_MATRIX_INDICATOR_SET_COLOR(28, 255, 128, 191);
			break;
	}
}

bool on_process_record_config(uint16_t keycode, keyrecord_t *record)
{
	switch (keycode)
	{
		case KZ_EXIT:
			if (record->event.pressed)
			{
				layer_off(L_CONFIG);
				return false;
			}
			break;

        case UC_NEXT:
			if (record->event.pressed)
			{
				if (get_mods() & MOD_MASK_SHIFT)
				{
					user_config.windows_unicode_fallback ^= 1;
					return false;
				}
				else
				{
					if (user_config.windows_unicode_fallback)
					{
						user_config.windows_unicode_fallback = false;
						return false;
					}
				}
			}
            break;
/*
		case NK_TOGG:
			if (record->event.pressed)
			{
				_nkro_enabled = keymap_config.nkro;
				return true;
			}
			break;*/
	}
	return true;
}

void on_post_process_record_config(uint16_t keycode, keyrecord_t *record)
{
	switch (keycode)
	{

	}
}
