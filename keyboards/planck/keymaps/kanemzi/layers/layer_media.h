#pragma once

#include QMK_KEYBOARD_H

#include "../constants.h"

/* Media Layer
 * ,-----------------------------------------------------------------------------------.
 * | Files| Mute | Vol- | Stop | Vol+ |██████|██████|██████|██████|██████|██████|██████|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Impr |██████| Prev | Play | Next |██████|██████|██████|██████|██████|██████|██████|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |██████|██████|██████|██████|██████|██████|██████|██████|██████|██████|██████|██████|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |██████|██████|██████|      |█████████████|██████|██████|██████|██████|██████|
 * `-----------------------------------------------------------------------------------'
 */
#define LAYER_MEDIA_GRID LAYOUT_planck_grid(                                                                                                                     \
    KC_MY_COMPUTER, KC_AUDIO_MUTE, KC_AUDIO_VOL_DOWN,   KC_MEDIA_STOP,       KC_AUDIO_VOL_UP,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    KC_PSCR,        XXXXXXX,       KC_MEDIA_PREV_TRACK, KC_MEDIA_PLAY_PAUSE, KC_MEDIA_NEXT_TRACK, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    XXXXXXX,        XXXXXXX,       XXXXXXX,             XXXXXXX,             XXXXXXX,             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    _______,        XXXXXXX,       XXXXXXX,             XXXXXXX,             _______,             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  \
)

#define LAYER_MEDIA_LEDMAP {                                                                    \
    GOLDENROD, GREEN, GREEN,  PURPLE, GREEN,  BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,  \
    TEAL,      BLACK, PURPLE, PURPLE, PURPLE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,  \
    BLACK,     BLACK, BLACK,  BLACK,  BLACK,   BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, \
    PURPLE,    BLACK, BLACK,  BLACK,  PURPLE,      BLACK,    BLACK, BLACK, BLACK, BLACK, BLACK  \
}
