#pragma once

#include "keymap_french.h"

#include "../constants.h"

/* Lower Layer (numpad & functions keys)
 * ,-----------------------------------------------------------------------------------.
 * |  Del |  F9  |  F10 |  F11 |  F12 |██████|   °  |   +  |   7  |   8  |   9  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |NumLck|  F5  |  F6  |  F7  |  F8  |   =  |   =  |   -  |   4  |   5  |   6  |   %  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |  F1  |  F2  |  F3  |  F4  |   ^  |   /  |   *  |   1  |   2  |   3  |   .  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |Media |      |      |      |      |             |      |   0  |   0  |      |   =  |
 * `-----------------------------------------------------------------------------------'
 */
#define LAYER_LOWER_GRID LAYOUT_planck_grid(                                                                              \
    KC_DELETE,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  XXXXXXX, FR_DEG,  KC_KP_PLUS,  KC_KP_7, KC_KP_8, KC_KP_9, _______,   \
    KC_NUM,      KC_F5,   KC_F6,   KC_F7,   KC_F8,   FR_EQL,  FR_EQL,  KC_KP_MINUS, KC_KP_4, KC_KP_5, KC_KP_6, FR_PERC,   \
    _______,     KC_F1,   KC_F2,   KC_F3,   KC_F4,   FR_CIRC, KC_PSLS, KC_PAST,     KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_DOT, \
    MO(L_MEDIA), _______, _______, _______, _______, _______, _______, _______,     KC_KP_0, KC_KP_0, _______, FR_EQL     \
)

#define LAYER_LOWER_LEDMAP {                                                                                   \
    RED,     ORANGE, ORANGE, ORANGE, ORANGE, BLACK, CORAL, CORAL,   CHARTREUSE, CHARTREUSE, CHARTREUSE, BLACK, \
    TEAL,    ORANGE, ORANGE, ORANGE, ORANGE, BLACK, CORAL, CORAL,   CHARTREUSE, CHARTREUSE, CHARTREUSE, CORAL, \
    BLACK,   ORANGE, ORANGE, ORANGE, ORANGE, CORAL, CORAL, CORAL,   CHARTREUSE, CHARTREUSE, CHARTREUSE, CORAL, \
    MAGENTA, BLACK,  BLACK,  BLACK,  PURPLE,     BLACK,    MAGENTA, CHARTREUSE, BLACK,      BLACK,      CORAL  \
}

