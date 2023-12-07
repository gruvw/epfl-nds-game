#include "sys/_stdint.h"

typedef uint32_t Board;
typedef unsigned char Coords;

extern const Board START_BOARD, CELL_MASK;
extern const Coords SIDE, END;

#define COORDS(row, col) ((row) *SIDE + (col))

typedef enum {
    EMPTY,
    CROSS,
    CIRCLE,
} Cell;

extern const Coords
    TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, MID_LEFT, MID_TOP,
    NO_INCR, COL_INCR, ROW_INCR, DIAG_DOWN_INCR, DIAG_UP_INCR;

typedef struct {
    Cell side;
    Coords start;
    Coords direction;
} Winner;

Board placed_cell(Board board, Cell cell, Coords coords);

Cell cell_at(Board board, Coords coords);

bool is_empty(Board board, Coords coords);

Winner winner_of(Board board);
