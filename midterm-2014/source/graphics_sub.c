#include "graphics_sub.h"
#include "game.h"

void configureGraphics_Sub() {
	// Configure the MAIN engine in mode 5 and activate background 2
	//... TO COMPLETE EXERCISE 1
	// Configure the VRAM bank C accordingly
	//... TO COMPLETE EXERCISE 1
}


void configBG2_Sub() {
	// Configure background BG2 in extended rotoscale mode using 8bit pixels
	//... TO COMPLETE EXERCISE 1

	// Transfer image and palette to the corresponding memory locations
	//... TO COMPLETE EXERCISE 1

    // Set up affine matrix
    REG_BG2PA_SUB = 256;
    REG_BG2PC_SUB = 0;
    REG_BG2PB_SUB = 0;
    REG_BG2PD_SUB = 256;
}
