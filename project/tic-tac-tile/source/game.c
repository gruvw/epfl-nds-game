#include "board.h"

Board init_game() {
    return START_BOARD;
}

Board bot_placed_cell(Board board) {
    for (Coords c = 0; c <= BOTTOM_RIGHT; c++) {
        if (is_empty(board, c)) {
            return placed_cell(board, CIRCLE, c);
        }
    }

    return board;
}
