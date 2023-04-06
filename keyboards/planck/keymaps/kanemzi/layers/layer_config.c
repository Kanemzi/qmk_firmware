#include "layer_config.h"

layer_info_t layer_info_config =
{
    .is_predominant = false,
    .on_layer_show = on_layer_show_config,
    .on_layer_hide = on_layer_hide_config,
    .on_layer_render = on_layer_render_config,
    .on_process_record = on_process_record_config
};

static uint8_t _current_unicode_input_mode = 0;

void on_layer_show_config(void)
{
    _current_unicode_input_mode = get_unicode_input_mode();
#ifdef CONSOLE_ENABLE
    dprintf("[Config] Show layer");
#endif
}

void on_layer_hide_config(void)
{
#ifdef CONSOLE_ENABLE
    dprintf("[Config] Hide layer");
#endif
}

void on_layer_render_config(uint8_t led_min, uint8_t led_max)
{
	if (keymap_config.nkro)
	{
		RGB_MATRIX_INDICATOR_SET_COLOR(26, 0, 255, 0);
	}

    if (user_config.windows_unicode_fallback)
    {
        RGB_MATRIX_INDICATOR_SET_COLOR(28, 255, 0, 0);
    }
    else switch (_current_unicode_input_mode)
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
            if ((get_mods() | get_weak_mods()) & MOD_MASK_SHIFT)
            {
                user_config.windows_unicode_fallback = true;
                return false;
            }
            else
            {
                user_config.windows_unicode_fallback = false;
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

void unicode_input_mode_set_user(uint8_t input_mode)
{
    if (!layer_info_config.is_predominant)
        return;

    _current_unicode_input_mode = input_mode;
}
