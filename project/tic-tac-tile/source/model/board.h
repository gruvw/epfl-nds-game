#include <stdbool.h>

#include "sys/_stdint.h"

#define OTHER_SIDE(side) ((side) == CROSS ? CIRCLE : CROSS)
#define STARTING_SIDE (CROSS)

#define COORDS(row, col) ((row) * SIDE + (col))
#define ROW(coords) ((coords) / SIDE)
#define COL(coords) ((coords) % SIDE)

#ifndef INCLUDE_BOARD

#define INCLUDE_BOARD

typedef uint32_t Board;
typedef int8_t Coords;

typedef enum {
    EMPTY,
    CROSS,
    CIRCLE,
} Cell;

typedef struct {
    Cell side;
    Coords start;
    Coords direction;
} Winner;

#endif

extern const Board START_BOARD, CELL_MASK;

extern const Coords SIDE, END;

extern const Coords
    TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, MID_LEFT, TOP_MID, MID_RIGHT, MID_MID, NONE,
    NO_INCR, COL_INCR, ROW_INCR, DIAG_DOWN_INCR, DIAG_UP_INCR;

Board placed_cell(Board board, Cell cell, Coords coords);

Cell cell_at(Board board, Coords coords);
bool is_board_full(Board board);
bool is_second_move(Board board);
Coords first_move_coords(Board board);

Winner winner_of(Board board);
bool is_board_finished(Board board);
