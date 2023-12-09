#include <nds.h>
#include <stdio.h>

#include "board.h"
#include "graphics.h"

int main(void) {
    // consoleDemoInit();

    // printf("\nTemplate nds\n");

    // Board b = START_BOARD;

    // b = placed_cell(b, CROSS, COORDS(0, 0));
    // Winner w = winner_of(b);
    // printf("%u\n", w.side);
    // printf("%u\n", w.start);
    // printf("%u\n", w.direction);

    // b = placed_cell(b, CROSS, COORDS(1, 1));
    // b = placed_cell(b, CROSS, COORDS(2, 2));
    // b = placed_cell(b, CIRCLE, COORDS(2, 1));

    // w = winner_of(b);
    // printf("%u\n", w.side);
    // printf("%u\n", w.start);
    // printf("%u\n", w.direction);

    // printf("%u\n", cell_at(b, COORDS(2, 1)));
    // printf("%u\n", is_empty(b, COORDS(2, 1)));

    graphics_setup();
    set_background();
    set_cross(150, 10);

    while (1) {
        swiWaitForVBlank();
    }
}
