/*
 * exercises.h
 *
 *  Created on: Oct 20, 2017
 *      Author: nds
 */

#ifndef EXERCISES_H_
#define EXERCISES_H_

#include <nds.h>
#include <string.h>


// Function prototypes
void exercise_1();
void exercise_2();
void exercise_3();


// Picture/Grit includes
#include "hand.h"
#include "numbers.h"

#include "../source/graphics_sub.h"
#include "../source/irq_management.h"

extern uint32 min, sec, msec;


void exercise_1()
{
	// Configure Sub screen
	configureGraphics_Sub_Plate();

	//---------------------- Print timer's hand in extended rotoscale mode emulating framebuffer mode ----------------------

	//Configure the engine in Mode 5 (already done for the plate image) and use the BG2
	REG_DISPCNT_SUB |= DISPLAY_BG2_ACTIVE; // WARNING: Here we must use the "|="
	//Configure background BG2 in extended rotoscale mode emulating framebuffer mode
	BGCTRL_SUB[2] = BG_BMP16_128x128 | BG_BMP_BASE(4);

	// Transfer image to the corresponding memory location
	swiCopy(handBitmap, BG_BMP_RAM_SUB(4), handBitmapLen/2); // Copy bitmap
}


void exercise_2()
{
	// Configure Main screen
	//---------------------- Print timer digits in tiled mode ----------------------

	// Enable a suitable VRAM block and map it to the main engine
	VRAM_A_CR = VRAM_ENABLE
				| VRAM_A_MAIN_BG;
	// Configure the engine in Mode 0 and use the BG1
	REG_DISPCNT = MODE_0_2D | DISPLAY_BG1_ACTIVE;
	// Configure the engine to be used as a 32x32 grid of tiles of 256 colors
	// Use BG_MAP_BASE(0) and  a suitable BG_TILE_BASE
	BGCTRL[1] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);

	// Copy the tiles and the palette to the corresponding location
	swiCopy(numbersTiles, BG_TILE_RAM(1), numbersTilesLen/2);
	swiCopy(numbersPal, BG_PALETTE, numbersPalLen);

	// DO NOT CALL irqInit();

	// Flush the keys register the first time
	scanKeys();

	// By default, the background color is set to WHITE
	// and the digits color is set to BLACK
	changeMainColorDisp(WHITE, BLACK);

	// Configure the timer and associate the ISRs
	IRQ_initialize();

	// Initialization and start the timer for test (Only)
	min = 60;
	irqEnable(IRQ_TIMER0);
}


void exercise_3()
{
	// Reset the timer initialization used for test
	min = 0;
	irqDisable(IRQ_TIMER0);
}

#endif /* EXERCISES_H_ */
