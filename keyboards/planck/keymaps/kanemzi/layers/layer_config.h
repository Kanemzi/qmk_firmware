#pragma once

#include QMK_KEYBOARD_H

#include "../constants.h"
#include "layers/layers.h"

/* Config Layer (persistent settings)
 * ,-----------------------------------------------------------------------------------.
 * | Save |██████|██████|██████|██████|██████|██████|██████|██████|██████|██████| Reset|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |██████|██████|██████|██████|██████|██████|██████| Hue- | Sat+ | Lum+ |██████|██████|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |██████|██████| NKRO | Debug| UcMod|██████|██████| Hue- | Sat- | Lum- |██████|██████|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |██████|██████|██████|██████|██████|█████████████|██████|██████|██████|██████| Exit |
 * `-----------------------------------------------------------------------------------'
 */
#define LAYER_CONFIG_GRID LAYOUT_planck_grid(                                                                       \
    KZ_CFG_SAVE, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT, \
    XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_HUI, RGB_SAI, RGB_VAI, XXXXXXX, XXXXXXX, \
    _______,     XXXXXXX, NK_TOGG, DB_TOGG, UC_NEXT, XXXXXXX, XXXXXXX, RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX, XXXXXXX, \
    XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KZ_EXIT  \
)

#define LAYER_CONFIG_LEDMAP {                                                           \
    GREEN, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, RED,   \
    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, WHITE, WHITE, WHITE, BLACK, BLACK, \
    BLACK, BLACK, RED,   RED,   RED,   BLACK, BLACK, RED,   RED,   RED,   BLACK, BLACK, \
    BLACK, BLACK, BLACK, BLACK, BLACK,     BLACK,    BLACK, BLACK, BLACK, BLACK, ORANGE \
}

void on_layer_show_config(void);
void on_layer_hide_config(void);
void on_layer_render_config(uint8_t led_min, uint8_t led_max);
bool on_process_record_config(uint16_t keycode, keyrecord_t *record);
void on_post_process_record_config(uint16_t keycode, keyrecord_t *record);

void unicode_input_mode_set_user(uint8_t input_mode);

extern layer_info_t layer_info_config;
