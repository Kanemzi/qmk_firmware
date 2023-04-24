#include "layer_midi.h"

#include "../kanemzi_utils.h"

static uint16_t _midi_game_key_press_time = 0;
extern MidiDevice midi_device;

layer_info_t layer_info_midi =
{
    .is_predominant = false,
    .on_layer_show = on_layer_show_midi,
    .on_layer_hide = on_layer_hide_midi,
    .on_layer_render = on_layer_render_midi,
    .on_process_record = on_process_record_midi,
    .on_post_process_record = NULL
};

typedef struct
{
    midi_game_config_t config;

	bool started : 1;
    bool player_turn: 1;
	bool is_replaying: 1; // set to 1 if currently replaying a batch
	bool has_won: 1; // player got correct answer on last round

    // Generated batch info
    uint16_t current_notes_batch[MI_GAME_BATCH_MAX_LENGTH];
    bool current_is_simultaneous;

    // Play batch
    uint16_t _note_duration;
    uint16_t _delay_between_notes;
    uint16_t play_start_time; // used for multi purpose timing (waiting and timing note batches)
    uint16_t played_notes_mask;
	bool has_muted_before_play: 1;

    // Player input handling
    uint8_t guessed_notes_count;
	uint16_t guessed_notes_mask; // Bit is set to 1 when the note was found
    uint8_t wrong_notes_count;
    uint8_t wrong_notes_indexes[MI_GAME_MAX_MISTAKES];
	uint16_t next_round_time; // When != 0, will skip to next round at a specific time
} midi_game_data_t;

typedef enum
{
	Invalid, // New invalid guess
	Valid, // New valid guess
	InvalidRepeat, // An invalid guess that was already registered
	ValidRepeat, // A valid guess that was already registered
} midi_game_guess_validity_t;

static midi_game_data_t midi_game;
static uint8_t _first_note_led_index = UINT8_MAX;

#define LOWEST_NOTE MI_C
#define HIGHEST_NOTE MI_B2

// checks if a given interval would cause an interval sequence to go twice on the same note
static inline bool _ensure_no_double_notes(int8_t const intervals[MI_GAME_BATCH_MAX_LENGTH - 1], uint8_t seq_length, int8_t interval)
{
	int8_t offset = interval;
	for (uint8_t i = seq_length - 1; i > 0; i--)
	{
		offset += intervals[i];
		if (offset == 0) return false;
	}
	return true;
}

// fills the buffer passed in parameter with a random note batch based on the current game config
static void _generate_random_note_batch(uint16_t out_batch_buffer[MI_GAME_BATCH_MAX_LENGTH], bool* out_is_simultaneous)
{
	// simultaneous notes is forbidden on a >= 3 notes config
	bool is_simultaneous = midi_game.config.notes_batch_length <= 3
		&& (rand() % 100) < MI_GAME_SIMULTANEOUS_PROBABILITY;
	*out_is_simultaneous = is_simultaneous;

	dprintf("[Midi Game] Start generating batch of size %d, (simultaneous: %d)\n", midi_game.config.notes_batch_length, is_simultaneous);

	int8_t lowest_offset = 0, highest_offset = 0;
	int8_t intervals[MI_GAME_BATCH_MAX_LENGTH - 1] = {0};

	uint8_t intervals_count = 0;
	int8_t current_offset = 0;

	while (intervals_count < midi_game.config.notes_batch_length - 1)
	{
		int8_t const amplitude = highest_offset - lowest_offset;
		int8_t const covered_down = abs(lowest_offset - current_offset);
		int8_t const covered_up = abs(highest_offset - current_offset);

		bool interval_valid = false;
		int8_t interval;
		do
		{
			int8_t const max_interval_up = MIN(MI_GAME_MAX_INTERVAL, MI_GAME_MAX_BATCH_AMPLITUDE - amplitude + covered_up);
			int8_t const max_interval_down = -MIN(MI_GAME_MAX_INTERVAL, MI_GAME_MAX_BATCH_AMPLITUDE - amplitude + covered_down);
			int8_t const interval_range = max_interval_up - max_interval_down;

			interval = max_interval_down + rand() % interval_range;
			if (interval >= 0) interval++; // Avoid 0 interval, also allows reaching max_interval_up

			interval_valid = !is_simultaneous || _ensure_no_double_notes(intervals, intervals_count, interval);
		} while (!interval_valid);

		current_offset += interval;
		if (current_offset < lowest_offset) lowest_offset = current_offset;
		if (current_offset > highest_offset) highest_offset = current_offset;

		dprintf("[Midi Game]\tInterval %d: %d\tCoverage [%d; %d]\n", intervals_count, interval, lowest_offset, highest_offset);

		intervals[intervals_count] = interval;
		intervals_count++;
	}

	// Second step : compute the range of starting notes that could make the intervals sequence fit on the keyboard
	uint16_t lowest_note = LOWEST_NOTE - lowest_offset; // lowest offset will always be negative
	uint16_t highest_note = HIGHEST_NOTE - highest_offset;

	// Last step : pick a random index from the previous range, then compute the notes that follows the intervals sequence
	uint16_t note = lowest_note + rand() % (highest_note - lowest_note + 1);
	dprintf("[Midi Game] Played from tone %d\n", note - LOWEST_NOTE);
	memset(out_batch_buffer, 0, MI_GAME_BATCH_MAX_LENGTH * sizeof(uint16_t));
	out_batch_buffer[0] = note;
	for (uint8_t i = 0; i < intervals_count; i++)
	{
		note += intervals[i];
		out_batch_buffer[i + 1] = note;
	}
}

static uint8_t _get_led_index_from_midi_note(uint16_t midi_keycode)
{
    if (midi_keycode < MI_C || midi_keycode > MI_B2) return UINT8_MAX;

	uint8_t const note_index = midi_keycode - MI_C;
	uint8_t const row = 2 - (note_index % 3), col = note_index / 3;
	return row * 12 + col;
}

// Marks a note index in the batch as guessed
static inline void _mark_note_guessed(uint8_t batch_index)
{
	midi_game.guessed_notes_mask |= (1 << batch_index);
	midi_game.guessed_notes_count++;
}

// Checks if a note index was already guessed in the batch
static inline bool _is_note_guessed(uint8_t batch_index)
{
	return midi_game.guessed_notes_mask & (1 << batch_index);
}

static midi_game_guess_validity_t _check_guess_validity(uint16_t midi_keycode, uint8_t corresponding_index, uint8_t* out_guessed_index)
{
	if (midi_game.current_is_simultaneous)
	{
		// Search if the input matches any note in the batch
		uint8_t matched_index = 0;
		for (; matched_index < midi_game.config.notes_batch_length; matched_index++)
			if (midi_keycode == midi_game.current_notes_batch[matched_index]) break;

		if (matched_index < midi_game.config.notes_batch_length) // A valid note was found
		{
			*out_guessed_index = matched_index;
			return _is_note_guessed(matched_index) ? ValidRepeat : Valid;
		}
	}
	else
	{
		uint16_t expected_note = midi_game.current_notes_batch[midi_game.guessed_notes_count];
		if (midi_keycode == expected_note)
		{
			*out_guessed_index = midi_game.guessed_notes_count;
			return Valid;
		}

		for (uint8_t i = 0; i < midi_game.guessed_notes_count; i++)
			if (midi_keycode == midi_game.current_notes_batch[i])
			{
				*out_guessed_index = i;
				return ValidRepeat;
			}
	}

	// Search if the mistake was already done
	for (uint8_t i = 0; i < midi_game.wrong_notes_count; i++)
		if (corresponding_index == midi_game.wrong_notes_indexes[i]) return InvalidRepeat;
	return Invalid;
}

// Checks if all the notes in the batch were found
static inline bool _has_guessed_all(void)
{
	return midi_game.guessed_notes_count == midi_game.config.notes_batch_length;
}

static void _play_note(uint16_t midi_keycode, bool state)
{
	keyrecord_t record;
	record.event.pressed = state;
	process_midi(midi_keycode, &record); // Hack to play midi notes
}

// Stop all the notes that are currently playing from the batch player
static void _stop_all_played_batch_notes(void)
{
    // Clean all the remaining notes from batch playing
    if (midi_game.played_notes_mask != 0)
    {
        for (uint8_t i = 0; i < midi_game.config.notes_batch_length; i++)
        {
            const uint16_t note = midi_game.current_notes_batch[i];
            const bool is_currently_played = midi_game.played_notes_mask & (1 << i);
            if (is_currently_played) _play_note(note, false);
        }
        midi_game.played_notes_mask = 0;
    }
}

static void _play_notes_batch(bool replay)
{
	midi_game.player_turn = false;
	midi_game.is_replaying = replay;
	midi_game.play_start_time = timer_read();
	midi_game.next_round_time = 0;
	midi_game.has_muted_before_play = false;

	_stop_all_played_batch_notes();

	if (!replay) // If not replaying, start a new round
	{
		_generate_random_note_batch(midi_game.current_notes_batch, &midi_game.current_is_simultaneous);
        midi_game.wrong_notes_count = midi_game.guessed_notes_count = 0;
		midi_game.guessed_notes_mask = 0;
		memset(midi_game.wrong_notes_indexes, UINT8_MAX, sizeof(midi_game.wrong_notes_indexes));
        midi_game._note_duration = midi_game.current_is_simultaneous ? MI_GAME_SIMULTANEOUS_NOTE_DURATION_MS : MI_GAME_NOTE_DURATION_MS;

		// Update led indicator for the first note reference
		uint16_t ref_note = midi_game.current_notes_batch[0];
		if (midi_game.current_is_simultaneous)
		{
			// If simultaneous, the indicator is the lowest note
			for (uint8_t i = 0; i < midi_game.config.notes_batch_length; i++)
				if (midi_game.current_notes_batch[i] < ref_note) ref_note = midi_game.current_notes_batch[i];
		}
		_first_note_led_index = _get_led_index_from_midi_note(ref_note);
		midi_game.has_won = false; // New turn, reset the won state
	}

	// Compute the note delay for the next batch (need to recompute on replay in case the BPM was changed)
    if (!midi_game.current_is_simultaneous)
    {
        const uint16_t bpm = ((float) midi_game.config.tempo / (MI_GAME_TEMPO_STEPS - 1)) * (MI_GAME_MAX_TEMPO - MI_GAME_MIN_TEMPO) + MI_GAME_MIN_TEMPO;
        midi_game._delay_between_notes = 60000 / bpm;
        dprintf("[Midi Game] Playing note batch at %d BPM (%dms)\n", bpm, midi_game._delay_between_notes);
    }
    else
    {
        midi_game._delay_between_notes = 0;
        dprintf("[Midi Game] Playing simultaneous notes batch\n");
    }

    midi_game.played_notes_mask = 0;
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

		if (!midi_game.has_muted_before_play)
		{
            midi_send_cc(&midi_device, midi_config.channel, 120, 0);
			midi_game.has_muted_before_play = true;
		}

        // Play and release the notes in the current batch
        for (uint8_t i = 0; i < midi_game.config.notes_batch_length; i++)
        {
            const uint16_t note = midi_game.current_notes_batch[i];
            const uint16_t note_start_time = i * midi_game._delay_between_notes;
            const uint16_t note_stop_time = note_start_time + midi_game._note_duration;

            const bool is_currently_played = midi_game.played_notes_mask & (1 << i);
            const bool should_be_played = play_elapsed_time >= note_start_time && play_elapsed_time <= note_stop_time;

            if (is_currently_played != should_be_played) // The note must be updated
            {
                if (should_be_played) _play_note(note, true);
				else _play_note(note, false);
                // Update the played note mask state by overriding the bit corresponding to the note
                midi_game.played_notes_mask = (midi_game.played_notes_mask & ~((uint16_t) 1 << i)) | ((should_be_played & 1) << i);
                dprintf("[Midi Game] Play mask update : 0x%04x (%s 0x%04x)\n", midi_game.played_notes_mask, should_be_played ? "pressed" : "released", note);
            }
        }

        // Detect when we are done playing the batch
        if (play_elapsed_time >= midi_game._note_duration && midi_game.played_notes_mask == 0)
        {
            midi_game.player_turn = true;
			midi_game.is_replaying = false;
			dprintf("[Midi Game] Waiting for player input\n");
        }
	}
	else if (midi_game.next_round_time != 0)
	{
		if (current_time > midi_game.next_round_time)
		    _play_notes_batch(false);
	}
}

static void _start_midi_game(void)
{
	if (midi_game.started) return;
	midi_game.started = true;
    srand(timer_read32()); // Set a random seed for the game

	midi_config.octave = MI_OC2 - MIDI_OCTAVE_MIN;
	midi_send_cc(&midi_device, midi_config.channel, 120, 0);

	dprintf("[Midi Game] Started\n");

	_play_notes_batch(false);
}

static void _stop_midi_game(void)
{
	if (!midi_game.started) return;
	midi_game.started = false;

    _stop_all_played_batch_notes();
	midi_send_cc(&midi_device, midi_config.channel, 120, 0);

	dprintf("[Midi Game] Stopped\n");
}

// Checks if a given input is valid at this state of the game
static void _handle_guess(uint16_t midi_keycode)
{
	uint8_t guess_led_index = _get_led_index_from_midi_note(midi_keycode);
	uint8_t batch_index = UINT8_MAX;
	midi_game_guess_validity_t guess_validity = _check_guess_validity(midi_keycode, guess_led_index, &batch_index);

	switch (guess_validity)
	{
		case Valid:
			_mark_note_guessed(batch_index);
			printf("[Midi Game] * Guessed note %d (%d / %d) !\n", batch_index, midi_game.guessed_notes_count, midi_game.config.notes_batch_length);
			if (!midi_game.current_is_simultaneous) // Clear registered mistakes in case they are the correct next note
				midi_game.wrong_notes_count = 0; // No need to clear buffer
			break;
		case ValidRepeat:
			printf("[Midi Game] * Playing valid note again %d\n", batch_index);
			break;
		case InvalidRepeat:
			printf("[Midi Game] * Repeat mistake on led %d\n", guess_led_index);
			break;
		case Invalid:
			if (midi_game.wrong_notes_count == MI_GAME_MAX_MISTAKES) break;
			midi_game.wrong_notes_indexes[midi_game.wrong_notes_count] = guess_led_index;
			midi_game.wrong_notes_count++;
			printf("[Midi Game] * New mistake on led %d (%d mistakes in total)\n", guess_led_index, midi_game.wrong_notes_count);
			break;
	}

	if (midi_game.wrong_notes_count >= MI_GAME_MAX_MISTAKES)
	{
		// Too many mistakes, skipping the level
		midi_game.has_won = false;
		midi_game.next_round_time = timer_read() + MI_GAME_DELAY_AFTER_CORRECT_ANSWER;
		printf("[Midi Game] *** Too many mistakes, skipping... ***\n");
	}
	else if (_has_guessed_all())
	{
		midi_game.has_won = true;
		midi_game.next_round_time = timer_read() + MI_GAME_DELAY_AFTER_CORRECT_ANSWER;
		printf("[Midi Game] *** All notes found ! ***\n");
	}
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
			if (midi_game.player_turn && record->event.pressed)
				_handle_guess(keycode);
			break;
	}

    return true;
}

static void _render_midi_game(uint16_t current_time, uint8_t led_min, uint8_t led_max)
{
	if (!midi_game.started) return;

	// Hide useless inputs (octaves)
	RGB_MATRIX_INDICATOR_SET_COLOR(39, 0, 0, 0);
	RGB_MATRIX_INDICATOR_SET_COLOR(40, 0, 0, 0);
	RGB_MATRIX_INDICATOR_SET_COLOR(42, 0, 0, 0);

	if (_first_note_led_index != UINT8_MAX && (midi_game.player_turn || midi_game.played_notes_mask != 0))
	{
		RGB_MATRIX_INDICATOR_SET_COLOR_V(_first_note_led_index, 255, 255, 0); // Ref note indicator
	}

	if (midi_game.next_round_time != 0)
	{
		uint8_t won_col = midi_game.has_won ? 255 : 0;
		for (uint8_t i = 38; i < 45; i++)
			RGB_MATRIX_INDICATOR_SET_COLOR_V(i, 255 - won_col, won_col, 0); // Bar is displayed when new batch is playing
	}
	else if (midi_game.is_replaying)
	{
		for (uint8_t i = 38; i < 45; i++)
			RGB_MATRIX_INDICATOR_SET_COLOR_V(i, 255, 128, 0); // Orange bar is displayed when the batch is replaying
	}

	if (midi_game.player_turn || midi_game.is_replaying) // While the player is guessing, show correct notes & mistakes
	{
		for (uint8_t i = 0; i < midi_game.wrong_notes_count; i++)
			RGB_MATRIX_INDICATOR_SET_COLOR_V(midi_game.wrong_notes_indexes[i], 255, 0, 0); // Display wrong notes

		for (uint8_t i = 0; i < midi_game.config.notes_batch_length; i++)
		{
			if(_is_note_guessed(i))
			{
				uint8_t index = _get_led_index_from_midi_note(midi_game.current_notes_batch[i]);
				RGB_MATRIX_INDICATOR_SET_COLOR_V(index, 0, 255, 0); // Display correct notes
			}
		}
	}
	RGB_MATRIX_INDICATOR_SET_COLOR_V(45, 255, 0, 0); // Midi Game status led
	RGB_MATRIX_INDICATOR_SET_COLOR_V(36, 255, 128, 0); // Replay button
	RGB_MATRIX_INDICATOR_SET_COLOR_V(37, 255, 255, 0); // Next button
}

void on_layer_show_midi(void)
{
	tap_code16(QK_MIDI_ON);
    dprintf("[Midi] Show layer\n");

    // Setup default configuration for the Midi Game
    midi_game.config.tempo = 3;
	midi_game.config.notes_batch_length = 3;
}

void on_layer_hide_midi(void)
{
	_stop_midi_game();
	tap_code16(QK_MIDI_OFF);
	_midi_game_key_press_time = 0;
    dprintf("[Midi] Hide layer\n");
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
	else if (_midi_game_key_press_time > 0 && TIMER_DIFF_16(now, _midi_game_key_press_time) >= MI_GAME_KEY_HOLD_TIME_MS)
	{
		RGB_MATRIX_INDICATOR_SET_COLOR_V(45, 255, 255, 0); // Midi Game button confirm
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
					_midi_game_key_press_time = record->event.time;
				else
				{
					if (TIMER_DIFF_16(record->event.time, _midi_game_key_press_time) >= MI_GAME_KEY_HOLD_TIME_MS)
						_start_midi_game();
					_midi_game_key_press_time = 0;
				}
			}
			else if (!record->event.pressed) // instantly stops the game when releasing the key again
			{
				_stop_midi_game();
				_midi_game_key_press_time = 0;
			}
			break;
		case KZ_EXIT:
			if (record->event.pressed)
			{
				layer_off(L_MIDI);
				return false;
			}
			break;
		// Temp bindings to change the midi game config
		case KZ_MI_GAME_REPLAY:
			if (record->event.pressed) midi_game.config.notes_batch_length = 2;
			return false;
		case KZ_MI_GAME_NEXT:
			if (record->event.pressed) midi_game.config.notes_batch_length = 3;
			return false;
	}
	return true;
}
