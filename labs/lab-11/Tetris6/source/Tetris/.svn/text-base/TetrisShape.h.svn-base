#pragma once

#include <stdlib.h>
#include "TetrisDefines.h"
#include "TetrisGrid.h"
#include "../Tools/ArrayTools.h"
#include "TetrisShape.cpp"


void Shape_init(void);
bool Shape_tryToMove(int newElemPos[]);
bool Shape_alignFromOrientation(int orientation);
bool Shape_spawn();
bool Shape_rotateShape(int rotation);
bool Shape_shiftCells(int cellscrool);
void Shape_update(int currentAction);
bool Shape_IsFalling();
bool Shape_addGravity();