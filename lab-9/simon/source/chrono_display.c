/*
 * chrono_display.c
 *
 *  Created on: Mar 21, 2011
 *      Author: nds
 */

#include "chrono_display.h"

void printDigit(u16* map, int number, int x, int y)
{
	int i,j;
	if(number < 10)
		for(i = 0; i<8;i++)
			for(j = 0; j<4; j++)
				if(number >= 0)
					map[(i + y)*32+j+x] = (u16)(i*4+j)+32*number;
				else
					map[(i + y)*32+j+x] = 32;
	if(number == 10)
		for(i = 0; i<8;i++)
			for(j = 0; j<2; j++)
				map[(i + y)*32+j+x] = (u16)(i*4+j)+32*10+2;
	if(number==11)
		for(i = 0; i<8;i++)
			for(j = 0; j<2; j++)
				map[(i + y)*32+j+x] = (u16)(i*4+j)+32*10;
}

void updateChronoDisp(u16* map,int min, int sec, int msec)
{
	int x = 0, y = 0;
	int number;

	/*****MINUTES******/
	number = min;
	if(min > 59) min = number = -1;
	//First digit
	x = 0; y = 8;
	if(min>=0) number = min/10;
	printDigit(map, number, x,y);
	//Second digit
	x = 4; y = 8;
	if(min>=0) number = min %10;
	printDigit(map, number, x,y);

	/*****POINTS******/
	x = 8; y = 8;
	number = 10;
	printDigit(map, number, x,y);

	/*****SECONDS******/
	number = sec;
	if(sec > 59) sec = number = -1;
	//First digit
	x = 10; y = 8;
	if(sec>=0) number = sec / 10;
	printDigit(map, number, x,y);
	//Second digit
	x = 14; y = 8;
	if(sec>=0) number = sec % 10;
	printDigit(map, number, x,y);

	/*****POINT MSEC******/
	x = 18; y = 8;
	number = 11;
	printDigit(map, number, x,y);

	/*****M.SECONDS******/
	number = msec;
	if(msec > 999) msec = number = -1;
	//First digit
	x = 20; y = 8;
	if(msec>=0) number = msec / 100;
	printDigit(map, number, x,y);

	//Second digit
	x = 24; y = 8;
	if(msec>=0) number = (msec % 100) / 10;
	printDigit(map, number, x,y);

	//Third digit
	x = 28; y = 8;
	if(msec>=0) number = (msec % 10) % 10;
	printDigit(map, number, x,y);
}

void updateChronoDisp_Main(int min, int sec, int msec)
{
	updateChronoDisp(BG_MAP_RAM(0), min,sec,msec);
}

void updateChronoDisp_Sub(int min, int sec, int msec)
{
	updateChronoDisp(BG_MAP_RAM_SUB(0), min,sec,msec);
}

void initChronoDisp_Main()
{
	VRAM_A_CR = VRAM_ENABLE
				| VRAM_A_MAIN_BG;
	REG_DISPCNT = MODE_0_2D | DISPLAY_BG0_ACTIVE;
	BGCTRL[0] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);

	swiCopy(numbersTiles, BG_TILE_RAM(1), numbersTilesLen);
	swiCopy(numbersPal, BG_PALETTE, numbersPalLen);
	BG_PALETTE[0] = ARGB16(1,0,31,31);
	BG_PALETTE[1] = ARGB16(1,0,0,0);
	memset(BG_MAP_RAM(0), 32,32*32*2);
	updateChronoDisp_Main(-1,-1,-1);
}

void initChronoDisp_Sub()
{
	VRAM_C_CR = VRAM_ENABLE
			| VRAM_C_SUB_BG;

	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG0_ACTIVE;
	BGCTRL_SUB[0] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);

	swiCopy(numbersTiles, BG_TILE_RAM_SUB(1), numbersTilesLen);
	swiCopy(numbersPal, BG_PALETTE_SUB, numbersPalLen);
	BG_PALETTE_SUB[0] = ARGB16(1,31,31,0);
	BG_PALETTE_SUB[1] = ARGB16(1,0,0,0);
	memset(BG_MAP_RAM_SUB(0), 32,32*32*2);
	updateChronoDisp_Sub(-1,-1,-1);
}

void changeColorDisp_Main(uint16 c)
{
	BG_PALETTE[0] = c;
}

void changeColorDisp_Sub(uint16 c)
{
	BG_PALETTE_SUB[0] = c;
}
