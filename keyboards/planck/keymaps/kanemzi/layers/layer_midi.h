#pragma once

#include QMK_KEYBOARD_H

#include "../constants.h"
#include "layers/layers.h"

/* Midi Layer (accordion keyboard layout)
 * ,-----------------------------------------------------------------------------------.
 * |  D3  |  F3  |  G#3 |  B3  |  D4  |  F4  |  G#4 |  B4  |  D5  |  F5  |  G#5 |  B5  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |  C#3 |  E3  |  G3  |  A#3 |  C#4 |  E4  |  G4  |  A#4 |  C#5 |  E5  |  G5  |  A#5 |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |  C3  |  D#3 |  F#3 |  A3  |  C4  |  D#4 |  F#4 |  A4  |  C5  |  D#5 |  F#5 |  A5  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |██████|██████|██████| Soft | Oct- |   Sustain   | Oct+ |██████|██████| Game | Exit |
 * `-----------------------------------------------------------------------------------'
 */
#define LAYER_MIDI_GRID LAYOUT_planck_grid(                                                                                         \
    MI_D,              MI_F,            MI_Gs,   MI_B,    MI_D1,   MI_F1,   MI_Gs1,  MI_B1,   MI_D2,   MI_F2,   MI_Gs2,     MI_B2,  \
    MI_Cs,             MI_E,            MI_G,    MI_As,   MI_Cs1,  MI_E1,   MI_G1,   MI_As1,  MI_Cs2,  MI_E2,   MI_G2,      MI_As2, \
    MI_C,              MI_Ds,           MI_Fs,   MI_A,    MI_C1,   MI_Ds1,  MI_Fs1,  MI_A1,   MI_C2,   MI_Ds2,  MI_Fs2,     MI_A2,  \
    KZ_MI_GAME_REPLAY, KZ_MI_GAME_NEXT, XXXXXXX, MI_SOFT, MI_OCTD, MI_SUST, XXXXXXX, MI_OCTU, XXXXXXX, XXXXXXX, KZ_MI_GAME, KZ_EXIT \
)
// Alt row ,        ,        ,        , MI_TRSD, MI_TR0 ,        , MI_TRSU,

#define LAYER_MIDI_LEDMAP {                                                                   \
    WHITE, WHITE, BLUE,  WHITE,     WHITE, WHITE, BLUE,  WHITE, WHITE, WHITE, BLUE,   WHITE,  \
    BLUE,  WHITE, WHITE, BLUE,      BLUE,  WHITE, WHITE, BLUE,  BLUE,  WHITE, WHITE,  BLUE,   \
    WHITE, BLUE,  BLUE,  WHITE,     WHITE, BLUE,  BLUE,  WHITE, WHITE, BLUE,  BLUE,   WHITE,  \
    BLACK, BLACK, BLACK, GOLDENROD, PINK,   GOLDENROD,   PINK,  BLACK, BLACK, ORANGE, MAGENTA \
}

#define MI_GAME_KEY_HOLD_TIME_MS 500
#define MI_GAME_BATCH_MAX_LENGTH 7
#define MI_GAME_NOTE_DURATION_MS 250
#define MI_GAME_DELAY_BETWEEN_BATCHES_MS 1000 // When answered correctly, waits for this amount of time before starting the next note batch

/* 						Midi Game rules
 * Sequence length == 2: allows all sequence mods
 * Sequence length >= 3: forbid simultaneous notes
 */

typedef enum
{
	ASCENDING = 1 << 1,
	DESCENDING = 1 << 2,
	SIMULTANEOUS = 1 << 3
} midi_game_batch_flags_t;

typedef struct
{
	uint8_t tempo : 4;
	uint8_t notes_batch_length: 3;
	uint8_t allowed_batch_types;
} midi_game_config_t;

void on_layer_show_midi(void);
void on_layer_hide_midi(void);
void on_layer_render_midi(uint8_t led_min, uint8_t led_max);
bool on_process_record_midi(uint16_t keycode, keyrecord_t *record);
extern layer_info_t layer_info_midi;
