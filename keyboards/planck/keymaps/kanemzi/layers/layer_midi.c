#include "layer_midi.h"

layer_info_t layer_info_midi =
{
    .is_predominant = false,
    .on_layer_show = on_layer_show_midi,
    .on_layer_hide = on_layer_hide_midi,
    .on_layer_render = NULL,
    .on_process_record = on_process_record_midi
};

void on_layer_show_midi(void)
{
	tap_code16(MI_ON);
#ifdef CONSOLE_ENABLE
    dprintf("[Midi] Show layer");
#endif
}

void on_layer_hide_midi(void)
{
	tap_code16(MI_OFF);
#ifdef CONSOLE_ENABLE
    dprintf("[Midi] Hide layer");
#endif
}

bool on_process_record_midi(uint16_t keycode, keyrecord_t *record)
{
	switch (keycode)
	{
		case KZ_EXIT:
			if (record->event.pressed)
			{
				layer_off(L_MIDI);
				return false;
			}
	}
	return true;
}