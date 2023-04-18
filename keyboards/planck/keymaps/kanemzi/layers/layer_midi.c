#include "layer_midi.h"

typedef struct
{
	bool started : 1;
} midi_game_data_t;

static midi_game_data_t midi_game;
static uint16_t midi_game_key_press_time = 0;

layer_info_t layer_info_midi =
{
    .is_predominant = false,
    .on_layer_show = on_layer_show_midi,
    .on_layer_hide = on_layer_hide_midi,
    .on_layer_render = on_layer_render_midi,
    .on_process_record = on_process_record_midi
};

static void _start_midi_game(void)
{
	if (midi_game.started) return;

	midi_game.started = true;
}

static void _stop_midi_game(void)
{
	if (!midi_game.started) return;

	midi_game.started = false;
}

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
	_stop_midi_game();
	midi_game_key_press_time = 0;
#ifdef CONSOLE_ENABLE
    dprintf("[Midi] Hide layer");
#endif
}

void on_layer_render_midi(uint8_t led_min, uint8_t led_max)
{
	uint16_t now = timer_read();

	if (midi_game.started)
	{
		// Midi Game rendering logic
		RGB_MATRIX_INDICATOR_SET_COLOR(45, 255, 0, 0);
	}
	else if (midi_game_key_press_time > 0 && TIMER_DIFF_16(now, midi_game_key_press_time) >= MI_GAME_KEY_HOLD_TIME_MS)
	{
		RGB_MATRIX_INDICATOR_SET_COLOR(45, 255, 255, 0);
	}
}

bool on_process_record_midi(uint16_t keycode, keyrecord_t *record)
{
	switch (keycode)
	{
		case KZ_MI_GAME:
			if (!midi_game.started) // needs to hold the key to start game (to avoid missclicks)
			{
				if (record->event.pressed)
					midi_game_key_press_time = record->event.time;
				else
				{
					if (TIMER_DIFF_16(record->event.time, midi_game_key_press_time) >= MI_GAME_KEY_HOLD_TIME_MS)
						_start_midi_game();
					midi_game_key_press_time = 0;
				}
			}
			else if (!record->event.pressed) // instantly stops the game when releasing the key again
			{
				_stop_midi_game();
				midi_game_key_press_time = 0;
			}
			break;
		case KZ_EXIT:
			if (record->event.pressed)
			{
				layer_off(L_MIDI);
				return false;
			}
			break;
	}
	return true;
}
