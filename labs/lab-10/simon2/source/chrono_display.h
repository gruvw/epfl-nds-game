/*
 * chrono_display.h
 *
 *  Created on:
 *      Author: nds
 */

#include <nds.h>
#include "numbers.h"
#include "string.h"

/*
 * Initializers of the display. This function should be called
 * whenever the display needs to be initialized to be used in the
 * upper screen (Main) or the bottom screen (Sub)
 *
 * Warning! Do not use the display in the bottom screen in combination
 * with the console. It will not work properly
 */
void initChronoDisp_Main();
void initChronoDisp_Sub();

/*
 * Functions to update the value of the display showed in the upper screen
 * (Main) or the one shown in the bottom screen (Sub). These functions receive
 * three integer as input representing minutes, seconds and milliseconds
 * respectively.
 *
 * Warning: Do not call any of these functions if the previous initialization
 * has not been performed.
 */
void updateChronoDisp_Main(int min, int sec, int msec);
void updateChronoDisp_Sub(int min, int sec, int msec);

/*
 * Functions to change the color of the upper display (Main) or the bottom
 * display (Sub). These functions receive a 16 bits integer as input that
 * represent a valid color. Some valid colors are declared at the beginning
 * of this file. New colors can be created with the macro ARGB16(..).
 *
 * Warning: Do not use these functions if the coresponding display has not
 * initialized
 */
void changeColorDisp_Main(uint16 c);
void changeColorDisp_Sub(uint16 c);
