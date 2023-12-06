/*---------------------------------------------------------------------------------

	Basic template code for starting a DS app

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include <string.h>

#include "helicopter.h"

// Choose which solution to use
// SOLUTION_COPY_MEM: Use only VRAM_A - Copy part of the image to BG_GFX in each timer interrupt
// SOLUTION_MOVE_MAP_BASE: Use both VRAM_A, VRAM_B - Copy whole image once and move the MAP_BASE in each timer interrupt
// NOTE: Observe that the results of part2 (scaling image up/down) are not exactly the same for both solutions! Why this happens?
#define SOLUTION_COPY_MEM  // comment this out to enable Solution 2
#ifndef SOLUTION_COPY_MEM
    #define SOLUTION_MOVE_MAP_BASE
#endif

//This variable tells us which image to render (0,1 or 2)
int image = 0;

//Timer0 ISR for changing the image every 100 ms
void Timer0ISR() {
    //Change image and copy it to the engin memory
    image = (image + 1) % 3;
#ifdef SOLUTION_COPY_MEM
    int offset = image * 256 * 192 / 4;
    memcpy(BG_GFX, &helicopterBitmap[offset], 256 * 192);
#elif defined(SOLUTION_MOVE_MAP_BASE)
    // Change background base to start the map from different location
    BGCTRL[2] = BG_MAP_BASE(image * 3) | BgSize_B8_256x256;
#endif
}

//Our increasing / decreasing factor and a flag
int increasing = 0;
float factor = 1.0;

//Timer1 ISR to change the scale of the affine transform matrix
void Timer1ISR() {
    //Increase/decrease the factor by 0.1
    if (increasing)
        factor = factor + 0.1;
    else
        factor = factor - 0.1;

    //Check and update the flag if necessary
    if (factor <= 0.2)
        increasing = 1;
    if (factor >= 1)
        increasing = 0;

    //Change the Affine Transformation Matrix
    bgTransform[2]->hdx = 256 / factor;
    bgTransform[2]->vdy = 256 / factor;
    bgTransform[2]->hdy = 0;
    bgTransform[2]->vdx = 0;
}

//---------------------------------------------------------------------------------
int main(void) {
    //---------------------------------------------------------------------------------

    //Engine and background configuration
    REG_DISPCNT = MODE_5_2D | DISPLAY_BG2_ACTIVE;
    VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;
#ifdef SOLUTION_MOVE_MAP_BASE
    VRAM_B_CR = VRAM_ENABLE | VRAM_B_MAIN_BG;
#endif

    BGCTRL[2] = BG_MAP_BASE(0) | BgSize_B8_256x256;

    //Transfer of the image and the palette to the engine
#ifdef SOLUTION_COPY_MEM
    memcpy(BG_GFX, helicopterBitmap, 256 * 192);
#elif defined(SOLUTION_MOVE_MAP_BASE)
    memcpy(BG_GFX, helicopterBitmap, helicopterBitmapLen);
#endif
    memcpy(BG_PALETTE, helicopterPal, helicopterPalLen);

    //Set the matrix affine transform
    bgTransform[2]->hdx = 256;
    bgTransform[2]->vdy = 256;
    bgTransform[2]->hdy = 0;
    bgTransform[2]->vdx = 0;

    //Interrupt initialization, enabling and handlers setup
    irqInit();
    irqEnable(IRQ_VBLANK);
    irqSet(IRQ_TIMER0, &Timer0ISR);
    TIMER_DATA(0) = TIMER_FREQ_1024(10);
    TIMER0_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
    irqEnable(IRQ_TIMER0);

    irqSet(IRQ_TIMER1, &Timer1ISR);
    TIMER_DATA(1) = TIMER_FREQ_1024(20);
    TIMER1_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
    irqEnable(IRQ_TIMER1);

    while (1) {
        swiWaitForVBlank();
    }
}
