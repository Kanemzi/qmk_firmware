#include "layer_config.h"

static uint8_t _current_unicode_input_mode = 0;
static bool _enabled = false;

void on_layer_show_config(void)
{
    _enabled = true;
    _current_unicode_input_mode = get_unicode_input_mode();
}

void on_layer_hide_config(void)
{
    _enabled = false;
}

bool on_layer_render_config(uint8_t led_min, uint8_t led_max)
{
    return false;
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
    }

    return false;
}

void unicode_input_mode_set_user(uint8_t input_mode)
{
    if (!_enabled)
        return;

    _current_unicode_input_mode = input_mode;
}
