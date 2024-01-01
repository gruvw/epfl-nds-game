#include <nds.h>
#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"
#include <stdio.h>
#include <string.h>
#include <sys/dir.h>
#include <fat.h>
#include <dirent.h>

//Declare the tiles to use
unsigned char white[] ={
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00
};

unsigned char semiwhite_left[] ={
	0x01,0x00,0x00,0x00,
	0x01,0x00,0x00,0x00,
	0x01,0x00,0x00,0x00,
	0x01,0x00,0x00,0x00,
	0x01,0x00,0x00,0x00,
	0x01,0x00,0x00,0x00,
	0x01,0x00,0x00,0x00,
	0x01,0x00,0x00,0x00
};

unsigned char semiwhite_up[] ={
	0x11,0x11,0x11,0x11,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00
};

unsigned char black[] ={
	0x11,0x11,0x11,0x11,
	0x11,0x11,0x11,0x11,
	0x11,0x11,0x11,0x11,
	0x11,0x11,0x11,0x11,
	0x11,0x11,0x11,0x11,
	0x11,0x11,0x11,0x11,
	0x11,0x11,0x11,0x11,
	0x11,0x11,0x11,0x11
};

mm_sound_effect sound;
FILE* input = NULL;
DIR* di;
struct dirent* entry;
struct stat st;

void playNote()
{
	char note[16];
	int duration;

	//Disable Timer
	TIMER0_CR &= ~(TIMER_ENABLE);

	//Read if there is an open file
	if(input)
	{
		//Read note and duration
		if(fscanf(input,"%s %i", note, &duration) != EOF)
		{
			//Look for the playback rate (note)
			sound.rate = 0;
			if(strcmp(note,"DO#")==0) sound.rate = (int)(1.059f * 1024); //DO#
			else if(strcmp(note,"RE#")==0) sound.rate = (int)(1.189f * 1024); //RE#
			else if(strcmp(note,"FA#")==0) sound.rate = (int)(1.414f * 1024); //FA#
			else if(strcmp(note,"SOL#")==0)	sound.rate = (int)(1.587f * 1024); //SOL#
			else if(strcmp(note,"LA#")==0) sound.rate = (int)(1.782f * 1024); //LA#
			else if(strcmp(note,"DO2#")==0)	sound.rate = (int)(2.119f * 1024); //DO2#
			else if(strcmp(note,"DO")==0) sound.rate = (int)(1.0f * 1024); //DO
			else if(strcmp(note,"RE")==0) sound.rate = (int)(1.122f * 1024); //RE
			else if(strcmp(note,"MI")==0) sound.rate = (int)(1.26f * 1024); //MI
			else if(strcmp(note,"FA")==0) sound.rate = (int)(1.335f * 1024); //FA
			else if(strcmp(note,"SOL")==0) sound.rate = (int)(1.498f * 1024); //SOL
			else if(strcmp(note,"LA")==0) sound.rate = (int)(1.682f * 1024); //LA
			else if(strcmp(note,"SI")==0) sound.rate = (int)(1.888f * 1024); //SI
			else if(strcmp(note,"DO2")==0) sound.rate = (int)(2.f * 1024); //DO2

			//Play the note
			if(sound.rate) mmEffectEx(&sound);

			//If duration>0 set the next timer event
			if(duration)
				TIMER0_DATA = TIMER_FREQ_1024(duration);
			//Enable timer again
			TIMER0_CR |= TIMER_ENABLE;
		}
	}
}


int main(void) {

	//Enable the VRAM bank
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;
	//Enable the Engine
	REG_DISPCNT = MODE_0_2D | DISPLAY_BG0_ACTIVE;

	//Configure BG0
	BGCTRL[0] = BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(0) | BG_TILE_BASE(1);

	//Swap the LCDs
	lcdSwap();

	//Initialize the console in the upper screen (sub engine)
	consoleDemoInit();

	//Black and white colors in the palette
	BG_PALETTE[0] = ARGB16(1,31,31,31);
	BG_PALETTE[1] = ARGB16(1,0,0,0);

	//Copy tiles to the tile Base
	dmaCopy(white,&BG_TILE_RAM(1)[0], 32);
	dmaCopy(semiwhite_left,&BG_TILE_RAM(1)[16], 32);
	dmaCopy(semiwhite_up,&BG_TILE_RAM(1)[32], 32);
	dmaCopy(black,&BG_TILE_RAM(1)[48], 32);

	/********************
	 * Generate the map
	 ********************/
	int row, key, i;

	//Create white keys
	for(row = 0; row<18; row++)
		for(key = 0; key < 8; key++)
		{
			BG_MAP_RAM(0)[row*32+key*4] = 1;
			BG_MAP_RAM(0)[row*32+key*4+1] = 0;
			BG_MAP_RAM(0)[row*32+key*4+2] = 0;
			BG_MAP_RAM(0)[row*32+key*4+3] = 1 | BIT(10);
		}
	for(i = 0; i<32; i++)
		BG_MAP_RAM(0)[row*32+i] = 2;

	//Create the black keys
	for(row = 0; row < 10; row++)
		for(key = 0; key < 8; key ++)
			if(key != 2 && key != 6)
			{
				BG_MAP_RAM(0)[3 + row*32+key*4] = 3;
				if(key != 7)
					BG_MAP_RAM(0)[3 + row*32+key*4+1] = 3;
			}
	/******End map generation***********/

	//Init the sound library
	mmInitDefaultMem((mm_addr)soundbank_bin);

	//Init Fat Library and open the directory "/melodies"
	fatInitDefault();
	di = opendir("/melodies/");
	printf("Press RIGHT to start listing the music folder");

	//TIMER and interrupts configuration
	TIMER0_CR = TIMER_DIV_1024 | TIMER_IRQ_REQ;
	irqSet(IRQ_TIMER0, &playNote);
	irqEnable(IRQ_TIMER0);

	//Load the effects
	mmLoadEffect(SFX_DO);

	//Declaration of the sound effect to play and set up of the structure
	sound.id = SFX_DO;
	sound.rate    =	(int)(1.0f * (1<<10));	// rate
	sound.handle  = 0;           // 0 = allocate new handle
	sound.volume  = 255;         //Max volumen
	sound.panning = 128;         // centered panning


	//Touch position declaration
	touchPosition touch;

	//Scan keys once to clean registers before main loop
	scanKeys();

	while(1) {
		//Scan the keys
		scanKeys();
		u32 keys = keysDown();
		//If the touchscreen is pressed
		if(keys & KEY_TOUCH)
		{
			//Read the touch-screen
			touchRead(&touch);

			//Sound rate set to 0
			sound.rate = 0;

			//Chek if the Piano keyboard is touched
			if(touch.py < 18*8)
			{
				//Solution 2
				//Identify the key and set the corresponding soun rate
				if(touch.px >= 24 && touch.px < 40 && touch.py < 80)
					sound.rate = (int)(1.059f * 1024); //DO#
				else if(touch.px >= 56 && touch.px < 72 && touch.py < 80)
					sound.rate = (int)(1.189f * 1024); //RE#
				else if(touch.px >= 120 && touch.px < 136 && touch.py < 80)
					sound.rate = (int)(1.414f * 1024); //FA#
				else if(touch.px >= 152 && touch.px < 168 && touch.py < 80)
					sound.rate = (int)(1.587f * 1024); //SOL#
				else if(touch.px >= 184 && touch.px < 200 && touch.py < 80)
					sound.rate = (int)(1.782f * 1024); //LA#
				else if(touch.px >= 248 && touch.px < 256 && touch.py < 80)
					sound.rate = (int)(2.119f * 1024); //DO2#
				else if(touch.px >= 0 && touch.px < 32)
					sound.rate = (int)(1.0f * 1024); //DO
				else if(touch.px >= 32 && touch.px < 64)
					sound.rate = (int)(1.122f * 1024); //RE
				else if(touch.px >= 64 && touch.px < 96)
					sound.rate = (int)(1.26f * 1024); //MI
				else if(touch.px >= 96 && touch.px < 128)
					sound.rate = (int)(1.335f * 1024); //FA
				else if(touch.px >= 128 && touch.px < 160)
					sound.rate = (int)(1.498f * 1024); //SOL
				else if(touch.px >= 160 && touch.px < 192)
					sound.rate = (int)(1.682f * 1024); //LA
				else if(touch.px >= 192 && touch.px < 224)
					sound.rate = (int)(1.888f * 1024); //SI
				else if(touch.px >= 224 && touch.px < 256)
					sound.rate = (int)(2.f * 1024); //DO2

				//Play the sound if identified
				if(sound.rate >= 0)
					mmEffectEx(&sound);
			}
		}

		if(keys & KEY_A)
		{
			//Start the playback
			playNote();
		}

		if(keys & KEY_RIGHT)
		{
			//Disable timer
			TIMER0_CR &= ~(TIMER_ENABLE);

			//Close file (if opened)
			if(input)
				fclose(input);

			//check if there is next entry, if not reopen the directory
			if((entry = readdir(di))== NULL)
			{
				//Close and reopen the directory
				closedir(di);
				di = opendir("/melodies/");
				entry = readdir(di);
			}

			//Retrieve the next entry (if any) different to "." or ".."
			while(entry != NULL && !(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")))
				entry = readdir(di);

			//If valid entry (not empty folder) open file and print title
			if(entry != NULL)
			{
				char str[256];
				strcpy(str,"");
				strcat(str, "/melodies/");
				strcat(str,entry->d_name);
				input = fopen(str, "r");
				consoleClear();
				printf("SONG : %s\n", entry->d_name);
			}
			else //Empty folder
				printf("Empty folder \n");
		}

		//Wait for VBlank
		swiWaitForVBlank();
	}
}
