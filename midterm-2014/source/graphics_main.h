/////////////////////////////////////////////////////////
////	DO NOT MODIFY THIS FILE
/////////////////////////////////////////////////////////
#ifndef GRAPHICS_MAIN_H_
#define GRAPHICS_MAIN_H_

#include <nds.h>

/*
 * configureGraphics_Main:
 * This function sets up the main graphical engine to work in mode 5 and
 * activates the background 2. It also sets up the VRAM bank A properly
 */
void configureGraphics_Main();


/*
 * configBG2_Main:
 * This function configures the BG2 to work in rotoscale mode emulating
 * framebuffer mode (i.e. using 16bit pixels)
 */
void configBG2_Main();


/*
 * fillRectangle:
 * This function fills a rectangle in the background 2 with the provided color.
 * The rectangle is delimited by the coordinates (top,left) and (bottom,right).
 * The function does nothing if one of the coordinate is out of the boundaries.
 * IMPORTANT: The screen buffer where to paint the pixels depends on the
 * configuration of the background 2 done in the function configBG2()
 */
void fillRectangle(int left, int right, int top, int bottom, u16 color);


/* configBG0_Main:
 * This function configure BG0 to work in tiled mode. It also copies the
 * custom tile(s) to the corresponding tile base, sets the necessary color(s)
 * in the palette and generates the map as specified in the exam sheet.
 */
void configBG0_Main();


#endif /* GRAPHICS_MAIN_H_ */
