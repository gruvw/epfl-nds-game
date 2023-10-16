#pragma once

#include <stdlib.h>
#include "TetrisDefines.h"
#include "../Tools/ArrayTools.h"
#include "TetrisGrid.cpp"

void Grid_init(void);
void Grid_Release(void);
void Grid_setBackGroundDimentions(int w, int h);
int Grid_getColumn(int index);
int Grid_getRow(int index);
bool Grid_IsCellValid(int index);
bool Grid_IsCellFree(int index);
bool Grid_checkForRunOff(int to[]);
bool Grid_tryToMoveCells(int from[], int to[]);
void Grid_paint();
void Grid_ShiftRowBy(int row, int down);
void Grid_SetAllRowTo(int row, bool b);
bool Grid_CheckRowForSame(int row, bool b);
void Grid_MakeGridCollapse(int row);
int Grid_update();