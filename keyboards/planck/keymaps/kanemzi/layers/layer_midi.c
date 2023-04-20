#include "layer_midi.h"

static uint16_t midi_game_key_press_time = 0;

layer_info_t layer_info_midi =
{
    .is_predominant = false,
    .on_layer_show = on_layer_show_midi,
    .on_layer_hide = on_layer_hide_midi,
    .on_layer_render = on_layer_render_midi,
    .on_process_record = on_process_record_midi
};

#pragma region MIDI_GAME

typedef struct
{
	bool started : 1;
    bool player_turn: 1;
    uint16_t timer; // multi purpose timer (used for delays and timing note batches)

    uint16_t current_notes_batch[MI_GAME_BATCH_MAX_LENGTH];
    bool current_is_simultaneous:1;

    uint8_t wrong_inputs[MI_GAME_MAX_MISTAKES];
    uint8_t correct_inputs[MI_GAME_BATCH_MAX_LENGTH];

    uint8_t
    midi_game_config_t config;
} midi_game_data_t;

static midi_game_data_t midi_game;

static void _start_midi_game(void)
{
	if (midi_game.started) return;
	midi_game.started = true;
    srand(timer_read32());
    tap_code16(MI_OC2); // reset octave
}

static void _stop_midi_game(void)
{
	if (!midi_game.started) return;
	midi_game.started = false;

}

static void _update_midi_game(void)
{
    if (!midi_game.started) return;


}

static bool _on_process_record_midi_game(uint16_t keycode, keyrecord_t *record)
{
    // @note: don't count correct note again if replaying it
    return false;
}

// fills the buffer passed in parameter with a random note batch based on the current game config
static void _generate_random_note_batch(uint16_t* out_batch_buffer, bool* out_is_simultaneous)
{
    // First generate a list of intervals (taking into account ascending/descending config values )

    // Compute the "boundaries" of this interval sequence, then define the index range where it can be placed on the keyboard

    // Pick a random index, and define if the notes should be played simultaneously (max 3)
}

static uint8_t _get_led_index_from_midi_note(uint16_t midi_keycode)
{
    if (midi_keycode < MI_C || midi_keycode > MI_B2) return 0xff;
}

#pragma endregion MIDI_GAME

void on_layer_show_midi(void)
{
	tap_code16(MI_ON);
#ifdef CONSOLE_ENABLE
    dprintf("[Midi] Show layer");
#endif

    // Setup default configuration for the Midi Game
    midi_game.config.tempo = 7;
	midi_game.config.notes_batch_length = 2;
	midi_game.config.allowed_batch_types = ASCENDING | DESCENDING | SIMULTANEOUS;
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
        // Update the Midi Game state before renreding
        // @todo could be done in an on_update callback
        _update_midi_game();
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
        case MI_OCN2 ... MI_OCTU:
            if (midi_game.started) return false; // Forbid octave variations during the game
            break;
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
