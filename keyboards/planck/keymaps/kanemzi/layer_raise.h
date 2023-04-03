#pragma once

#include "keymap_french.h"

#include "constants.h"

/* Raise Layer (accents & symbols)
 * ,-----------------------------------------------------------------------------------.
 * |  Del |   à  |   é  |   è  |   <  |      |      |   >  |   _  |   @  |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |   ^ ¨|   [  |   (  |   {  |   "  |   '  |   }  |   )  |   ]  |   $ €|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |   \  |   ~  |   ç  |   ù  |   &  |   |  |   `  |   ´  |   #  |   ! §|   ·  | todo: ´ and ·
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      | Mouse|
 * `-----------------------------------------------------------------------------------'
 */
#define LAYER_RAISE_GRID LAYOUT_planck_grid(                                                                         \
    KC_DELETE, FR_AGRV, FR_EACU, FR_EGRV, FR_LABK, _______, _______, FR_RABK, FR_UNDS, FR_AT,   _______, _______,    \
    _______,   FR_CIRC, FR_LBRC, FR_LPRN, FR_LCBR, FR_DQUO, FR_QUOT, FR_RCBR, FR_RPRN, FR_RBRC, FR_DLR,  _______,    \
    _______,   FR_BSLS, FR_TILD, FR_CCED, FR_UGRV, FR_AMPR, FR_PIPE, FR_GRV , _______, FR_HASH, FR_EXLM, _______,    \
    _______,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, MO(L_MOUSE) \
)

#define LAYER_RAISE_LEDMAP {                                                                 \
    RED,   PURPLE, PURPLE, PURPLE, GOLD,   BLACK, BLACK, GOLD,   TEAL,  TEAL,  BLACK, BLACK, \
    BLACK, TEAL,   GOLD,   GOLD,   GOLD,   TEAL,  TEAL,  GOLD,   GOLD,  GOLD,  TEAL,  BLACK, \
    BLACK, TEAL,   TEAL,   PURPLE, PURPLE, TEAL,  TEAL,  TEAL,   BLACK, TEAL,  TEAL,  BLACK, \
    BLACK, BLACK,  BLACK,  BLACK,  ORANGE,    BLACK,     ORANGE, BLACK, BLACK, BLACK, ORANGE \
}
