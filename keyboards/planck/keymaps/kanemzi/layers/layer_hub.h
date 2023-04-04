#pragma once

#include QMK_KEYBOARD_H

#include "../constants.h"

/* Hub Layer (access to complex features)
 * ,-----------------------------------------------------------------------------------.
 * |██████|██████|██████|██████|██████|██████|██████|██████|██████|██████|██████|██████|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |██████|██████|Config|██████|██████|██████|██████|██████|██████| Midi |██████|██████|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |██████|██████|██████|██████|██████|██████|██████|██████|██████|██████|██████|██████|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |██████|██████|██████|██████|      |█████████████|      |██████|██████|██████|██████|
 * `-----------------------------------------------------------------------------------'
 */
#define LAYER_HUB_GRID LAYOUT_planck_grid(                                                                              \
    XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, TG(L_CONFIG), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TG(L_MIDI), XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, _______, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX  \
)

#define LAYER_HUB_LEDMAP {                                                                 \
    BLACK, BLACK, BLACK,  BLACK, BLACK, BLACK, BLACK, BLACK,  BLACK, BLACK,  BLACK, BLACK, \
    BLACK, BLACK, ORANGE, BLACK, BLACK, BLACK, BLACK, BLACK,  BLACK, PURPLE, BLACK, BLACK, \
    BLACK, BLACK, BLACK,  BLACK, BLACK, BLACK, BLACK, BLACK,  BLACK, BLACK,  BLACK, BLACK, \
    BLACK, BLACK, BLACK,  BLACK, PURPLE,   BLACK,     PURPLE, BLACK, BLACK,  BLACK, BLACK  \
}
