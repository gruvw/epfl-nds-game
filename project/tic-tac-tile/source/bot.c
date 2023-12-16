#include "bot.h"

Board bot_placed_cell(Board board) {
    // TODO
    for (Coords c = 0; c <= BOTTOM_RIGHT; c++) {
        if (cell_at(board, c) == EMPTY) {
            return placed_cell(board, CIRCLE, c);
        }
    }

    return board;
}
