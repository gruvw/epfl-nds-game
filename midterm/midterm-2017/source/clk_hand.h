/////////////////////////////////////////////////////////
////	DO NOT MODIFY THIS FILE
/////////////////////////////////////////////////////////

#ifndef CLK_HAND_DISPLAY_H_
#define CLK_HAND_DISPLAY_H_

#include <nds.h>
#include <math.h>

// Constants
#define CLK_HAND_WIDTH_PIX        128 // Pixel width of the image representing the red hand of the clock
#define CLK_HAND_MARGIN_X_PIX     ((SCREEN_HEIGHT - CLK_HAND_WIDTH_PIX)/2) // Pixel margin on the left of the image representing the red hand of the clock
#define CLK_HAND_MARGIN_Y_PIX     ((SCREEN_HEIGHT - CLK_HAND_WIDTH_PIX)/2) // Pixel margin on the top and bottom of the image representing the red hand of the clock

// Function prototypes
void rotateImage_Sub_BG2(int x, int y, float angle_rads);
void updateClkHandPosition(uint32 sec_init, uint32 sec_curr);



#endif /* CLK_HAND_DISPLAY_H_ */
