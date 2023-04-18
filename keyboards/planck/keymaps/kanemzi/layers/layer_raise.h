#pragma once

#include "keymap_french.h"

#include "../constants.h"

/* Raise Layer (accents & symbols)
 * ,-----------------------------------------------------------------------------------.
 * |  Del |   à  |   é  |   è  |   <  |   '  |      |   >  |   _  |   @  |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |  ->  |   ^ ¨|   [  |   (  |   {  |   "  |      |   }  |   )  |   ]  |   $ €|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |   \  |   ~  |   ç  |   ù  |   &  |   |  |   `  |   – —|   #  |   ! §|   ·  | todo: ´ and ·
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      | Mouse|
 * `-----------------------------------------------------------------------------------'
 */
#define LAYER_RAISE_GRID LAYOUT_planck_grid(                                                                         \
    KC_DELETE, FR_AGRV, FR_EACU, FR_EGRV, FR_LABK, FR_QUOT, _______, FR_RABK, FR_UNDS,  FR_AT,   _______, _______,    \
    KZ_ARROW,  FR_CIRC, FR_LBRC, FR_LPRN, FR_LCBR, FR_DQUO, _______, FR_RCBR, FR_RPRN,  FR_RBRC, FR_DLR,  _______,    \
    _______,   FR_BSLS, FR_TILD, FR_CCED, FR_UGRV, FR_AMPR, FR_PIPE, FR_GRV , X(HCADR), FR_HASH, FR_EXLM, X(MIDDOT),  \
    _______,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  MO(L_MOUSE) \
)

#define LAYER_RAISE_LEDMAP {                                                                   \
    RED,   PURPLE, PURPLE, PURPLE,  GOLD,    WHITE, BLACK, GOLD,  WHITE, WHITE, BLACK, BLACK,  \
    WHITE, WHITE,  GOLD,   GOLD,    GOLD,    WHITE, BLACK, GOLD,  GOLD,  GOLD,  WHITE, BLACK,  \
    BLACK, WHITE,  WHITE,   PURPLE, PURPLE,  WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,  \
    BLACK, BLACK,  BLACK,  BLACK,   MAGENTA,    BLACK,    PURPLE, BLACK, BLACK, BLACK, MAGENTA \
}
