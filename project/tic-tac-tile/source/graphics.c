#include <nds.h>
#include <stddef.h>
#include <string.h>

#include "nds/arm9/background.h"
#include "nds/arm9/video.h"
#include "nds/ndstypes.h"

#include "a-palette.h"
#include "b-background.h"
#include "c-cross.h"
#include "d-circle.h"
#include "e-select.h"

#define CROSS_PALETTE_INCR 2
#define CIRCLE_PALETTE_INCR 3
#define SELECT_PALETTE_INCR 4

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
    correct_palette((void *) d_circleBitmap, d_circleBitmapLen, CIRCLE_PALETTE_INCR);
    correct_palette((void *) e_selectBitmap, e_selectBitmapLen, SELECT_PALETTE_INCR);
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

void overlay_sprite(void * sprite, size_t row_pos, size_t col_pos, size_t side) {
    const u16 * s = sprite;
    for (size_t col = 0; col < side / 2; col++) {
        for (size_t row = 0; row < side; row++) {
            BG_BMP_RAM(0)[(row_pos + row) * SCREEN_WIDTH / 2 + col_pos / 2 + col] = s[row * side / 2 + col];
        }
    }
}

void main_graphics() {
    overlay_sprite((void *) e_selectBitmap, 72, 104, 48);
    // overlay_sprite((void *) e_selectBitmap, 122, 54, 48);

    overlay_sprite((void *) c_crossBitmap, 76, 108, 40);
    overlay_sprite((void *) d_circleBitmap, 126, 58, 40);
}
