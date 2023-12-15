#include <stdbool.h>
#include "sys/_stdint.h"

#define COORDS(row, col) ((row) * SIDE + (col))
#define ROW(coords) ((coords) / SIDE)
#define COL(coords) ((coords) % SIDE)

typedef uint32_t Board;
typedef unsigned char Coords;

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

extern const Board START_BOARD, CELL_MASK;

extern const Coords SIDE, END;

extern const Coords
    TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, MID_LEFT, MID_TOP,
    NO_INCR, COL_INCR, ROW_INCR, DIAG_DOWN_INCR, DIAG_UP_INCR;

Board placed_cell(Board board, Cell cell, Coords coords);

Cell cell_at(Board board, Coords coords);

bool is_empty(Board board, Coords coords);

Winner winner_of(Board board);
