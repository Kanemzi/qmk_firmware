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
    KZ_EXIT = SAFE_RANGE, // Exit any persistent layer (configuration panel, midi keyboard)
    KZ_MS_WHEEL_MODE, // While held, change the mouse arrows mode from "move" to "wheel"
    KZ_CFG_SAVE, // Save the current configuration if dirty
    KZ_MI_ALT, // While held, enables the alternative control row on midi keyboard
};

typedef enum unicode_names
{
    GMTL,
    GMTR,
    CADR,
    HCADR,
    MIDDOT,
    EXP0,
    EXP1,
    EXP3,
    EXP4,
    EXP5,
    EXP6,
    EXP7,
    EXP8,
    EXP9,
    AGRVCAP,
    EACUCAP,
    EGRVCAP,
    UGRVCAP,
    CCEDCAP,
    OE,
    OECAP,
    TM,
} unicode_names;

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

typedef union
{
    uint32_t raw;
    struct {
        bool windows_unicode_fallback :1; // Whether or not the keyboard should use the shitty Windows alt+decimal alternative to unicode (when possible)
    };
} user_config_t;

extern user_config_t user_config;
extern const uint32_t unicode_map[] PROGMEM;

#define ALT_SEQ_END 10
#define ALT_NO_SEQ {ALT_SEQ_END, 0, 0}
extern const uint8_t windows_unicode_fallback_map[][3] PROGMEM;
