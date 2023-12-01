
#include <nds.h>
#include <stdio.h>
#include "demo.h"

/*** You may want to use follow variables in your solutions ***/
// Color definitions
#define	RED   ARGB16(1,31,0,0)
#define	BLUE  ARGB16(1,0,0,31)
#define	GREEN ARGB16(1,0,31,0)
#define	WHITE ARGB16(1,31,31,31)
#define	BLACK ARGB16(1,0,0,0)

// 5 custom color tiles
u8 RedTile[64] =
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

u8 BlueTile[64] =
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

u8 GreenTile[64] =
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

u8 WhiteTile[64] =
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

u8 BlackTile[64] =
{
	4,4,4,4,4,4,4,4,
	4,4,4,4,4,4,4,4,
	4,4,4,4,4,4,4,4,
	4,4,4,4,4,4,4,4,
	4,4,4,4,4,4,4,4,
	4,4,4,4,4,4,4,4,
	4,4,4,4,4,4,4,4,
	4,4,4,4,4,4,4,4
};

// rainbow_snake struct
typedef struct rainbow_snake{
	int tail_x;
	int tail_y;
	int tail_color;
	int body_x;
	int body_y;
	int body_color;
	int head_x;
	int head_y;
	int head_color;
} rainbow_snake;

// declare a snake struct
rainbow_snake snake;

// Direction definition
enum DIRECTION {
  LEFT,
  RIGHT,
  UP,
  DOWN
};
// initialize direction as RIGHT
enum DIRECTION direction = RIGHT;

/*** You may want to use the above variables in your solutions ***/

void timer_ISR()
{
	/*************
	 * EXERCISE 3 (part B)
	 *************/
	// 1) update the position of different parts of the snake, leave a white tile behind the tail


	// 2) generate a new map for the snake

}


int main(void) {

	/*************
	 * EXERCISE 1
	 *************/

	// 1) VRAM Configuration for MAIN engine

	// 2) Main engine configuration in rotoscale mode

	// 3) Configure the background (Ignore warning 'large implictly truncated to unsigned type')

	// 4) Copy bitmap and palette generated by grit

	// !!! Uncomment this for including the Affine Matrix Transformation
	/*
	bgTransform[2]->xdx = 1*256;
	bgTransform[2]->ydx = 0*256;
	bgTransform[2]->xdy = 0*256;
	bgTransform[2]->ydy = 1*256;
	bgTransform[2]->dx = 0*256;
	bgTransform[2]->dy = 0*256;
	*/


	// !!! Uncomment this for including the Affine Matrix Transformation (MAC and Native Linux user only!)
	/*
	bgTransform[2]->hdx = 1*256;
	bgTransform[2]->vdx = 0*256;
	bgTransform[2]->hdy = 0*256;
	bgTransform[2]->vdy = 1*256;
	bgTransform[2]->dx = 0*256;
	bgTransform[2]->dy = 0*256;
	*/

	/**************
	 * EXERCISE 2 *
	 **************/

	// 1) VRAM configuration for SUB engine

	// 2) SUB engine configuration in tiled mode

	// 3) Configure the background

	// 4) Copy the 5 tiles to the tile base

	// 5) Initialize the palette (5 components)

	// 6) Generate the map for black background



	// 7) set the initial position of the snake, as well as the color of each part
//	snake.tail_x = ...

	// 8) Generate the map for the snake



	/*************
	 * EXERCISE 3 (part A)
	 *************/
	///////////////////////////////////////////////////////////////////////////////////////
	//IMPORTANT NOTE!!!: Do not initialize the interrupts with the call to irqInit() since
	// later the touchscreen will be used
	///////////////////////////////////////////////////////////////////////////////////////

	// 1) Configure the timer to raise an interrupt 10 times per second
//	int frequency =

	// 2) Associate the implemented ISR to the Timer Interrupt Line

	// 3) Enable the timer interrupt

	// 4) Implement the functionality of the timer_ISR(), which is defined above main()

	scanKeys();
    while(1){

		/*************
		 * EXERCISE 4
		 *************/

    	// 1) Scan the keypad

    	// 2) Obtain the pressed keys

    	// 3) change the direction of the snake according to the pressed keys

		// 4) Complete the functionality about moving up and down of the snake in timer_ISR(), which is defined above main()

		/*************
		 * EXERCISE 5
		 *************/
    	// 1) If the touchscreen is touched, increase and set the new frequency. Note that the frequency should be no larger than 60


        swiWaitForVBlank();
    }
}