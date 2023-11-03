#pragma once

//remember, hud is pixel aligned from top, bottom, left or right 
//but center is dynamic depending on screen size

//constants

//GAME
//frames per second
#define FRAME_RATE 60
#define DIFFICULTY 3
//delay before reupdating
#define OUT_OF_PAUSE_DELAY (FRAME_RATE*2)
//only first keypress counts for game actions.
#define DISREGARD_MULTIPLE_KEYPRESSED true

//game actions
#define ACTION_NONE 0
#define ACTION_STRAFE_LEFT 1
#define ACTION_STRAFE_RIGHT 2
#define ACTION_ROTATE_L 3
#define ACTION_ROTATE_R 4
#define ACTION_MAKE_FALL 5

//HUD
//does playfield leave space for a margin
#define PLAYFIELD_USE_MARGINS true
//in px 
#define MARGIN_TOP 36
#define MARGIN_LEFT 12
#define MARGIN_RIGHT 64
#define MARGIN_BOTTOM 36
//ANCHORED TO THE RIGHT!
#define HUD_SCORE_TEXT_OFFSET 5
#define HUD_SCORE_Y_START 40
#define HUD_SCORE_INTERLINE 30
//static Color HUD_SCORE_WORD_COLOR Color.White
//static Color HUD_SCORE_NUM_COLOR Color.Yellow
#define HUD_NEXT_WORD_Y_START (HUD_SCORE_Y_START+HUD_SCORE_INTERLINE+30)
#define HUD_NEXT_TEXT_OFFSET HUD_SCORE_TEXT_OFFSET
#define HUD_NEXT_SHAPE_X_START 50
#define HUD_NEXT_SHAPE_Y_START (HUD_NEXT_WORD_Y_START+50)
#define HUD_NEXT_SHAPE_CELL_SIZE 16
#define HUD_NEXT_SHAPE_CELL_OFFSET (HUD_NEXT_SHAPE_CELL_SIZE+3)
//static Color HUD_NEXT_WORD_COLOR Color.White
//static Color HUD_NEXT_SHAPE_COLOR Color.Gray
#define HUD_TOP_SCORES_Y_START (HUD_NEXT_WORD_Y_START+100)
#define HUD_TOP_SCORES_TEXT_OFFSET HUD_SCORE_TEXT_OFFSET
//static Color HUD_TOP_SCORES_TITLE_COLOR Color.White
//static Color HUD_TOP_SCORES_RANK_COLOR Color.Green
//static Color HUD_TOP_SCORES_WORD_COLOR Color.Green
//static Color HUD_TOP_SCORES_SCORE_COLOR Color.White
#define HUD_TOP_SCORES_RANK_OFFSET 45
#define HUD_TOP_SCORES_WORD_OFFSET 25



//PLAYFIELD
//grid size
#define PLAYFIELD_COLS 12
#define PLAYFIELD_ROWS 16
#define PLAYFIELD_CELLNB (PLAYFIELD_ROWS * PLAYFIELD_COLS)

//ticks before engine calls gravity
#define GRAVITY_RATE (FRAME_RATE/2)

//cell where new shapes apear
#define START_CELL ((PLAYFIELD_COLS/2)-1)
//cellscroll directions
#define C_CENTER 0
#define C_LEFT (-1)
#define C_UP (-PLAYFIELD_COLS)
#define C_RIGHT 1
#define C_DOWN PLAYFIELD_COLS


//SHAPES
//a value that lets playfield skip the test of shape cells
#define DONT_CHECK_CELL (-1000)

//states
#define STATE_USER    0
#define STATE_LOCKED  1
#define STATE_FALLING 2

//rotations
#define ROT_LEFT  (-1)
#define ROT_RIGHT  1

//orientations
#define OR_NORTH 0
#define OR_EAST  1
#define OR_SOUTH 2
#define OR_WEST  3
#define START_ORIENTATION OR_EAST


//max elems
#define ELEM_BASE 0
#define ELEM_1 	  1
#define ELEM_2    2
#define ELEM_3    3
#define MAX_ELEMS 4
//shape types
#define TYPE_LONG 		0
#define TYPE_BL   		1
#define TYPE_L 	  		2
#define TYPE_SQUARE 	3
#define TYPE_S 			4
#define TYPE_BS 		5
#define TYPE_T 			6
#define TYPE_MAX_TYPES	7


enum ACTION
{
    NONE,
    EXIT,
    LEFT,
    DOWN,
    UP,
    RIGHT,
    SELECT,
};

//TABLES

//this table contains all the shape types constructs for each orientation
#define SHAPE_TABLE_ELEMS_1 	  0
#define SHAPE_TABLE_ELEMS_2 	  1
#define SHAPE_TABLE_ELEMS_3 	  2
#define SHAPE_TABLE_ELEMS_PER_ROW 3
#define SHAPE_TABLE_ROWS_PER_TYPE 4
#define SHAPE_TABLE_TYPE_OFFSET	  (SHAPE_TABLE_ROWS_PER_TYPE*SHAPE_TABLE_ELEMS_PER_ROW)
int SHAPE_TABLE[] =
{
    //TYPE LONG 0
    //OR_NORTH (elem1,2,3, mincol, maxcol)
    C_UP, C_DOWN, C_UP*2, //I
    //OR_EAST (elem1,2,3)
    C_LEFT, C_RIGHT, C_RIGHT*2, //---
    //OR_SOUTH (elem1,2,3)
    C_UP, C_DOWN, C_DOWN*2, //I
    //OR_WEST (elem1,2,3)
    C_LEFT, C_RIGHT, C_LEFT*2,//---
	
    //TYPE backwards L 1
    //OR_NORTH (elem1,2,3, mincol, maxcol)
    C_UP, C_DOWN, C_UP+C_RIGHT,//backwards L
    //OR_EAST (elem1,2,3)
    C_LEFT, C_RIGHT, C_RIGHT+C_DOWN,//--|
    //OR_SOUTH (elem1,2,3)
    C_UP, C_DOWN, C_DOWN+C_LEFT,//inverse L
    //OR_WEST (elem1,2,3)
    C_LEFT, C_RIGHT, C_LEFT+C_UP,//|--

    //TYPE L 2
    //OR_NORTH (elem1,2,3, mincol, maxcol)
    C_UP, C_DOWN, C_UP+C_LEFT,//L
    //OR_EAST (elem1,2,3)
    C_LEFT, C_RIGHT, C_RIGHT+C_UP,//--|
    //OR_SOUTH (elem1,2,3)
    C_UP, C_DOWN, C_DOWN+C_RIGHT,//L
    //OR_WEST (elem1,2,3)
    C_LEFT, C_RIGHT, C_LEFT+C_DOWN,//|--
	
    //TYPE SQUARE 3
    //OR_NORTH (elem1,2,3, mincol, maxcol)
    C_RIGHT, C_DOWN, C_RIGHT+C_DOWN,//O
    //OR_EAST (elem1,2,3)
    C_RIGHT, C_DOWN, C_RIGHT+C_DOWN,//O
    //OR_SOUTH (elem1,2,3)
    C_RIGHT, C_DOWN, C_RIGHT+C_DOWN,//O
    //OR_WEST (elem1,2,3)
    C_RIGHT, C_DOWN, C_RIGHT+C_DOWN,//O

    //TYPE S 4
    //OR_NORTH (elem1,2,3, mincol, maxcol)
    C_DOWN+C_LEFT, C_DOWN, C_RIGHT,//_|-
    //OR_EAST (elem1,2,3)
    C_UP, C_RIGHT, C_RIGHT+C_DOWN,//|-i
    //OR_SOUTH (elem1,2,3)
    C_DOWN+C_LEFT, C_DOWN, C_RIGHT,//_|-
    //OR_WEST (elem1,2,3)
    C_UP, C_RIGHT, C_RIGHT+C_DOWN,//|-i

    //TYPE backwardsS 4
    //OR_NORTH (elem1,2,3, mincol, maxcol)
    C_LEFT, C_DOWN, C_DOWN+C_RIGHT,//-|_
    //OR_EAST (elem1,2,3)
    C_UP+C_RIGHT, C_RIGHT, C_DOWN,//i-|
    //OR_SOUTH (elem1,2,3)
    C_LEFT, C_DOWN, C_DOWN+C_RIGHT,//-|_
    //OR_WEST (elem1,2,3)
    C_UP+C_RIGHT, C_RIGHT, C_DOWN,//i-|
	
    //TYPE T 5
    //OR_NORTH (elem1,2,3, mincol, maxcol)
    C_UP, C_DOWN, C_RIGHT,//T
    //OR_EAST (elem1,2,3)
    C_LEFT, C_DOWN, C_RIGHT,//T
    //OR_SOUTH (elem1,2,3)
    C_LEFT, C_DOWN, C_UP,//T
    //OR_WEST (elem1,2,3)
    C_LEFT, C_UP, C_RIGHT,//T
};