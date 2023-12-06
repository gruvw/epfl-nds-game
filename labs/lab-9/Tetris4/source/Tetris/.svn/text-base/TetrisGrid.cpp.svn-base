#pragma once

#include "TetrisGrid.h"

bool Grid_Cells[PLAYFIELD_CELLNB];
int Grid_TileW;
int Grid_TileH;
int Grid_Left;
int Grid_Top;
int Grid_Right;
int Grid_Bottom;

void Grid_init(void)
{
    int i;
    for (i = 0; i < PLAYFIELD_CELLNB; i++)
    {
        Grid_Cells[i] = false;
        SetMap16x16To(i,false);
    }
}

void Grid_setBackGroundDimentions(int w, int h)
{
	Grid_Left = MARGIN_LEFT;
	Grid_Top = MARGIN_TOP;
	w -= Grid_Left + MARGIN_RIGHT;
	h -= Grid_Top  + MARGIN_BOTTOM;
	Grid_Right  = Grid_Left + w;
	Grid_Bottom = Grid_Top  + h;
	
	Grid_TileW = w / PLAYFIELD_COLS;
	Grid_TileH = h / PLAYFIELD_ROWS;
}

int Grid_getColumn(int index)
{
    if (index < 0)
        return -((abs(index) % PLAYFIELD_COLS) + 1);
    else
        return (index % PLAYFIELD_COLS);
}

int Grid_getRow(int index)
{
    if (index < 0)
        return -((abs(index) / PLAYFIELD_COLS) + 1);
    else
        return (index / PLAYFIELD_COLS);
}

bool Grid_IsCellValid(int index)
{
    return (index >= 0 && index < PLAYFIELD_CELLNB);
}

bool Grid_IsCellFree(int index)
{
    if (Grid_IsCellValid(index))
        return !Grid_Cells[index];
    else
        return false;
}

bool Grid_checkForRunOff(int to[])
{
    bool hasRunOff = true;
    int i;
    int testArray[MAX_ELEMS];
    
    Array_cloneInt(to,testArray,MAX_ELEMS);
    for (i = MAX_ELEMS - 1; i >= 0; i--)
    {
        testArray[i] -= testArray[0];
        testArray[i] += START_CELL;
    }

    //normalize the test array to test rowDiff
    for (i = 0; i < MAX_ELEMS; i++)
    {
        if (Grid_getRow(to[i]) - Grid_getRow(to[0]) != Grid_getRow(testArray[i]) - Grid_getRow(testArray[0]))
        {
            hasRunOff = false;
            break;
        }
    }
    
    return hasRunOff;
}

bool Grid_tryToMoveCells(int from[], int to[])
{
    
    //test grid
    if (!Grid_checkForRunOff(to))
        return false;
    
    bool validMove = false;
    int i;
    for (i = 0; i < MAX_ELEMS; i++)
    {
        
        bool cellAboveGrid = to[i] < 0; //can happen on init
        if (!Array_IsInArray(to[i], from, MAX_ELEMS))
        {
            if (Grid_IsCellFree(to[i]) || cellAboveGrid)
                validMove = true;
            else
                return false;
        }
    }
    

    //write to grid
    if (validMove)
    {
    
        for (i = 0; i < MAX_ELEMS; i++)
        {
            if (Grid_IsCellValid(from[i]))
            {
                Grid_Cells[from[i]] = false;
                SetMap16x16To(from[i],false);
            }
        }
        for (i = 0; i < MAX_ELEMS; i++)
        {
            bool cellAboveGrid = to[i] < 0; //can happen on init
            if (!cellAboveGrid)
            {
                Grid_Cells[to[i]] = true;
                SetMap16x16To(to[i],true);
            }
        }
    }

    return validMove;
}

/*
void Grid_paint()
{
    //paint bg
    //FillRectangle(MAIN, Grid_Top, Grid_Bottom, Grid_Left, Grid_Right, RGB15(10,10,10) | BIT(15));
	
    //paint elems
    int l, t;
    int i;
    for (i = 0; i < PLAYFIELD_CELLNB; i++)
    {
        l = Grid_Left + (i % PLAYFIELD_COLS) * Grid_TileW;
        t = Grid_Top + (i / PLAYFIELD_COLS) * Grid_TileH;
        //Rectangle cell = new Rectangle(l, t, mTileW, mTileH);
        //TetrisGame.ShapeManager.DrawRect(TetrisGame.Pixel, TetrisGame.spriteBatch, cell, Color.Yellow);
        //DrawRectangle(MAIN, t, t+Grid_TileH, l, l+Grid_TileW, RGB15(10,25,10) | BIT(15));

        //if(Grid_Cells[i])//more fancy graphics for occupied cells
        {
            //TetrisGame.spriteBatch.Draw(TetrisGame.Pixel, cell, Color.Yellow);
        }
    }

    //TetrisGame.ShapeManager.DrawRect(TetrisGame.Pixel, TetrisGame.spriteBatch, background, Color.Red);

}*/


void Grid_ShiftRowBy(int row, int down)
{
    int index;
    int i;
    for (i = 0; i < PLAYFIELD_COLS; i++)
    {
        index = (row * PLAYFIELD_COLS) + i;
        Grid_Cells[index + down] = Grid_Cells[index];
        SetMap16x16To(index + down,Grid_Cells[index]);
    }

}

void Grid_SetAllRowTo(int row, bool b)
{
    int i;
    for (i = 0; i < PLAYFIELD_COLS; i++)
    {
        Grid_Cells[(row * PLAYFIELD_COLS) + i] = b;
        SetMap16x16To((row * PLAYFIELD_COLS) + i,b);
    }
}

bool Grid_CheckRowForSame(int row, bool b)
{
    int i;
    for (i = 0; i < PLAYFIELD_COLS; i++)
    {
        if (Grid_Cells[(row * PLAYFIELD_COLS) + i] != b)
            return false;
    }
    return true;
}

void Grid_MakeGridCollapse(int row)
{
    int r;
    for (r = row; r >= 0; r--)
    {
        if (Grid_CheckRowForSame(r, false))
            break;
        Grid_ShiftRowBy(r, C_DOWN);
        Grid_SetAllRowTo(r, false);
    }
}

int Grid_update()
{
    int points = 0;
    int row;
    for (row = PLAYFIELD_ROWS - 1; row >= 0; row--)
    {
        if (Grid_CheckRowForSame(row, false))
            break;
        if (Grid_CheckRowForSame(row, true))
        {
            points++;
            Grid_SetAllRowTo(row, false);
            Grid_MakeGridCollapse(row - 1);
            row++;//we collapsed grid onto this row so we need to check it again by cancelling the upcomming decrement
        }
    }
    return points;
}