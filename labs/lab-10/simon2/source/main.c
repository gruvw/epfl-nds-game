#include <nds.h>
#include <stdio.h>
#include "quarter.h"
#include "math.h"
#include "chrono_display.h"

#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"

//Define to use paletes easily
#define PALETTE(x)	(x << 12)

//Buttons of the simon game
typedef enum{RED,YELLOW,BLUE,GREEN,NONE}button;

//Active button
button active_button = NONE;

//Time variables (for the chronometer)
int min = 0, sec = 0, msec = 0;

//Timer 2 ISR (chronometer)
void TIMER2_ISR()
{
	msec = (msec + 1) % 1000;
	if(!msec)
	{
		sec = (sec + 1) % 60;
		if(!sec)
			min++;
	}
}

//Timer0 ISR (blinking effect of the buttons)
void TIMER0_ISR()
{
	//It is necessary to wait until the screen is refreshed...
	swiWaitForVBlank();

	//Switching between light and dark color
	switch(active_button)
	{
	case RED: BG_PALETTE_SUB[17] ^= 0x0B;break;
	case GREEN: BG_PALETTE_SUB[1] ^= (0x0B << 5);break;
	case BLUE: BG_PALETTE_SUB[49] ^= (0x0B << 10) | (0x5<<5);break;
	case YELLOW: BG_PALETTE_SUB[33] ^= 0x0B | (0xB<<5);break;
	default:
		BG_PALETTE_SUB[1] = ARGB16(1,0,20,0);
		BG_PALETTE_SUB[17] = ARGB16(1,20,0,0);
		BG_PALETTE_SUB[33] = ARGB16(1,20,20,0);
		BG_PALETTE_SUB[49] = ARGB16(1,0,0,20);
		break;
	}
}

//Update the chronometer every VBlank
void VBLANK_ISR()
{
	updateChronoDisp_Main(min,sec,msec);
}

//-----------------------------------------------------------------------------
int main(void) {
//-----------------------------------------------------------------------------
	//SOUND
	//Init the sound library
	mmInitDefaultMem((mm_addr)soundbank_bin);
	//Load module
	mmLoad(MOD_MUSIC);
	//Load effect
	mmLoadEffect(SFX_LASER);
	mmLoadEffect(SFX_RESULT);


	//GRAPHICS
	//Enable the VRAM bank
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;
	//Enable the Engine
	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG0_ACTIVE;
	//Configure BG0
	BGCTRL_SUB[0] = BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(0) | BG_TILE_BASE(1);

	//Copy the tiles to the tile base
	swiCopy(quarterTiles, BG_TILE_RAM_SUB(1), quarterTilesLen/2);

	//Copy the 4 16-colors palettes (4 times the same palette)
	swiCopy(quarterPal, &BG_PALETTE_SUB[0], quarterPalLen/2);
	swiCopy(quarterPal, &BG_PALETTE_SUB[16], quarterPalLen/2);
	swiCopy(quarterPal, &BG_PALETTE_SUB[32], quarterPalLen/2);
	swiCopy(quarterPal, &BG_PALETTE_SUB[48], quarterPalLen/2);

	//Fill all the BG0 with the white tile (tile corresponding to the 12th position
	//of the map, the upper right corner of the original image)
	int i;
	for(i=0; i<32*24; i++)
		BG_MAP_RAM_SUB(0)[i] = quarterMap[11];

	//Create the map of the BG0
	int row, column, tile = 0;
	for(row = 0; row < 12; row++)
		for(column = 0; column <12; column++)
		{
			//Upper left quarter
			BG_MAP_RAM_SUB(0)[4 + row*32 + 11 - column]
			                  = quarterMap[tile] | (1<<10) | PALETTE(0);
			//Upper right quarter
			BG_MAP_RAM_SUB(0)[4 + row*32 + 12 + column]
			                  = quarterMap[tile] | PALETTE(1);
			//Bottom left quarter
			BG_MAP_RAM_SUB(0)[4 + (23-row)*32 + 11 - column]
			                  = quarterMap[tile] | (1<<10) | (1<<11) | PALETTE(2);
			//Bottom right quarter
			BG_MAP_RAM_SUB(0)[4 + (23-row)*32 + 12 + column]
			                  = quarterMap[tile] | (1<<11) | PALETTE(3);
			tile++;
		}

	//Change the default colors of the quarters
	BG_PALETTE_SUB[1] = ARGB16(1,0,20,0);	//Upper left (green)
	BG_PALETTE_SUB[17] = ARGB16(1,20,0,0);	//Upper right (red)
	BG_PALETTE_SUB[33] = ARGB16(1,20,20,0);	//Bottom left (yellow)
	BG_PALETTE_SUB[49] = ARGB16(1,0,0,20);	//Bottom rigth (Blue)

	//Enable Interrupt for the timers and Vblank and set the handlers
	irqSet(IRQ_TIMER0, &TIMER0_ISR);
	irqEnable(IRQ_TIMER0);
	irqSet(IRQ_TIMER2, &TIMER2_ISR);
	irqEnable(IRQ_TIMER2);
	irqSet(IRQ_VBLANK, &VBLANK_ISR);
	irqEnable(IRQ_VBLANK);

	//Configure the timers
	TIMER0_DATA = TIMER_FREQ_1024(8);
	TIMER0_CR = TIMER_DIV_1024 | TIMER_IRQ_REQ | TIMER_ENABLE;
	TIMER2_DATA = TIMER_FREQ_256(1000);
	TIMER2_CR = TIMER_DIV_256 | TIMER_IRQ_REQ;

	//Initialize the chrono display
	initChronoDisp_Main();
	changeColorDisp_Main(ARGB16(1,31,31,0));
	updateChronoDisp_Main(0,0,0);

	int x, y, radius;
	int hits = -1;
	int last;
	active_button = last = NONE;

	while(1) {

		//Scan the keys
		scanKeys();

		//If start is pressed, the game starts
		if(keysDown() & KEY_START)
		{
			/*MUSIC STARTS*/
			mmStart(MOD_MUSIC,MM_PLAY_LOOP);
			//Init the randomizer
			srand(TIMER0_DATA);
			//Random button starts
			active_button = rand() % 4;
			//Enable Timers
			TIMER0_CR |= TIMER_ENABLE;
			TIMER2_CR |= TIMER_ENABLE;
			//10 times will be hitted
			hits = 9;
			//Initialize the time variables for the chronometer
			min = sec = msec = 0;
		}

		//Declare a touchPosition variable
		touchPosition touch;
		//Poll the touch-screen
		touchRead(&touch);
		//If the touch received is different to (0,0)
		if(touch.px || touch.py)
		{
			//Calculates x and y with respect to the center of the game
			x = touch.px-127;
			y = touch.py-95;
			//Calculate the radius (distance to the center)
			radius = sqrt(x*x + y*y);

			int temp_butt = active_button;
			//Check if any of the buttons has been touched
			if(x>=3 && y <=-3 && radius >=19 && radius <96)	//RED touched
				if(active_button == RED) active_button = NONE;
			if(x>=3 && y >=3 && radius >=19 && radius <96) //BLUE touched
				if(active_button == BLUE) active_button = NONE;
			if(x<=-3 && y <=-3 && radius >=19 && radius <96) //GREEN touched
				if(active_button == GREEN) active_button = NONE;
			if(x<=-3 && y >=3 && radius >=19 && radius <96) //YELLO touched
				if(active_button ==YELLOW) active_button = NONE;

			/*EFFECT TOUCHED*/
			if(temp_butt != active_button) mmEffect(SFX_LASER);
		}


		if(active_button == NONE)
		{
			//End of the game (Stop Time Timer)
			if(hits == 0)
			{
				hits--;
				TIMER2_CR &= ~TIMER_ENABLE;
				last = active_button;
				/*MUSIC STOPS*/
				mmStop();
				/*EFFECT RESULT*/
				mmEffect(SFX_RESULT);
			}
			else if(hits > 0)
			//Look for another random button different to the last one
			{
				int temp = rand() % 4;
				if(temp != last)
				{
					hits--;
					last = active_button = temp;
				}
			}
			//Reset the colors to the dark ones
			BG_PALETTE_SUB[1] = ARGB16(1,0,20,0);
			BG_PALETTE_SUB[17] = ARGB16(1,20,0,0);
			BG_PALETTE_SUB[33] = ARGB16(1,20,20,0);
			BG_PALETTE_SUB[49] = ARGB16(1,0,0,20);
		}
		swiWaitForVBlank();
	}
}
