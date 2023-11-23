/////////////////////////////////////////////////////////
////	DO NOT MODIFY THIS FILE
/////////////////////////////////////////////////////////

#include <nds.h>
#include "string.h"

/*
 * This function plots the 32 tiles of the given digit (number) in
 * the coordinates x, y of the map (not the pixels). The parameters are:
 * 		map = pointer to the location where the Background map is placed
 * 		number = digit to be printed in the range [0..9]. If the number is
 * 			out of this range the funciton does not print any digit
 * 		x, y = Coordinates of the map. This display is implemented to be used
 * 			with a 32x32 map of pixels. X and Y represent the coordinates of
 * 			the 32x32 grid where to start printing the digit (upper left corner).
 * 			In other words, the 32 tiles of the digit should be plotted form
 * 			the positions (x,y) to the position (x+4, x+8).
 */
void printDigit(u16* map, int number, int x, int y);

/*
 * This function prints the value of the countdown timer. It does not receive the coor-
 * dinates where to plot the countdown timer display. It will always be plotted in the
 * center of the screen starting in the coordinates x=0, y=8 (see the example
 *  depicted in the manual). The parameters are
 *  	map = pointer to the location where the Background map is placed
 *  	min = number of minutes in the range [0...59]
 *  	sec = number of seconds in the range [0...59]
 *  	msec = number of milliseconds in the range [0...999]
 *
 *  If any of the parameter is out of the range the corresponding digits should
 *  not be printed.
 *
 *  The separator colon for the minutes and seconds and the separator pint for
 *  the seconds and the milliseconds should also be plotted correctly. Both sym-
 *  bols are made out of 2x8 tiles (16x64 pixels).
 *
 *  The function should also clear all the screen before. To do that, an empty
 *  tile (for example the tile 32) will be assigned first to all the 32x32 map
 */
void updateTimerDisp(u16* map, int min, int sec, int msec);

/*
 * This function change the colors used to display the countdown timer display.
 * The parameters are:
 * 		b = background color (that replaces to the default white)
 * 		c = digits color (that replaces to the default black)
 * The tiles of the countdown timer display are created out of two colors (black
 * and white) that are stored in two places in the palet. Changing this two
 * components of the palette will change the color of the display. Look at the
 * generated tiles to guess which components of the palette are used.
 */
void changeMainColorDisp(u16 b, u16 c);


