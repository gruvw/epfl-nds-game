#include <stdbool.h>
#include <stddef.h>

#include "board.h"

const Board START_BOARD = 0;
const Board CELL_MASK = 0b11U;

const Coords SIDE = 3;
const Coords END = SIDE - 1;

const Coords TOP_LEFT = COORDS(0, 0);
const Coords TOP_RIGHT = COORDS(0, END);
const Coords BOTTOM_LEFT = COORDS(END, 0);
const Coords MID_LEFT = COORDS(END / 2, 0);
const Coords MID_TOP = COORDS(0, END / 2);

const Coords NO_INCR = TOP_LEFT;
const Coords COL_INCR = COORDS(0, 1);
const Coords ROW_INCR = COORDS(1, 0);
const Coords DIAG_DOWN_INCR = COORDS(1, 1);
const Coords DIAG_UP_INCR = COORDS(-1, 1);

const Cell WINS[] = {
    TOP_LEFT, COL_INCR,
    TOP_LEFT, DIAG_DOWN_INCR,
    TOP_LEFT, ROW_INCR,
    BOTTOM_LEFT, DIAG_UP_INCR,
    BOTTOM_LEFT, ROW_INCR,
    MID_LEFT, ROW_INCR,
    MID_TOP, COL_INCR,
    TOP_RIGHT, COL_INCR,
};

Board placed_cell(Board board, Cell cell, Coords coords) {
    Board prefix = board >> coords;
    Board suffix = board & ((1U << coords) - 1);
    Board placed = (prefix & ~CELL_MASK) | cell;
    return (placed << coords) | suffix;
}

Cell cell_at(Board board, Coords coords) {
    Board prefix = board >> coords;
    return prefix & CELL_MASK;
}

bool is_empty(Board board, Coords coords) {
    return cell_at(board, coords) == EMPTY;
}

bool three_same(Board board, Coords start, Coords direction) {
    Cell first = cell_at(board, start);
    Cell second = cell_at(board, start + direction);
    Cell third = cell_at(board, start + (direction << 1U));
    return (first != EMPTY) && (first == second) && (first == third);
}

Winner winner_of(Board board) {
    for (size_t i = 0; i < sizeof(WINS) / sizeof(*WINS); i += 2) {
        if (three_same(board, WINS[i], WINS[i + 1])) {
            return (Winner) {
                cell_at(board, WINS[i]),
                WINS[i],
                WINS[i + 1]
            };
        }
    }

    return (Winner) { EMPTY, TOP_LEFT, NO_INCR };
}
