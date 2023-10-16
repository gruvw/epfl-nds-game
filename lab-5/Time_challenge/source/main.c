
#include <nds.h>
#include <stdlib.h>
#include "chrono_display.h"
#include "game.h"
#include <maxmod9.h>

//Random ticks to wait to play the sound and start the game
int rand_ticks;

//Game state variable
int gameState = 0;

/*
 * TIMER0 Interrupt Handler:
 * TIMER0 is the responsible of counting down the random time between
 * the Start button is pressed and the chronometers starts to count with
 * the sound effect. This time is indicated by the variable rand_ticks that
 * store the remaining ticks to play the sound and start the chronometers.
 *
 * This handler decreases by one the variable rand_ticks each time it is
 * called. When rand_ticks reaches the value 0, TIMER0 should be disabled
 * and the function that start the chronometers (startChronometers)
 * must be called.
 */
void ISR_Timer0()
{
	/**TO COMPLETE**/

}


/*
 * Variables that store the minutes, seconds and milliseconds coresponding
 * to player 1
 */
int min = 0, sec = 0, msec = 0;
/*
 * TIER1 handler.
 * This handler is called each 1 ms. It updates the value of the variables
 * 'min', 'sec' and 'msec'. (Exercise 3 in the practical session)
 */
void ISR_Timer1()
{
	/**TO COMPLETE**/
}


/*
 * Variables that store the minutes, seconds and milliseconds coresponding
 * to player 2
 */
int min2 = 0, sec2 = 0, msec2 = 0;
/*
 * TIMER2 Interrupt Handler
 * This handler is called every 1 ms and updates msec2, sec2 and min2.
 * (Like for timer 1 but with variables of player 2)
 */
void ISR_Timer2()
{
	/**TO COMPLETE**/
}

/*
 * Keys interrupt handler
 */
void ISR_Keys()
{
	u16 keys = ~(REG_KEYINPUT);

	//START KEY = Restart the game
	if(keys & KEY_START)
	{
		//Stop (disable) TIMERS 1 and 2
	/**TO COMPLETE**/

		//Set the displays to 0 and yellow
		min = sec = msec = 0;
		min2 = sec2 = msec2 = 0;
		changeColorDisp_Main(YELLOW);
		changeColorDisp_Sub(YELLOW);
		//Change the state of the game
		gameState = 1;
		//Set the time to wait until the alarm fires
		rand_ticks = 20 + (rand() % 40);
		//Start TIMER0 to count the random time
	/**TO COMPLETE**/

	}

	//KEY A = Player 1 pressed
	if(keys & KEY_A)
	{
		//Stop player 2 TIME1
	/**TO COMPLETE**/

		/*
		 * If game state is 1, player 1 pressed the key before expected,
		 * both player loose. The active timers should be stopped and the
		 * displays should turn its color to red. The game state returns to 0
		 */
		if(gameState == 1)
		{
	/**TO COMPLETE**/

		}


		/*
		 * If game state is 2 then player 1 pressed first and wins. Its
		 * display should turn to color green and player 2 display should
		 * turn to color red. Player 1's timer should be stopped. The game
		 * state returns to 0
		 */
		if(gameState == 2)
		{
	/**TO COMPLETE**/


		}
	}

	//KEY LEFT = Player 2 pressed
	if(keys & KEY_LEFT)
	{
		/*
		 * The actions to be taken are the same than in previous piece of
		 * code corresponding to player 1 but this time player 2 is the
		 * one that has pressed first.
		 */
		irqDisable(IRQ_TIMER2);
		if(gameState == 1)
		{
	/**TO COMPLETE**/

		}

		if(gameState == 2)
		{
	/**TO COMPLETE**/

		}
	}
}

/*
 * Interrupt handler for VBlank.
 * The chronometer displays must be updated correctly
 */
void ISR_VBlank()
{
	/**TO COMPLETE**/

}


void startChronometers()
{
	//Only if game state is 1
	if(gameState == 1)
	{
		//Game state turns to 2
		gameState = 2;

		//Players' timers (TIMER1 and TIMER2) are activated
	/**TO COMPLETE**/

		//Sound effect is played
		playSoundEffect();
	}
}

//---------------
int main(void) {
//---------------
	/*
	 * Game initialization
	 */
	gameInit();

	/*
	 * Timers initialization
	 * Timer0 fires an interrupt every 100 ms
	 * Timer1 and Timer2 fire and interruption every 1 second
	 * Choose an suitable divider
	 */
	/**TO COMPLETE**/


	/*
	 * Irq Handlers set up
	 * Associate handlers for Timer0, Timer1, Timer2, VBlank, and Keys
	 */
	/**TO COMPLETE**/


	/*
	 * Initial Irq Enable (VBlank and keys)
	 * Enable the interrupts for VBlank and the keys
	 */
	/**TO COMPLETE**/

	while(1) {
		swiWaitForVBlank();
	}
}
