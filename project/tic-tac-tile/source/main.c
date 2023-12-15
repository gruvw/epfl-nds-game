#include <nds.h>
#include <stdio.h>

#include "board.h"
#include "graphics.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')

int main(void) {
    // consoleDemoInit();

    // printf("\nTemplate nds\n");

    // Board b = START_BOARD;
    // for (Coords c = 0; c <= BOTTOM_RIGHT; c++) {
    //     b = placed_cell(b, c % 2 ? CROSS : CIRCLE, c);
    // }
    // for (Coords c = 0; c <= BOTTOM_RIGHT; c++) {
    //     printf("%d", COL(c));
    //     if (!is_empty(b, c)) {
    //         // printf(cell_at(b, c) == CROSS ? "X" : "O");
    //     }
    // }
    // printf("\n"BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(b));

    // b = placed_cell(b, CIRCLE, COORDS(0, 0));
    // b = placed_cell(b, CIRCLE, COORDS(0, 1));
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
    set_backgrounds();
    main_graphics();

    while (1) {
        swiWaitForVBlank();
    }
}
