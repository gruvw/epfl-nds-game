#include "graphics_main.h"
#include "game.h"

void configureGraphics_Main() {
	// Configure the MAIN engine in mode 5 activating background 2
	//... TO COMPLETE EXERCISE 2
	// Configure the VRAM bank A accordingly
	//... TO COMPLETE EXERCISE 2
}

void configBG2_Main() {
	// Configure background BG2 in extended rotoscale mode emulating framebuffer mode
	//... TO COMPLETE EXERCISE 2

    // Set up affine matrix
    REG_BG2PA = 256;
    REG_BG2PC = 0;
    REG_BG2PB = 0;
    REG_BG2PD = 256;
}

void fillRectangle(int left, int right, int top, int bottom, u16 color){

	//Check boundaries of rectangle and return if not correct
	//All points (top, bottom, left and right) must be within the screen boundaries
	//... TO COMPLETE EXERCISE 2

	//Paint the rectangle
	//... TO COMPLETE EXERCISE 2
	/*
		Example: top=1 bottom=2 left=1 right=2 should paint a 2x2 pixels rectangle
		in the top left corner of the screen as in the following diagram

						Column
					0	1	2	3	... W-1
		 Row 	0	-	-	-	-		-
		 Row	1	-	#	#	-		-
		 Row	2	-	#	#	-		-
		 Row	3	-	-	-	-		-
		 ...					   ...
		 Row   H-1	-	-	-	-		-
	*/
}


// Custom tile
//... TO COMPLETE EXERCISE 5


void configBG0_Main() {

	//Configure background
	//... TO COMPLETE EXERCISE 5

	//Copy the tile(s)
	//... TO COMPLETE EXERCISE 5

	//Set color(s) in the palette
	//... TO COMPLETE EXERCISE 5

	//Create map
	//... TO COMPLETE EXERCISE 5
}

