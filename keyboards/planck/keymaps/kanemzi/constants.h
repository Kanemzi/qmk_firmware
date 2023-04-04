#pragma once

#include QMK_KEYBOARD_H

typedef enum planck_layers
{
    L_BASE = 0,
    L_LOWER,
    L_RAISE,
    L_HUB,
    L_MEDIA,
    L_MOUSE,
    L_MIDI,
    L_CONFIG
} planck_layers;

enum planck_keycodes
{
    KZ_EXIT = SAFE_RANGE,
    KZ_MS_WHEEL_MODE,
    KZ_CFG_SAVE
};

/* colors */
#define AZURE {RGB_AZURE}
#define BLACK {RGB_BLACK}
#define BLUE {RGB_BLUE}
#define CHARTREUSE {RGB_CHARTREUSE}
#define CORAL {RGB_CORAL}
#define CYAN {RGB_CYAN}
#define GOLD {RGB_GOLD}
#define GOLDENROD {RGB_GOLDENROD}
#define GREEN {RGB_GREEN}
#define MAGENTA {RGB_MAGENTA}
#define ORANGE {RGB_ORANGE}
#define PINK {RGB_PINK}
#define PURPLE {RGB_PURPLE}
#define RED {RGB_RED}
#define SPRINGGREEN {RGB_SPRINGGREEN}
#define TEAL {RGB_TEAL}
#define TURQUOISE {RGB_TURQUOISE}
#define WHITE {RGB_WHITE}
#define YELLOW {RGB_YELLOW}