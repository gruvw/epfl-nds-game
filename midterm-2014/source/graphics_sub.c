#include "graphics_sub.h"
#include "bottom.h"
#include "game.h"
#include "nds/bios.h"

void configureGraphics_Sub() {
	// Configure the MAIN engine in mode 5 and activate background 2
    REG_DISPCNT_SUB = MODE_5_2D | DISPLAY_BG2_ACTIVE;

	// Configure the VRAM bank C accordingly
    VRAM_A_CR = VRAM_ENABLE | VRAM_C_SUB_BG;
}


void configBG2_Sub() {
	// Configure background BG2 in extended rotoscale mode using 8bit pixels
    BGCTRL_SUB[2] = BG_MAP_BASE(0) | BgSize_B8_256x256;

	// Transfer image and palette to the corresponding memory locations
    swiCopy(bottomBitmap, BG_BMP_RAM_SUB(0), bottomBitmapLen/2);
    swiCopy(bottomPal, BG_PALETTE_SUB, bottomPalLen/2);

    // Set up affine matrix
    REG_BG2PA_SUB = 256;
    REG_BG2PC_SUB = 0;
    REG_BG2PB_SUB = 0;
    REG_BG2PD_SUB = 256;
}
