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
	bool is_replaying: 1; // set to 1 if currently replaying a batch
    uint16_t play_start_time; // used for multi purpose timing (waiting and timing note batches)

    uint16_t current_notes_batch[MI_GAME_BATCH_MAX_LENGTH];
    bool current_is_simultaneous:1;

    uint8_t wrong_inputs[MI_GAME_MAX_MISTAKES];
    uint8_t correct_inputs[MI_GAME_BATCH_MAX_LENGTH];

    midi_game_config_t config;
} midi_game_data_t;

static midi_game_data_t midi_game;

// fills the buffer passed in parameter with a random note batch based on the current game config
static void _generate_random_note_batch(uint16_t* out_batch_buffer, bool* out_is_simultaneous)
{
    // First generate a list of intervals (taking into account ascending/descending config values )

    // Compute the "boundaries" of this interval sequence, then define the index range where it can be placed on the keyboard

    // Pick a random index, and define if the notes should be played simultaneously (max 3)
}

static uint8_t _get_led_index_from_midi_note(uint16_t midi_keycode)
{
    if (midi_keycode < MI_C || midi_keycode > MI_B2) return UINT8_MAX;
	// @todo
}

static void _start_midi_game(void)
{
	if (midi_game.started) return;
	midi_game.started = true;
    srand(timer_read32());

    tap_code16(MI_OC2); // reset octave
	tap_code16(QK_MIDI_ALL_NOTES_OFF);
}

static void _stop_midi_game(void)
{
	if (!midi_game.started) return;
	midi_game.started = false;
}

static void _update_midi_game(uint16_t current_time)
{
    if (!midi_game.started) return;

	if (!midi_game.player_turn) // Wait and play the current note batch
	{
		uint16_t play_elapsed_time = TIMER_DIFF_16(current_time, midi_game.play_start_time);
		if (!midi_game.is_replaying)
		{
			// Wait a bit before playing the batch (if replay requested, just start directly)
			if (play_elapsed_time < MI_GAME_DELAY_BETWEEN_BATCHES_MS) return;
			else play_elasped_time -= MI_GAME_DELAY_BETWEEN_BATCHES_MS;
		}


	}
}

static void _play_notes_batch(bool replay)
{
	midi_game.player_turn = false;
	midi_game.timer = 0;
	midi_game.is_replaying = replay;
	midi_game.play_start_time = timer_read();

	if (!replay) // If not replaying, start a new round
	{
		_generate_random_note_batch(&midi_game.current_notes_batch, &midi_game.current_is_simultaneous);
		memset(wrong_inputs, UINT8_MAX, sizeof(wrong_inputs));
    	memset(correct_inputs, UINT8_MAX, sizeof(correct_inputs));
	}

	tap_code16(QK_MIDI_ALL_NOTES_OFF);
}

static bool _on_process_record_midi_game(uint16_t keycode, keyrecord_t *record)
{
	if (!midi_game.started) return true;
    // @note: don't count correct note again if replaying it

	switch (keycode)
	{
		case MI_OCN2 ... MI_OCTU:
			if (midi_game.started) return false; // Forbid octave variations during the game
			break;
		case KZ_MI_GAME_REPLAY:
			if (record->event.pressed) _play_notes_batch(true);
			return false;
		case KZ_MI_GAME_NEXT:
			if (record->event.pressed) _play_notes_batch(false);
			return false;
	}

    return true;
}

static void _render_midi_game(uint16_t current_time, uint8_t led_min, uint8_t led_max)
{
	if (!midi_game.started) return;

	if (!midi_game.player_turn)
	{
		for (uint8_t i = 38; i < 45; i++)
		{
			RGB_MATRIX_INDICATOR_SET_COLOR(i, 0, 255, 255);
		}
	}

	RGB_MATRIX_INDICATOR_SET_COLOR(45, 255, 0, 0); // Midi Game status led
	RGB_MATRIX_INDICATOR_SET_COLOR(36, 255, 128, 0); // Replay button
	RGB_MATRIX_INDICATOR_SET_COLOR(37, 255, 255, 0); // Next button
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
        _update_midi_game(now);
		// Midi Game rendering logic
		_render_midi_game(now, led_min, led_max);
	}
	else if (midi_game_key_press_time > 0 && TIMER_DIFF_16(now, midi_game_key_press_time) >= MI_GAME_KEY_HOLD_TIME_MS)
	{
		RGB_MATRIX_INDICATOR_SET_COLOR(45, 255, 255, 0); // Midi Game button confirm
	}
}

bool on_process_record_midi(uint16_t keycode, keyrecord_t *record)
{
	if (midi_game.started && !_on_process_record_midi_game(keycode, record)) return false;

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
