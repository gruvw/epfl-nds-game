#include "graphics_main.h"
#include "game.h"
#include "nds/arm9/background.h"
#include "nds/arm9/video.h"
#include "nds/bios.h"

void configureGraphics_Main() {
	// Configure the MAIN engine in mode 5 activating background 2
    REG_DISPCNT = MODE_5_2D | DISPLAY_BG2_ACTIVE | DISPLAY_BG0_ACTIVE;

	// Configure the VRAM bank A accordingly
    VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;
}

void configBG2_Main() {
	// Configure background BG2 in extended rotoscale mode emulating framebuffer mode
    BGCTRL[2] = BG_BMP_BASE(1) | BgSize_B16_256x256;

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
            BG_BMP_RAM(1)[row * SCREEN_WIDTH + col] = color;
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
u8 tile[64] = {
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,0,0,0,
    1,1,1,1,0,0,0,0,
    1,1,1,0,0,0,0,0,
    1,1,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0,
};

void configBG0_Main() {

	//Configure background
    BGCTRL[0] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(1) | BG_TILE_BASE(0);

	//Copy the tile(s)
    swiCopy(tile, BG_TILE_RAM(0), 32);

	//Set color(s) in the palette
    BG_PALETTE[1] = BLUE;

	//Create map
    for (size_t row = 0; row < 24; row +=2) {
        for (size_t col = 0; col < 32; col += 2) {
            BG_MAP_RAM(1)[row*32 + col] = 0;
            BG_MAP_RAM(1)[row*32 + col + 1] = 0 | BIT(10);
            BG_MAP_RAM(1)[(row+1)*32 + col] = 0 | BIT(11);
            BG_MAP_RAM(1)[(row+1)*32 + col + 1] = 0 | BIT(10) | BIT(11);
        }
    }
}
