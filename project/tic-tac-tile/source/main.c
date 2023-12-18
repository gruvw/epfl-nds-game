#include <nds.h>
#include <stdio.h>

#include "game.h"
#include "bot.h"
#include "graphics.h"
#include "nds/input.h"

int main(void) {
    // consoleDemoInit();
    // printf("\nTemplate nds\n");

    // Board b = START_BOARD;
    // b = placed_cell(b, CROSS, COORDS(0, 0));
    // b = placed_cell(b, CROSS, COORDS(0, 1));
    // b = placed_cell(b, CROSS, COORDS(0, 2));

    // Coords a = above_empty(b, COORDS(1, 2));
    // printf("row %d, col %d", ROW(a), COL(a));

    graphics_setup();
    game_setup();
    game_loop();
}

// ASK glitching dmaCopy X, O, select
// ASK -O1 optimizations fail
// ASK second NDS to test wifi
