/////////////////////////////////////////////////////////
////	DO NOT MODIFY THIS FILE
/////////////////////////////////////////////////////////
#ifndef GRAPHICS_SUB_H_
#define GRAPHICS_SUB_H_
#include <nds.h>

/*
 * configureGraphics_Sub:
 *  configures the SUB engine in MODE 5 and activates BG2. It also sets-up the
 *  corresponding VRAM bank
 */
void configureGraphics_Sub();

/*
 * configBG2_Sub:
 * 	configures the background BG2 in rotoscale mode and transfers the
 * 	corresponding image (bottom.png) to the background using a pixel depth
 * 	of 8 bits
 */
void configBG2_Sub();

#endif /* GRAPHICS_SUB_H_ */
