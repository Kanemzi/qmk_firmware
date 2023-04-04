#pragma once

#include "keymap_french.h"

#include "constants.h"

/* Base Layer
 * ,-----------------------------------------------------------------------------------.
 * | Esc  |   A  |   Z  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Tab  |   Q  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   M  | Enter|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | LShft|   W  |   X  |   C  |   V  |   B  |   N  |   , ?|   ; .|   : /|   Up | RShft|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl | AltGr|  OS  | LAlt | Lower|    Space    | Raise| AltGr| Left | Down | Right|
 * `-----------------------------------------------------------------------------------'
 */
#define LAYER_BASE_GRID LAYOUT_planck_grid(                                                                                 \
    KC_ESCAPE, FR_A,    FR_Z,    FR_E,    FR_R,        FR_T,     FR_Y,    FR_U,        FR_I,    FR_O,    FR_P,    KC_BSPC,  \
    KC_TAB,    FR_Q,    FR_S,    FR_D,    FR_F,        FR_G,     FR_H,    FR_J,        FR_K,    FR_L,    FR_M,    KC_ENTER, \
    KC_LSFT,   FR_W,    FR_X,    FR_C,    FR_V,        FR_B,     FR_N,    FR_COMM,     FR_SCLN, FR_COLN, KC_UP,   KC_RSFT,  \
    KC_LCTL,   KC_RALT, KC_LGUI, KC_LALT, MO(L_LOWER), KC_SPACE, XXXXXXX, MO(L_RAISE), KC_RALT, KC_LEFT, KC_DOWN, KC_RIGHT  \
)

#define LAYER_BASE_LEDMAP {\
    GREEN, WHITE,  WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,  WHITE,  WHITE,  WHITE,  GREEN, \
    GREEN, WHITE,  WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,  WHITE,  WHITE,  WHITE,  GREEN, \
    GREEN, WHITE,  WHITE, WHITE, WHITE, WHITE, WHITE, PINK,   PINK,   PINK,   ORANGE, GREEN, \
    GREEN, ORANGE, GREEN, GREEN, ORANGE,    GREEN,    ORANGE, GREEN,  ORANGE, ORANGE, ORANGE \
}
