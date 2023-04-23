#pragma once

#include QMK_KEYBOARD_H

#include "../constants.h"

/* Hub Layer (access to complex features)
 * ,-----------------------------------------------------------------------------------.
 * |██████|██████|██████|██████|██████|██████|██████|██████|   Up |██████|██████|██████|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |██████|██████|Config| Midi |██████|██████|██████| Left | Down | Right|██████|██████|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |██████|██████|██████|██████|██████|██████|██████|██████|██████|██████|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |█████████████|      |██████|██████|██████|      |
 * `-----------------------------------------------------------------------------------'
 */
#define LAYER_HUB_GRID LAYOUT_planck_grid(                                                                               \
    XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_UP  , XXXXXXX,  XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, TG(L_CONFIG), TG(L_MIDI), XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_RIGHT, XXXXXXX, XXXXXXX, \
    _______, XXXXXXX, XXXXXXX,      XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, _______, \
    KC_LCTL, _______, _______,      _______,    _______, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX,  XXXXXXX, _______  \
)

#define LAYER_HUB_LEDMAP {                                                                 \
    BLACK, BLACK, BLACK,  BLACK,  BLACK, BLACK, BLACK, BLACK,  ORANGE, BLACK,  BLACK, BLACK, \
    BLACK, BLACK, ORANGE, PURPLE, BLACK, BLACK, BLACK, ORANGE, ORANGE, ORANGE, BLACK, BLACK, \
    BLACK, BLACK, BLACK,  BLACK,  BLACK, BLACK, BLACK, BLACK,  BLACK,  BLACK,  BLACK, BLACK, \
    BLACK, BLACK, BLACK,  BLACK,  PURPLE,   BLACK,     PURPLE, BLACK,  BLACK,  BLACK, BLACK  \
}
