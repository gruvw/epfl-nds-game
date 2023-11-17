#include <nds.h>
#include <stdio.h>
#include "background.h"
#include "nds/bios.h"
#include "nds/timers.h"

#define	RED ARGB16(1,31,0,0)
#define GREEN ARGB16(1,0,31,0)
#define	BLUE ARGB16(1,0,0,31)
#define	YELLOW ARGB16(1,31,31,0)
#define	WHITE ARGB16(1,31,31,31)

//Array of colors (can be useful in the implementation)
u16 colors[4] = {RED, GREEN, YELLOW, BLUE};


//Timer ISR (for EXERCISE 3)
int blinking_region = 0;	// Region toggling its color
int ticks = 0;				// Number of calls to the timer ISR
void timer_ISR()
{
	//Account the call to the timer ISR (update ticks)
    ticks++;

	//Toggle the color of the palette corresponding to the blinking region
	//(if odd number of ticks set to to WHITE, otherwise to the corresponding color)
    if (ticks % 2) {
        BG_PALETTE_SUB[blinking_region] = WHITE;
    } else {
        BG_PALETTE_SUB[blinking_region] = colors[blinking_region];
    }


	//Change the blinking region if necessary (once every 2 second or 8 ticks)
    if (!(ticks % 8)) {
        blinking_region = (blinking_region + 1) % 4;
    }
}


u8 upperLeftTile[64] =
{
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
};

u8 upperRightTile[64] =
{
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1
};

u8 lowerLeftTile[64] =
{
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2
};

u8 lowerRightTile[64] =
{
	3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3
};

int main(void) {


	/*************
	 * EXERCISE 1
	 *************/
	// 1) VRAM Configuration for MAIN engine
    VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;

	// 2) Main engine configuration in rotoscale mode
    REG_DISPCNT = MODE_5_2D | DISPLAY_BG2_ACTIVE;

	// 3) Configure the background (Ignore warning 'large implictly truncated to unsigned type')
    BGCTRL[2] = BG_BMP_BASE(0) | BgSize_B8_256x256;

	// 4) Copy bitmap and palette generated by grit
    swiCopy(backgroundBitmap, BG_GFX, backgroundBitmapLen/2);
    swiCopy(backgroundPal, BG_PALETTE, backgroundPalLen/2);

	//Affine Marix Transformation
	bgTransform[2]->hdx = 1*256;
	bgTransform[2]->vdx = 0*256;
	bgTransform[2]->hdy = 0*256;
	bgTransform[2]->vdy = 1*256;
	bgTransform[2]->dx = 0*256;
	bgTransform[2]->dy = 0*256;

	/*************
	 * EXERCISE 2
	 *************/
	///////////////////////////////////////////////////////////////////////////////////////
	//ATTENTION!!!: You may find useful to check the example (slides 15 to 27) given during
	// the week 8 (Graphics III, tiled mode)
	///////////////////////////////////////////////////////////////////////////////////////

	// 1) VRAM configuration for SUB engine
    VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;

	// 2) SUB engine configuration in tiled mode
    REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG0_ACTIVE;

	// 3) Configure the background
    BGCTRL_SUB[0] = BG_MAP_BASE(0) | BG_TILE_BASE(1) | BG_32x32 | BG_COLOR_256;

	// 4) Copy the 4 tiles to the tile base
    swiCopy(upperLeftTile, &BG_TILE_RAM_SUB(1)[0], 32);
    swiCopy(upperRightTile, &BG_TILE_RAM_SUB(1)[32], 32);
    swiCopy(lowerLeftTile, &BG_TILE_RAM_SUB(1)[64], 32);
    swiCopy(lowerRightTile, &BG_TILE_RAM_SUB(1)[96], 32);

	// 5) Initialize the palette (4 components, one per region)
    BG_PALETTE_SUB[0] = RED;
    BG_PALETTE_SUB[1] = GREEN;
    BG_PALETTE_SUB[2] = YELLOW;
    BG_PALETTE_SUB[3] = BLUE;

	// 6) Generate the map
    size_t mid_row = 12, mid_col = 16;
    for (size_t row = 0; row < 12; row++) {
        for (size_t col = 0; col < 16; col++) {
            BG_MAP_RAM_SUB(0)[row * 32 + col] = 0;
            BG_MAP_RAM_SUB(0)[row * 32 + mid_col + col] = 1;
            BG_MAP_RAM_SUB(0)[(row + mid_row) * 32 + col] = 2;
            BG_MAP_RAM_SUB(0)[(row + mid_row) * 32 + mid_col + col] = 3;
        }
    }

	/*************
	 * EXERCISE 3
	 *************/
	///////////////////////////////////////////////////////////////////////////////////////
	//IMPORTANT NOTE!!!: Do not initialize the interrupts with the call to irqInit() since
	// later the touchscreen will be used
	///////////////////////////////////////////////////////////////////////////////////////
	// 1) Configure the timer to raise an interrupt 4 times per second
    TIMER_DATA(0) = TIMER_FREQ_1024(4);
    TIMER_CR(0) = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;

	// 2) Implement the functionality of the timer ISR [already declared outside the main() ]

	// 3) Associate the implemented ISR to the Timer Interrupt Line
    irqSet(IRQ_TIMER(0), &timer_ISR);

	// 4) Enable the timer interrupt
    irqEnable(IRQ_TIMER(0));


	/*************
	 * EXERCISE 4
	 *************/
    while(1){
    	// 1) Scan the keypad
    	//...

    	// 2) Obtain the pressed keys
    	//...

    	// 3) Check if the pressed key is the correct one and then change the blinking region
    	//...

    	// 4) Read the touchscreen position
    	//...

    	// 5) Check if the touched position is on the blinking region and in that case change the blinking region
    	//...

        swiWaitForVBlank();
    }
}
