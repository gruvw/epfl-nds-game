#pragma once

#include "TetrisGameplay.h"

//game specific
//private TetrisShape currentShape;	//current shape controllable by the user
int Gameplay_currentAction;			//current game action fired by player
int Gameplay_playerScore;
char* Gameplay_playerName;

//members
unsigned int Gameplay_ticks; 	    //time stamp when next update can be called
unsigned int Gameplay_LastGravity;	//allow updates of shape independently from gravity by checking this


/**
 * Initialize members
 */
void Gameplay_init(void) {
    //initialize members
    Shape_isInited = false;
    Gameplay_currentAction = ACTION_NONE;
    Gameplay_ticks = 0;
    Gameplay_LastGravity = 1;
    Gameplay_playerScore = 0;

    Grid_init();
}

/**
 * Constructor
 */
void Gameplay_Load(void)
{
    //inst objs
    Grid_setBackGroundDimentions(256,192);
    Shape_init();
	
    //initialize
    Gameplay_init();
}

void Gameplay_restartGame(void) 
{
    Gameplay_init();
    Shape_isGameOver = false;
}

/**
 * Handle key presses (make sure view is focusable)
 */
void Gameplay_handleInput(enum ACTION a)
{
    int actionToSend = -1;
    switch (a)
    {
        case UP:
            actionToSend = ACTION_ROTATE_L;
            break;
        case DOWN:
            actionToSend = ACTION_ROTATE_R;
            break;
        case RIGHT:
            actionToSend = ACTION_STRAFE_RIGHT;
            break;
        case LEFT:
            actionToSend = ACTION_STRAFE_LEFT;
            break;
        case SELECT:
            actionToSend = ACTION_MAKE_FALL;
            break;
        default:
            break;
    }

    if (actionToSend != -1)
        Gameplay_currentAction = actionToSend;
}


/**
 * Update user actions
 * Update engine actions (gravity and line check)
 */
void Gameplay_update(void) 
{
	Gameplay_ticks++;
    //manage gameOver
    if(Shape_isGameOver)
    {
        //iprintf("GameOver\n");
        //bool isNewHighScore = TetrisGame.highScore.IsHighScore(playerName, playerScore) && playerScore > 0;
        //TetrisGame.soundPlayer.FireSoundFX(isNewHighScore ? SOUND_FX.Win : SOUND_FX.Lose);
        //TetrisGame.menu.setMenuState(TetrisMenu.MENU_STATE.STATE_MENUGAMEOVER, isNewHighScore );

    }
    //normal state
    else 
    {
        Shape_update(Gameplay_currentAction);
	    Gameplay_currentAction = ACTION_NONE;//consume action
	    
        if (Gameplay_ticks > Gameplay_LastGravity  || Shape_IsFalling())
	    {
            //GRAVITY_RATE - playerScore makes it increasingly harder!
		    Gameplay_LastGravity += GRAVITY_RATE - (Gameplay_playerScore / 10);
		    bool shapeIsLocked = Shape_addGravity();
		    
		    if(shapeIsLocked)
		    {
                //update returns the number of points
			    int points = Grid_update();
                if (points != 0)
                {
                    //play sound
                    Gameplay_playerScore += points;
                }
		    }
	    }
    }
    return;
}

void Gameplay_displayNextShape(int nextType)
{
    switch(nextType)
    {
        case TYPE_LONG   :
		    iprintf("----");
            break;
        case TYPE_BL     :
		    iprintf("_|  ");
            break;
        case TYPE_L      :
		    iprintf("L   ");
            break;
        case TYPE_SQUARE :
		    iprintf("O   ");
            break;
        case TYPE_S      :
		    iprintf("S   ");
            break;
        case TYPE_BS     :
		    iprintf("Z   ");
            break;
        case TYPE_T      :
		    iprintf("T   ");
            break;
    }
}

/**
 * Paint game
 */
void Gameplay_draw() 
{
    //paint elements
    //Grid_paint(); //not necessary, grid is painted directly by the Map16x16 tile engine
	
    //paint HUD from top right anchordisplayNextShape
    Gameplay_displayNextShape(Shape_NextType);
}