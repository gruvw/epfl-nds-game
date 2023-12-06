#include "graphics_sub.h"
#include "nds/arm9/background.h"
#include "nds/arm9/video.h"
#include "nds/bios.h"
#include "plate.h"
//... TO COMPLETE EXERCISE 1

// ------ Exercise 1 ------ //

/*
 * Print timer's plate, dial and buttons in extended rotoscale mode
 * with palette of 256 colors
 */
void configureGraphics_Sub_Plate() {
    // Configure the VRAM bank C accordingly
    VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;

    // Configure the engine in Mode 5 and use the BG3
    REG_DISPCNT_SUB = MODE_5_2D | DISPLAY_BG3_ACTIVE;


    // Configure background BG3 in extended rotoscale mode using 8 bit pixels and "0" for the base address
    BGCTRL_SUB[3] = BG_BMP_BASE(0) | BgSize_B8_256x256;

    // Transfer image bitmap and palette to the corresponding memory locations
    swiCopy(plateBitmap, BG_BMP_RAM_SUB(0), plateBitmapLen / 2);
    swiCopy(platePal, BG_PALETTE_SUB, platePalLen / 2);

    // Set up affine matrix
    REG_BG3PA_SUB = 256;
    REG_BG3PC_SUB = 0;
    REG_BG3PB_SUB = 0;
    REG_BG3PD_SUB = 256;
}
