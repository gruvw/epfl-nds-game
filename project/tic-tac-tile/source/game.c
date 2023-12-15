#include "graphics.h"
#include "nds.h"
#include "nds/arm9/input.h"
#include "nds/input.h"
#include "nds/interrupts.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

#define PRESSED(key) ((~REG_KEYINPUT & key) != 0)

// === Gloabals ===

Board board;
Coords selection_coords;

// === Utilities ===

void refresh_game_screen() {
    clear_game_screen();
    draw_select(selection_coords);
    draw_board(board);
}

// === Interrupt handlers ===

void keys_handler() {
    if (PRESSED(KEY_RIGHT) && selection_coords < BOTTOM_RIGHT) {
        selection_coords += COL_INCR;
    } else if (PRESSED(KEY_LEFT) && selection_coords > TOP_LEFT) {
        selection_coords -= COL_INCR;
    } else if (PRESSED(KEY_DOWN) && selection_coords < BOTTOM_LEFT) {
        selection_coords += ROW_INCR;
    } else if (PRESSED(KEY_UP) && selection_coords > TOP_RIGHT) {
        selection_coords -= ROW_INCR;
    } else if (PRESSED(KEY_A)) {
        board = placed_cell(board, CROSS, selection_coords);
    }

    refresh_game_screen();
}

// === Game ===

void game_setup() {
    // Golbals
    board = START_BOARD;
    selection_coords = TOP_LEFT;

    // Interrupts
    REG_KEYCNT = BIT(14) | KEY_UP | KEY_DOWN | KEY_RIGHT | KEY_LEFT | KEY_A;

    irqSet(IRQ_KEYS, &keys_handler);
    irqEnable(IRQ_KEYS);
}

void game_loop() {
    // draw_select(COORDS(0,1));
    // draw_select(COORDS(2,2));
    // Board b = START_BOARD;
    // for (Coords c = 0; c <= BOTTOM_RIGHT; c++) {
    //     b = placed_cell(b, c % 2 ? CROSS : CIRCLE, c);
    // }
    // draw_board(b);
    // clear_game();
    // draw_select(COORDS(2,2));

    while (1) {
        swiWaitForVBlank();
    }
}
