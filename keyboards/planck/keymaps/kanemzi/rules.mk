SRC += layers/layer_config.c
SRC += layers/layer_midi.c
SRC += layers/layer_mouse.c
SRC += kanemzi_utils.c

EXTRAFLAGS += -flto

LTO_ENABLE = yes

CONSOLE_ENABLE = yes
UNICODEMAP_ENABLE = yes

DEBOUNCE_TYPE = sym_eager_pr

MIDI_ENABLE = yes
MOUSEKEY_ENABLE = yes
ENCODER_ENABLE = no
AUDIO_ENABLE = no
MUSIC_ENABLE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
DEFERRED_EXEC_ENABLE = no
