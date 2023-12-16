#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "board.h"

#define B(coords) ((uint32_t) coords << 1U)

const Board START_BOARD = 0;
const Board CELL_MASK = 0b11U;

const Coords SIDE = 3;
const Coords END = SIDE - 1;

const Coords TOP_LEFT = COORDS(0, 0);
const Coords TOP_RIGHT = COORDS(0, END);
const Coords BOTTOM_LEFT = COORDS(END, 0);
const Coords BOTTOM_RIGHT = COORDS(END, END);
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
    TOP_RIGHT, ROW_INCR,
    MID_LEFT, COL_INCR,
    MID_TOP, ROW_INCR,
};

Board prefix(Board board, Coords coords) {
    return board >> B(coords);
}

Board placed_cell(Board board, Cell cell, Coords coords) {
    Board suffix = board & ((1U << B(coords)) - 1);
    Board placed = (prefix(board, coords) & ~CELL_MASK) | cell;
    return (placed << B(coords)) | suffix;
}

Cell cell_at(Board board, Coords coords) {
    return prefix(board, coords) & CELL_MASK;
}

bool is_full(Board board) {
    for (Coords c = 0; c < BOTTOM_RIGHT; c++) {
        if (cell_at(board, c) == EMPTY) {
            return false;
        }
    }

    return true;
}

bool three_same(Board board, Coords start, Coords direction) {
    Cell first = cell_at(board, start);
    Cell second = cell_at(board, start + direction);
    Cell third = cell_at(board, start + 2 * direction);
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
