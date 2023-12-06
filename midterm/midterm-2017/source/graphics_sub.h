/////////////////////////////////////////////////////////
////	DO NOT MODIFY THIS FILE
/////////////////////////////////////////////////////////

#ifndef GRAPHICS_SUB_H_
#define GRAPHICS_SUB_H_

#include <nds.h>
#include <string.h>


// Macros for the colors
#define	RED ARGB16(1,31,0,0)
#define GREEN ARGB16(1,0,31,0)
#define	BLUE ARGB16(1,0,0,31)
#define	YELLOW ARGB16(1,31,31,0)
#define	LIGHT_BLUE ARGB16(1,0,31,31)
#define	WHITE ARGB16(1,31,31,31)
#define	BLACK ARGB16(1,0,0,0)


/*
 * Print timer's plate, dial and buttons in extended rotoscale mode
 * with palette of 256 colors
 */
void configureGraphics_Sub_Plate();


#endif /* GRAPHICS_SUB_H_ */
