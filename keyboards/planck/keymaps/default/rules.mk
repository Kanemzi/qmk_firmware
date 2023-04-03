ifeq ($(strip $(AUDIO_ENABLE)), yes)
    SRC += muse.c
endif

LTO_ENABLE = yes
AUDIO_ENABLE = no
CONSOLE_ENABLE = no