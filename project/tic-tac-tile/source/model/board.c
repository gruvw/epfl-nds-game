#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "board.h"

// === Utility macros ===

#define COL_MOD(c) ((c) < 0 ? (c) + SIDE : (c))
#define B(coords) ((uint32_t) coords << 1U)
#define PREFIX(board, coords) ((board) >> B(coords))

// === Utility constants ===

const Board START_BOARD = 0;
const Board CELL_MASK = 0b11U;

const Coords SIDE = 3;
const Coords END = SIDE - 1;

const Coords TOP_LEFT = COORDS(0, 0);
const Coords TOP_RIGHT = COORDS(0, END);
const Coords BOTTOM_LEFT = COORDS(END, 0);
const Coords BOTTOM_RIGHT = COORDS(END, END);
const Coords MID_LEFT = COORDS(END / 2, 0);
const Coords TOP_MID = COORDS(0, END / 2);
const Coords MID_RIGHT = COORDS(END / 2, END);
const Coords MID_MID = COORDS(END / 2, END / 2);
const Coords NONE = -1;

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
    BOTTOM_LEFT, COL_INCR,
    TOP_RIGHT, ROW_INCR,
    MID_LEFT, COL_INCR,
    TOP_MID, ROW_INCR,
};

// === Board methods ===

Board placed_cell(Board board, Cell cell, Coords coords) {
    Board suffix = board & ((1U << B(coords)) - 1);
    Board placed = (PREFIX(board, coords) & ~CELL_MASK) | cell;
    return (placed << B(coords)) | suffix;
}

Cell cell_at(Board board, Coords coords) {
    return PREFIX(board, coords) & CELL_MASK;
}

bool is_full(Board board) {
    for (Coords c = TOP_LEFT; c <= BOTTOM_RIGHT; c++) {
        if (cell_at(board, c) == EMPTY) {
            return false;
        }
    }

    return true;
}

bool is_second_move(Board board) {
    bool seen = false;

    for (Coords c = TOP_LEFT; c <= BOTTOM_RIGHT; c++) {
        if (cell_at(board, c) != EMPTY) {
            if (seen) {
                return false;
            }
            seen = true;
        }
    }

    return seen;
}

Coords first_move_coords(Board board) {
    // Assumes there is a least one move on board

    for (Coords c = TOP_LEFT; c <= BOTTOM_RIGHT; c++) {
        if (cell_at(board, c) != EMPTY) {
            return c;
        }
    }

    return NONE;
}

bool is_finished(Board board) {
    return winner_of(board).side != EMPTY || is_full(board);
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
            return (Winner) { cell_at(board, WINS[i]), WINS[i], WINS[i + 1] };
        }
    }

    return (Winner) { EMPTY, TOP_LEFT, NO_INCR };
}
