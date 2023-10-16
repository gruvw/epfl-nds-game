
#include <nds.h>
#include <stdio.h>
#include <math.h>
#include "chrono_display.h"


//Exercises 2 and 3
int min, sec, msec;



int main(void) {

	consoleDemoInit();
/*
	//Exercise 1

	u16 start, end;
	int result;

	// Initialize the timer with the correct configuration
	TIMER0_CR = ...

	// Set the TIMER_DATA to 0 (the timer starts to count in 0)
	TIMER_DATA(0) = 0;

	//Read the value of the timer
	start = ...
	result = floor(sqrt((double)start));
	//Read the value of the timer again
	end = ...
	//Print the solution
	printf(...

	//Do the same fot the hand-written square root
	start = ...
	result = iSqrt(start);
	end = ...
	printf(...
*/


	//Exercise 2
/*
	//Initialization of the display
	initChronoDisp_Main();

	//Initialize the interrupts correctly
	...

	//Set the seconds and mseconds to 0
	sec = msec = 0;

	//Configure two timers (i.e.: TIMER0 and TIMER1) correcly
	TIMER_DATA(0) = ...
	TIMER0_CR = ...
	TIMER_DATA(1) = ...
	TIMER1_CR = ...
	
	//Associate the interrupt handlers
	...
	...
	//Enable the interrupt lines of the timers
	...
	...
*/

/*
	//Exercise 3 (Comment exercise 1 and 2)

	//Initialize the display and the color of the display
	initChronoDisp_Main();
	changeColorDisp_Main(RED);

	//Initialize the interrupts correctly
	...

	//Set the minutes, the seconds and the mseconds
	min = sec = msec = 0;

	//Configure the timer (i.e. TIMER0) correctly
	TIMER_DATA(0) = ...
	TIMER0_CR = ...
	
	//Associate the interrupt handler and enable the interrupt line
	...
	...
*/

/*
	//Exercise 4
	
	//Associate the interrupt handler for the VBLANK interrupt line and enable it
	...
	...
*/

/*
	//Exercise 5

	//Configure the timer correctly, associate the handler and enable the interrupt
	...
	...
	...
	...
*/

/*
	//Exercise 6

	//Configure the interruption to be fired when button A is pressed
	REG_KEYCNT = (1<<14)| KEY_A;
	//Associate the handler and enable the line of interrupt
	...
	...
*/


	while(1) {
		swiWaitForVBlank();
	}
}
