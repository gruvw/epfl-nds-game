#pragma once

#include <stdlib.h>
#include "TetrisShape.h"
#include <nds/arm9/sound.h>		// sound functions

bool Shape_isGameOver = false;
bool Shape_isInited = false;

int Shape_Orient;
int Shape_Type;
int Shape_NextType;
int Shape_State;
int Shape_Elems[MAX_ELEMS];
int Shape_TempElems[MAX_ELEMS];

void Shape_init(void)
{
    Shape_NextType = rand() % TYPE_MAX_TYPES;
    int i;
    for (i = 0; i < MAX_ELEMS; i++)
    {
        Shape_Elems[i] = DONT_CHECK_CELL;
        Shape_TempElems[i] = DONT_CHECK_CELL;
    }
}

bool Shape_tryToMove(int newElemPos[])
{
    bool hasMoved = Grid_tryToMoveCells(Shape_Elems, newElemPos);
    if (hasMoved)
        Array_cloneInt(newElemPos,Shape_Elems,MAX_ELEMS);
      
    return hasMoved;
}

bool Shape_alignFromOrientation(int orientation)
{
    Array_cloneInt(Shape_Elems,Shape_TempElems,MAX_ELEMS);
    int typeOffset = (Shape_Type * SHAPE_TABLE_TYPE_OFFSET);
    orientation *= SHAPE_TABLE_ELEMS_PER_ROW;
    Shape_TempElems[ELEM_1] = Shape_TempElems[ELEM_BASE] + SHAPE_TABLE[typeOffset + orientation + SHAPE_TABLE_ELEMS_1];
    Shape_TempElems[ELEM_2] = Shape_TempElems[ELEM_BASE] + SHAPE_TABLE[typeOffset + orientation + SHAPE_TABLE_ELEMS_2];
    Shape_TempElems[ELEM_3] = Shape_TempElems[ELEM_BASE] + SHAPE_TABLE[typeOffset + orientation + SHAPE_TABLE_ELEMS_3];

    return Shape_tryToMove(Shape_TempElems);
}



bool Shape_spawn()
{

    int i;
    Shape_isGameOver = false;
    for (i = 0; i < MAX_ELEMS; i++)
    {
        Shape_Elems[i] = DONT_CHECK_CELL;
    }
    Shape_Type = Shape_NextType;
    Shape_NextType = rand() % TYPE_MAX_TYPES;
    Shape_Elems[ELEM_BASE] = START_CELL;
    Shape_Orient = START_ORIENTATION;
    Shape_State = STATE_USER;
    return Shape_alignFromOrientation(Shape_Orient);
}

bool Shape_rotateShape(int rotation)
{
    int orient = Shape_Orient;
    orient += rotation;
    if (orient < OR_NORTH)
        orient = OR_WEST;
    else if (orient > OR_WEST)
        orient = OR_NORTH;
    bool hasRotated = Shape_alignFromOrientation(orient);
    if (hasRotated)
        Shape_Orient = orient;

    return hasRotated;
}


bool Shape_shiftCells(int cellscrool)
{
    int i;
    Array_cloneInt(Shape_Elems, Shape_TempElems, MAX_ELEMS);
 
    
    for (i = 0; i < MAX_ELEMS; i++)
    {
        if (Shape_Elems[i] != DONT_CHECK_CELL)
            Shape_TempElems[i] += cellscrool;
    }

    if (cellscrool == C_LEFT || cellscrool == C_RIGHT)
        if (Grid_getRow(Shape_Elems[ELEM_BASE]) != Grid_getRow(Shape_TempElems[ELEM_BASE]))
            return false;

    return Shape_tryToMove(Shape_TempElems);
}

void Shape_update(int currentAction)
{
    if (Shape_State == STATE_USER)
    {
        switch (currentAction)
        {
            case ACTION_STRAFE_LEFT:
                {
                    Shape_shiftCells(C_LEFT);
                    break;
                }
            case ACTION_STRAFE_RIGHT:
                {
                    Shape_shiftCells(C_RIGHT);
                    break;
                }
            case ACTION_ROTATE_L:
                {
                    if (Shape_rotateShape(ROT_LEFT))
                    ;
					Audio_PlaySound( eSFX_SWISH );
                    //playGenericSound(swish_raw_bin, swish_raw_bin_size);
                    break;
                }
            case ACTION_ROTATE_R:
                {
                    if (Shape_rotateShape(ROT_RIGHT))
                    ;
					Audio_PlaySound( eSFX_SWISH );
					//playGenericSound(swish_raw_bin, swish_raw_bin_size);
                    break;
                }
            case ACTION_MAKE_FALL:
                {
                    Shape_State = STATE_FALLING;
					Audio_PlaySound( eSFX_CLUNK );
                    //playGenericSound(clunk_raw_bin, clunk_raw_bin_size);
                    break;
                }
        }
    }
}

bool Shape_IsFalling()
{
    return Shape_State == STATE_FALLING;
}

//true when need to recheck playfield
bool Shape_addGravity()
{
    if (Shape_isInited)
    {
        //gravity
        bool falling = Shape_shiftCells(C_DOWN);
        if (!falling)
        {
            Shape_State = STATE_LOCKED;
            Shape_isInited = false;
            return true;
        }
    }
    else
    {
        Shape_isInited = Shape_spawn();

        if (!Shape_isInited)//this means no room to init = game over
        {
            Shape_isGameOver = true;
        }
    }
    return false;
}