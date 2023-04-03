#pragma once

#include QMK_KEYBOARD_H

#include "constants.h"

/* Mouse Layer
 * ,-----------------------------------------------------------------------------------.
 * |██████|██████|██████|██████|██████|██████|██████| Bt 1 |  MUp | Bt 2 |██████|██████|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |██████|██████|██████|██████|██████|██████| Bt 3 | MLeft| MDown|MRight|██████|██████|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |██████|██████|██████|██████|██████|██████|██████|██████|██████|██████|██████|██████|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |██████|██████|██████|██████|██████|█████████████|      | Wheel|██████|██████|      | todo implement Wheel mode
 * `-----------------------------------------------------------------------------------'
 */
#define LAYER_MOUSE_GRID LAYOUT_planck_grid(                                                                                          \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_MS_BTN1, KC_MS_UP,   KC_MS_BTN2,  XXXXXXX,          XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_MS_BTN3, KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT, KZ_MS_WHEEL_MODE, XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,     XXXXXXX,          XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX,    _______,    XXXXXXX,    XXXXXXX,     XXXXXXX,          XXXXXXX  \
)

#define LAYER_MOUSE_LEDMAP {                                                                                 \
    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,      CHARTREUSE, CORAL,       CHARTREUSE, BLACK, BLACK, \
    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, CHARTREUSE, CORAL,      CORAL,       CORAL,      BLACK, BLACK, \
    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,      BLACK,      BLACK,       BLACK,      BLACK, BLACK, \
    BLACK, BLACK, BLACK, BLACK, BLACK,    BLACK,          ORANGE,     SPRINGGREEN, BLACK,      BLACK, ORANGE \
}
