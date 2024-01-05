#include <nds.h>
#include <maxmod9.h>

#include "soundbank.h"
#include "soundbank_bin.h"

#include "audio.h"

#define MUSIC_VOLUME 190

void audio_setup() {
    mmInitDefaultMem((mm_addr) soundbank_bin);

    // Load modules
    mmLoad(MOD_MENU);
    mmLoad(MOD_GAME);

    // Load effects
    mmLoadEffect(SFX_OVER);
    mmLoadEffect(SFX_PLAY);
    mmLoadEffect(SFX_TOUCH);

    mmSetModuleVolume(MUSIC_VOLUME);
}

void menu_music() {
    mmStop();
    mmStart(MOD_MENU, MM_PLAY_LOOP);
}

void game_music() {
    mmStop();
    mmStart(MOD_GAME, MM_PLAY_LOOP);
}

void select_sound(bool touch) {
    mmEffect(touch ? SFX_TOUCH : SFX_PLAY);
}

void game_over_sound() {
    mmStop();
    mmEffect(SFX_OVER);
}
