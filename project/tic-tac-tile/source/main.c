#include <nds.h>

#include "controller/game.h"
#include "view/graphics.h"

int main(void) {
    game_setup();

    game_loop();
}

// ASK why no need for tiles map with sprites
// ASK can we use swi and dma interchangably (does not work)
// ASK -O1 optimizations fail
// ASK second NDS to test wifi
// ASK wifi can lose packets ??
// ASK sprites main sub
// ASK speed up audio (mod) file
