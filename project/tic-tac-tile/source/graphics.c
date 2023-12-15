#include <nds.h>
#include <stddef.h>
#include <string.h>

#include "a-palette.h"
#include "b-background.h"
#include "c-cross.h"
#include "d-circle.h"
#include "e-select.h"
#include "f-sub-background.h"
#include "g-sub-palette.h"
#include "nds/arm9/background.h"
#include "nds/arm9/video.h"
#include "nds/ndstypes.h"

#define BG_PALETTE_INCR 1
#define CROSS_PALETTE_INCR 3
#define CIRCLE_PALETTE_INCR 4
#define SELECT_PALETTE_INCR 5

// === Utilities ===

void set_bg_transform(size_t bg_nb) {
    bgTransform[bg_nb]->hdx = BIT(8);
    bgTransform[bg_nb]->vdx = 0 * 256;
    bgTransform[bg_nb]->hdy = 0 * 256;
    bgTransform[bg_nb]->vdy = BIT(8);
    bgTransform[bg_nb]->dx = 0 * 256;
    bgTransform[bg_nb]->dy = 0 * 256;
}

// === Images Palette Corrections ===

void correct_palette(void * data, size_t len, int increment) {
    unsigned char * r = data;
    for (size_t i = 0; i < len; i++) {
        r[i] += (r[i] == 0) ? 0 : increment;
    }
}

void images_palette_correction() {
    correct_palette((void *) b_backgroundBitmap, b_backgroundBitmapLen, BG_PALETTE_INCR);
    correct_palette((void *) c_crossBitmap, c_crossBitmapLen, CROSS_PALETTE_INCR);
    correct_palette((void *) d_circleBitmap, d_circleBitmapLen, CIRCLE_PALETTE_INCR);
    correct_palette((void *) e_selectBitmap, e_selectBitmapLen, SELECT_PALETTE_INCR);
}

// === Setup ===

void graphics_setup() {
    // Graphics power
    REG_POWERCNT |= POWER_LCD | POWER_2D_A | POWER_2D_B;

    // Graphics setup control registers
    VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;
    REG_DISPCNT = MODE_5_2D | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE;
    VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;
    REG_DISPCNT_SUB = MODE_5_2D | DISPLAY_BG0_ACTIVE;

    BGCTRL[2] = BgSize_B8_256x256 | BG_BMP_BASE(0);
    BGCTRL[3] = BgSize_B8_256x256 | BG_BMP_BASE(3);
    BGCTRL_SUB[0] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);

    set_bg_transform(2);
    set_bg_transform(3);

    // Reset all backgrounds
    memset(BG_BMP_RAM(0), 0, SCREEN_WIDTH * SCREEN_HEIGHT);
    memset(BG_BMP_RAM(3), 0, SCREEN_WIDTH * SCREEN_HEIGHT);

    // Copy palettes
    swiCopy(a_paletteBitmap, BG_PALETTE + 1, 128);
    swiCopy(g_sub_paletteBitmap, BG_PALETTE_SUB + 1, 128);

    // Copy tiles
    swiCopy(f_sub_backgroundTiles, BG_TILE_RAM_SUB(1), f_sub_backgroundTilesLen / 2);

    images_palette_correction();
}

// === Graphics Drawing ===

void set_backgrounds() {
    swiCopy(b_backgroundBitmap, BG_BMP_RAM(3), b_backgroundBitmapLen / 2);
    swiCopy(f_sub_backgroundMap, BG_MAP_RAM_SUB(0), f_sub_backgroundMapLen / 2);
}

void overlay_sprite(const u16 * sprite, size_t row_pos, size_t col_pos, size_t side) {
    for (size_t col = 0; col < side / 2; col++) {
        for (size_t row = 0; row < side; row++) {
            size_t pos = (row_pos + row) * SCREEN_WIDTH / 2 + col_pos / 2 + col;
            BG_BMP_RAM(0)[pos] = sprite[row * side / 2 + col];
        }
    }
}

void main_graphics() {
    overlay_sprite((void *) e_selectBitmap, 72, 104, 48);
    // overlay_sprite((void *) e_selectBitmap, 122, 54, 48);

    overlay_sprite((void *) c_crossBitmap, 76, 108, 40);
    overlay_sprite((void *) d_circleBitmap, 126, 58, 40);
}
