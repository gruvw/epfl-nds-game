#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "board.h"

#define COL_MOD(c) ((c) < 0 ? (c) + SIDE : (c))
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
const Coords MID_RIGHT = COORDS(END / 2, END);
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
    for (Coords c = TOP_LEFT; c <= BOTTOM_RIGHT; c++) {
        if (cell_at(board, c) == EMPTY) {
            return false;
        }
    }

    return true;
}

Coords next_empty(Board board, Coords start) {
    for (Coords c = start + 1; c <= BOTTOM_RIGHT; c++) {
        if (cell_at(board, c) == EMPTY) {
            return c;
        }
    }

    return start;
}

Coords prev_empty(Board board, Coords start) {
    for (Coords c = start - 1; c >= TOP_LEFT; c--) {
        if (cell_at(board, c) == EMPTY) {
            return c;
        }
    }

    return start;
}

// Helper for above_empty and below_empty
Coords in_row_empty(Board board, Coords start) {
    if (cell_at(board, start) == EMPTY) {
        return start;
    }

    for (Coords col = 0; col <= END; col++) {
        Coords candidate = COORDS(ROW(start), col);
        if (cell_at(board, candidate) == EMPTY) {
            return candidate;
        }
    }

    return NONE;
}

Coords above_empty(Board board, Coords start) {
    for (size_t i = 0; i < END; i ++) {
        if (start <= TOP_RIGHT + i * SIDE) {
            return start;
        }

        Coords above = start - (i + 1) * SIDE;
        Coords found = in_row_empty(board, above);
        if (found != NONE) {
            return found;
        }
    }

    return start;
}

Coords bellow_empty(Board board, Coords start) {
    for (size_t i = 0; i < END; i ++) {
        if (start >= BOTTOM_LEFT - i * SIDE) {
            return start;
        }

        Coords bellow = start + (i + 1) * SIDE;
        Coords found = in_row_empty(board, bellow);
        if (found != NONE) {
            return found;
        }
    }

    return start;
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
            return (Winner) {
                cell_at(board, WINS[i]),
                WINS[i],
                WINS[i + 1]
            };
        }
    }

    return (Winner) { EMPTY, TOP_LEFT, NO_INCR };
}
