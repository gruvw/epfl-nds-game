#include <nds.h>

#include "controller/game.h"
#include "view/graphics.h"

int main(void) {
    game_setup();

    game_loop();
}

// ASK second NDS to test wifi
// ASK speed up audio (mod) file
