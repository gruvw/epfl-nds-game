#include <nds.h>
#include <stddef.h>
#include <string.h>

#include "nds/arm9/background.h"
#include "nds/ndstypes.h"

#include "a-palette.h"
#include "b-background.h"
#include "c-cross.h"

void setBackground() {
    // 1) VRAM Configuration for MAIN engine
    VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;

    // 2) Main engine configuration in rotoscale mode
    REG_DISPCNT = MODE_5_2D | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE;

    // 3) Configure the background (Ignore warning 'large implictly truncated to unsigned type')
    BGCTRL[3] = BG_BMP_BASE(0) | BgSize_B8_256x256;

    swiCopy(a_palettePal, BG_PALETTE, a_palettePalLen / 2);
    swiCopy(b_backgroundBitmap, BG_BMP_RAM(0), b_backgroundBitmapLen/2);

    bgTransform[3]->hdx = 1 * 256;
    bgTransform[3]->vdx = 0 * 256;
    bgTransform[3]->hdy = 0 * 256;
    bgTransform[3]->vdy = 1 * 256;
    bgTransform[3]->dx = 0 * 256;
    bgTransform[3]->dy = 0 * 256;
}

void setCross() {
    // 3) Configure the background (Ignore warning 'large implictly truncated to unsigned type')
    BGCTRL[2] = BG_BMP_BASE(3) | BgSize_B8_256x256;

    const u16 * s = (u16 *) c_crossBitmap;
    // for (size_t col = 0; col < 40 / 2; col++) {
    //     for (size_t row = 0; row < 40; row++) {
    //         *(BG_BMP_RAM(3) + row * 256 / 2 + col) = s[row * 40 / 2 + col];
    //     }
    // }

    // const u16 * s = (u16 *) CrossBitmap;
    for (size_t col = 0; col < 40 / 2; col++) {
        for (size_t row = 0; row < 40; row++) {
            *(BG_BMP_RAM(3) + row * 256 / 2 + col) = s[row * 40 / 2 + col];
        }
    }

    bgTransform[2]->hdx = 1 * 256;
    bgTransform[2]->vdx = 0 * 256;
    bgTransform[2]->hdy = 0 * 256;
    bgTransform[2]->vdy = 1 * 256;
    bgTransform[2]->dx = 0 * 256;
    bgTransform[2]->dy = 0 * 256;
}
