#pragma once

#include QMK_KEYBOARD_H

#include "../constants.h"
#include "layers/layers.h"

/* Mouse Layer
 * ,-----------------------------------------------------------------------------------.
 * |██████|██████|██████|██████|██████|██████|██████| Bt 1 |  MUp | Bt 2 |██████|██████|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |██████|██████|██████|██████|██████|██████| Bt 3 | MLeft| MDown|MRight|██████|██████|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |██████|██████|██████|██████|██████|██████|██████|██████|██████|██████|██████|██████|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |██████|██████|██████|██████| Wheel|█████████████| Wheel|██████|██████|██████|      |
 * `-----------------------------------------------------------------------------------'
 */
#define LAYER_MOUSE_GRID LAYOUT_planck_grid(                                                                                          \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX,    KC_MS_BTN1,       KC_MS_UP,   KC_MS_BTN2,  XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, KC_MS_BTN3, KC_MS_LEFT,       KC_MS_DOWN, KC_MS_RIGHT, XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX,    XXXXXXX,          XXXXXXX,    XXXXXXX,     XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KZ_MS_WHEEL_MODE, XXXXXXX, XXXXXXX,    KZ_MS_WHEEL_MODE, XXXXXXX,    XXXXXXX,     XXXXXXX, XXXXXXX  \
)

#define LAYER_MOUSE_LEDMAP {                                                                                 \
    BLACK, BLACK, BLACK, BLACK, BLACK,       BLACK, BLACK,      CHARTREUSE, CORAL, CHARTREUSE, BLACK, BLACK, \
    BLACK, BLACK, BLACK, BLACK, BLACK,       BLACK, CHARTREUSE, CORAL,      CORAL, CORAL,      BLACK, BLACK, \
    BLACK, BLACK, BLACK, BLACK, BLACK,       BLACK, BLACK,      BLACK,      BLACK, BLACK,      BLACK, BLACK, \
    BLACK, BLACK, BLACK, BLACK, SPRINGGREEN,    BLACK,          SPRINGGREEN,BLACK, BLACK,      BLACK, PURPLE \
}

void on_layer_show_mouse(void);
void on_layer_hide_mouse(void);
void on_layer_render_mouse(uint8_t led_min, uint8_t led_max);
bool on_process_record_mouse(uint16_t keycode, keyrecord_t *record);

extern layer_info_t layer_info_mouse;