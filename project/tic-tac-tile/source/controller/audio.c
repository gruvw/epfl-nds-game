#include <nds.h>
#include <maxmod9.h>
#include <stdbool.h>

#include "soundbank.h"
#include "soundbank_bin.h"

void audio_setup() {
    mmInitDefaultMem((mm_addr) soundbank_bin);

    // Load modules
    mmLoad(MOD_MENU);
    mmLoad(MOD_GAME);

    // Load effects
    mmLoadEffect(SFX_OVER);
    mmLoadEffect(SFX_PLAY);
    mmLoadEffect(SFX_TOUCH);

    mmSetModuleVolume(190);
}

void menu_audio() {
    mmStop();
    mmStart(MOD_MENU, MM_PLAY_LOOP);
}

void game_audio() {
    mmStop();
    mmStart(MOD_GAME, MM_PLAY_LOOP);
}

void select_audio(bool touch) {
    mmEffect(touch ? SFX_TOUCH : SFX_PLAY);
}

void game_over_audio() {
    mmStop();
    mmEffect(SFX_OVER);
}
