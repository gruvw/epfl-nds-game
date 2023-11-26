#include "P_Controls.h"
#include "Tetris/TetrisGameplay.h"

void handleTouchPad()
{
    // Read the touch position.
	touchPosition touch;
	touchRead(&touch);

	//Only if px or py are different to 0 handle the touch
	if(touch.px || touch.py)
	{
		//Rotation
		if(touch.px > 170 && touch.px < 256)
		{
			if(touch.py > 0 && touch.py < 96)
				Gameplay_handleInput(UP);
			else
				Gameplay_handleInput(DOWN);
		}
		//Lateral movement
		else if(touch.px > 85 && touch.px < 170)
		{
			if(touch.py > 0 && touch.py < 96)
				Gameplay_handleInput(LEFT);
			else
				Gameplay_handleInput(RIGHT);

		}
		//Block fall down
		else if(touch.px > 0 && touch.px < 85)
		{
			Gameplay_handleInput(SELECT);
		}
	}
}


/*
 * This function handles the input by monitoring the keys and the touchscreen
 * and invoking the function Gameplay_HandleInput(...) with the correct input
 * value
 */
void handleInput()
{
	//Scan the keys
	scanKeys();

	/* Identify the keys and map the action according to the table given in
	 * the manual. Call the function Gameplay_handleInput(..) to send an action
	 * to the logic of the game */
	u16 keys = keysDown();
    if(keys & KEY_A)
        Gameplay_handleInput(SELECT);
    else if(keys & KEY_LEFT)
        Gameplay_handleInput(UP);
    else if(keys & KEY_RIGHT)
        Gameplay_handleInput(DOWN);
    else if (keys & KEY_UP)
        Gameplay_handleInput(LEFT);
    else if (keys & KEY_DOWN)
        Gameplay_handleInput(RIGHT);


    //Call the touch-handling function
   	handleTouchPad();

    //Update the game and redraw the blocks
	Gameplay_update();
	Gameplay_draw();
}
