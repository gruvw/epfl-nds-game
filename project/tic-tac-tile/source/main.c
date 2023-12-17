#include <nds.h>
#include <stdio.h>

#include "game.h"
#include "graphics.h"
#include "nds/input.h"

int main(void) {
    // consoleDemoInit();
    // printf("\nTemplate nds\n");

    graphics_setup();
    game_setup();
    game_loop();
}

// ASK glitching dmaCopy X, O, select
// ASK -O1 optimizations fail
