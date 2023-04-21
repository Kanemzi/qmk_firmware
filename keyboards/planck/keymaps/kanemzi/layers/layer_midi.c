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
    midi_game_config_t config;

	bool started : 1;
    bool player_turn: 1;
	bool is_replaying: 1; // set to 1 if currently replaying a batch

    // Generated batch info
    uint16_t current_notes_batch[MI_GAME_BATCH_MAX_LENGTH];
    bool current_is_simultaneous;

    // Play batch
    uint16_t _note_duration;
    uint16_t _delay_between_notes;
    uint16_t play_start_time; // used for multi purpose timing (waiting and timing note batches)
    uint16_t played_notes_mask;

    // Player input handling
    uint8_t next_guess_index; // The index of the next note to find in the batch (used as a counter for simultaneous batches)
    uint8_t wrong_input_count;
    uint8_t wrong_inputs[MI_GAME_MAX_MISTAKES];
} midi_game_data_t;

static midi_game_data_t midi_game;

// fills the buffer passed in parameter with a random note batch based on the current game config
static void _generate_random_note_batch(uint16_t* out_batch_buffer, bool* out_is_simultaneous)
{
    // @note : can't pick the same note multiple time in the same batch

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
			else play_elapsed_time -= MI_GAME_DELAY_BETWEEN_BATCHES_MS;
		}

        // Play and release the notes in the current batch
        for (uint8_t i = 0; i < MI_GAME_BATCH_MAX_LENGTH && midi_game.current_notes_batch[i] != 0; i++)
        {
            const uint16_t note = midi_game.current_notes_batch[i];
            const uint16_t note_start_time = i * midi_game._delay_between_notes;
            const uint16_t note_stop_time = note_start_time + midi_game._note_duration;

            const bool is_currently_played = midi_game.played_notes_mask & (1 << i);
            const bool should_be_played = play_elapsed_time >= note_start_time && play_elapsed_time <= note_stop_time;

            if (is_currently_played != should_be_played) // The note must be updated
            {
                if (should_be_played) register_code16(note);
                else unregister_code16(note);
                // Update the played note mask state by overriding the bit corresponding to the note
                midi_game.played_notes_mask = (midi_game.played_notes_mask & ~(1 << i)) | ((should_be_played & 1) << i);
                dprintf("[Midi Game] played_notes_mask update : 0x%04x", midi_game.played_notes_mask);
            }
        }

        // Detect when we are done playing the batch
        if (play_elapsed_time >= midi_game._note_duration && midi_game.played_notes_mask)
        {
            midi_game.player_turn = true;
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
        midi_game.wrong_input_count = 0;
		memset(midi_game.wrong_inputs, UINT8_MAX, sizeof(midi_game.wrong_inputs));
    	memset(midi_game.correct_inputs, UINT8_MAX, sizeof(midi_game.correct_inputs));
        midi_game._note_duration = midi_game.current_is_simultaneous ? MI_GAME_SIMULTANEOUS_NOTE_DURATION_MS : MI_GAME_NOTE_DURATION_MS;

        // Auto guess the first note (as a reference pitch), it's allowed to press it but optional
        midi_game.next_guess_index = midi_game.correct_input_count = 1;
        midi_game.correct_inputs[0] = _get_led_index_from_midi_note(midi_game.current_notes_batch[0]);
    }

    if (!midi_game.current_is_simultaneous)
    {
        const uint16_t bpm = ((float) midi_game.config.tempo / (MI_GAME_TEMPO_STEPS - 1)) * (MI_GAME_MAX_TEMPO - MI_GAME_MIN_TEMPO) + MI_GAME_MIN_TEMPO;
        midi_game._delay_between_notes = 60000 / bpm;
        dprintf("[Midi Game] Playing note batch at %dBPM (%dms)", bpm, midi_game._delay_between_notes);
    }
    else
    {
        midi_game._delay_between_notes = 0;
        dprintf("[Midi Game] Playing simultaneous notes batch");
    }

    midi_game.played_notes_mask = 0;
	tap_code16(QK_MIDI_ALL_NOTES_OFF);
}

static bool _on_process_record_midi_game(uint16_t keycode, keyrecord_t *record)
{
	if (!midi_game.started) return true;

	switch (keycode)
	{
		case MI_OCN2 ... MI_OCTU:
            return false; // Forbid octave variations during the game
		case KZ_MI_GAME_REPLAY:
			if (record->event.pressed) _play_notes_batch(true);
			return false;
		case KZ_MI_GAME_NEXT:
			if (record->event.pressed) _play_notes_batch(false);
			return false;
        case MI_C ... MI_B2:
            if (!midi_game.player_turn) return false;

            break; // @note: don't count correct note again if replaying it

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
			RGB_MATRIX_INDICATOR_SET_COLOR(i, 0, 255, 255); // Red bar is displayed when the batch is playing
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
    midi_game.config.tempo = 6;
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
