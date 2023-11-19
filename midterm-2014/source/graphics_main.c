#include "graphics_main.h"
#include "game.h"
#include "nds/arm9/video.h"

void configureGraphics_Main() {
	// Configure the MAIN engine in mode 5 activating background 2
    REG_DISPCNT = MODE_5_2D | DISPLAY_BG2_ACTIVE;

	// Configure the VRAM bank A accordingly
    VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;
}

void configBG2_Main() {
	// Configure background BG2 in extended rotoscale mode emulating framebuffer mode
    BGCTRL[2] = BG_BMP_BASE(0) | BgSize_B16_256x256;

    // Set up affine matrix
    REG_BG2PA = 256;
    REG_BG2PC = 0;
    REG_BG2PB = 0;
    REG_BG2PD = 256;
}

void fillRectangle(int left, int right, int top, int bottom, u16 color){

	//Check boundaries of rectangle and return if not correct
	//All points (top, bottom, left and right) must be within the screen boundaries
    if (left < 0 || right < 0 || top < 0 || bottom < 0) return;
    if (left >= SCREEN_WIDTH || right >= SCREEN_WIDTH || top >= SCREEN_HEIGHT || bottom >= SCREEN_HEIGHT) return;
    if (left > right || top > bottom) return;

	//Paint the rectangle
    for (size_t row = top; row <= bottom; row++) {
        for (size_t col = left; col <= right; col++) {
            BG_BMP_RAM(0)[row * SCREEN_WIDTH + col] = color;
        }
    }
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
