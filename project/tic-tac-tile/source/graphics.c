#include <nds.h>
#include <stddef.h>
#include <string.h>

#include "nds/arm9/background.h"
#include "nds/arm9/video.h"
#include "nds/ndstypes.h"

#include "a-palette.h"
#include "b-background.h"
#include "c-cross.h"

#define CROSS_PALETTE_INCR 1

// === Utilities ===

void set_bg_transform(size_t bg_nb) {
    bgTransform[bg_nb]->vdy = bgTransform[bg_nb]->hdx = BIT(8);
    bgTransform[bg_nb]->dy = bgTransform[bg_nb]->dx = bgTransform[bg_nb]->hdy = bgTransform[bg_nb]->vdx = 0;
}

// === Images Palette Corrections ===

void correct_palette(void * data, size_t len, int increment) {
    unsigned char * r = data;
    for (size_t i = 0; i < len; i++) {
        r[i] += (r[i] == 0) ? 0 : increment;
    }
}

void images_palette_correction() {
    correct_palette((void *) c_crossBitmap, c_crossBitmapLen, CROSS_PALETTE_INCR);
}

// === Setup ===

void graphics_setup() {
    VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;

    REG_DISPCNT = MODE_5_2D | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE;

    BGCTRL[2] = BG_BMP_BASE(0) | BgSize_B8_256x256;
    BGCTRL[3] = BG_BMP_BASE(3) | BgSize_B8_256x256;

    swiCopy(a_palettePal, BG_PALETTE, a_palettePalLen / 2);

    images_palette_correction();

    set_bg_transform(2);
    set_bg_transform(3);
}

// === Graphics Drawing ===

void set_background() {
    swiCopy(b_backgroundBitmap, BG_BMP_RAM(3), b_backgroundBitmapLen / 2);
}

void set_cross(size_t row_pos, size_t col_pos) {
    images_palette_correction();

    const u16 * s = (u16 *) c_crossBitmap;
    for (size_t col = 0; col < 40 / 2; col++) {
        for (size_t row = 0; row < 40; row++) {
            BG_BMP_RAM(0)[(row_pos + row) * SCREEN_WIDTH / 2 + col_pos / 2 + col] = s[row * 40 / 2 + col];
        }
    }
}
