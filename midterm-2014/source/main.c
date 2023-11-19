#include <nds.h>
#include "graphics_main.h"
#include "graphics_sub.h"
#include "timer_management.h"
#include "game.h"

void exercise_1() { //DO NOT modify this function
	configureGraphics_Sub();
	configBG2_Sub();
}

void exercise_2() { //DO NOT modify this function
	int i;
	configureGraphics_Main();
	configBG2_Main();
	configBG0_Main();

	fillRectangle(-1,138,0,20, ARGB16(1,31,0,0));
	fillRectangle(250,257,0,20, ARGB16(1,31,0,0));
	fillRectangle(250,254,180,195, ARGB16(1,31,0,0));
	fillRectangle(250,254,-5,50, ARGB16(1,31,0,0));
	fillRectangle(0,255,0,191, ARGB16(1,31,16,0));
	fillRectangle(118,138,0,20, ARGB16(1,31,31,31));
	for(i=0; i<12;i++){
		fillRectangle(i*16,(i+1)*16-1,i*16,(i+1)*16-1, ARGB16(1,31,31,31));
		fillRectangle((15-i)*16,(16-i)*16-1,i*16,(i+1)*16-1, ARGB16(1,31,31,31));
	}
}

void exercise_3() { //DO NOT modify this function
	initTimer();
}


void exercise_4() {
	//Randomizer initialization -> DO NOT modify this line
	srand(time(NULL));
	// Game initialization -> DO NOT modify this line
	initGame();
	// Timer initialization -> DO NOT modify this line
	initTimer();

	while(1) {
		// Scan the keys that have been pressed down
		//... TO COMPLETE EXERCISE 4

		// Check if the player has pressed START
		// In that case restart the timer (initTimer) and the game (initGame)
		//... TO COMPLETE EXERCISE 4

		// Check if the touchscreen has been touched and if YES get the coordinates
		// if WHITE region touched, call playerPlaysWhite()
		// if BLACK region touched, call playerPlaysBlack()
		// if touch is not in one of those regions, do nothing
		//... TO COMPLETE EXERCISE 4


		swiWaitForVBlank();
	}
}


int main(void) {
	// EXERCISE 1: Uncomment the following line
	//exercise_1();

	// EXERCISE 2: Uncomment the following line (DO NOT comment the previous ones)
	//exercise_2();

	// EXERCISE 3: Uncomment the following line (DO NOT comment the previous ones)
	//exercise_3();

	// EXERCISE 4: Uncomment the following line (DO NOT comment the previous ones)
	//exercise_4();

	while(1)
		swiWaitForVBlank();
}
