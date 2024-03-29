#include <nds.h>

#include "../controller/game.h"
#include "../model/board.h"

#include "sprites.h"

#include "a-palette.h"
#include "b-background.h"
#include "c-cross.h"
#include "d-circle.h"
#include "e-select.h"
#include "f-sub-background.h"
#include "g-sub-palette.h"
#include "h-begin.h"
#include "i-sub-finished.h"

#include "graphics.h"

// === Palette corrections ===

// Main display
#define BG_PALETTE_INCR 1
#define CROSS_PALETTE_INCR (BG_PALETTE_INCR + 2)
#define CIRCLE_PALETTE_INCR (CROSS_PALETTE_INCR + 6)
#define SELECT_PALETTE_INCR (CIRCLE_PALETTE_INCR + 3)
#define BEGIN_INCR (SELECT_PALETTE_INCR + 2)

// Sub display
#define BG_SUB_FINISHED_INCR 13

// === Types ===

typedef struct {
    size_t row;
    size_t col;
} ScreenPosition;

// === Utilities ===

// Translate board coords to screen position
ScreenPosition position_from_coords(Coords coords) {
    return (ScreenPosition) {
        26 + 50 * ROW(coords),
        58 + 50 * COL(coords),
    };
}

void set_bg_transform(size_t bg_nb) {
    bgTransform[bg_nb]->hdx = BIT(8);
    bgTransform[bg_nb]->vdx = 0;
    bgTransform[bg_nb]->hdy = 0;
    bgTransform[bg_nb]->vdy = BIT(8);
    bgTransform[bg_nb]->dx = 0;
    bgTransform[bg_nb]->dy = 0;
}

// === Images Palette Corrections ===

void patch_palette(void * data, size_t len, int increment, bool background) {
    unsigned char * r = data;
    for (size_t i = 0; i < len; i++) {
        r[i] += (r[i] == 0 && !background) ? 0 : increment;
    }
}

void images_palette_correction() {
    // Main display
    patch_palette((void *) b_backgroundBitmap, b_backgroundBitmapLen, BG_PALETTE_INCR, true);
    patch_palette((void *) c_crossBitmap, c_crossBitmapLen, CROSS_PALETTE_INCR, false);
    patch_palette((void *) d_circleBitmap, d_circleBitmapLen, CIRCLE_PALETTE_INCR, false);
    patch_palette((void *) e_selectBitmap, e_selectBitmapLen, SELECT_PALETTE_INCR, false);
    patch_palette((void *) h_beginBitmap, h_beginBitmapLen, BEGIN_INCR, true);

    // Sub display
    patch_palette((void *) i_sub_finishedBitmap, i_sub_finishedBitmapLen, BG_SUB_FINISHED_INCR, true);
}

// === Backgrounds ===

void set_backgrounds() {
    dmaCopy(b_backgroundBitmap, BG_BMP_RAM(3), b_backgroundBitmapLen);
    dmaCopy(f_sub_backgroundTiles, BG_TILE_RAM_SUB(1), f_sub_backgroundTilesLen);
    dmaCopy(f_sub_backgroundMap, BG_MAP_RAM_SUB(0), f_sub_backgroundMapLen);
    swiCopy(i_sub_finishedBitmap, BG_BMP_RAM_SUB(3), i_sub_finishedBitmapLen / 2);
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

    BGCTRL[2] = BgSize_B8_256x256 | BG_BMP_BASE(0);  // used for X, O, selected coords
    BGCTRL[3] = BgSize_B8_256x256 | BG_BMP_BASE(3);  // used for tic-tac-toe background, and begin menu
    BGCTRL_SUB[0] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);  // used for settings menu
    BGCTRL_SUB[2] = BgSize_B8_256x256 | BG_BMP_BASE(3);  // used for Game Over

    // Set up extended rotoscale affine matrix
    set_bg_transform(2);
    set_bg_transform(3);
    REG_BG2PA_SUB = BIT(8);
    REG_BG2PC_SUB = 0;
    REG_BG2PB_SUB = 0;
    REG_BG2PD_SUB = BIT(8);

    // Reset all backgrounds
    dmaFillHalfWords(0, BG_BMP_RAM(0), SCREEN_WIDTH * SCREEN_HEIGHT);
    dmaFillHalfWords(0, BG_BMP_RAM(3), SCREEN_WIDTH * SCREEN_HEIGHT);

    // Copy palettes
    swiCopy(a_paletteBitmap, BG_PALETTE + 1, 17);
    swiCopy(g_sub_paletteBitmap, BG_PALETTE_SUB + 1, 16);

    // Setup & Show
    images_palette_correction();
    set_backgrounds();

    // Sprites
    sprites_setup();
}

// === Graphics Drawing ===

void overlay_img(const u16 * img, size_t row_pos, size_t col_pos, size_t side) {
    for (size_t row = 0; row < side; row++) {
        size_t pos = (row_pos + row) * SCREEN_WIDTH / 2 + col_pos / 2;
        dmaCopy(img + row * side / 2, BG_BMP_RAM(0) + pos, side);
    }
}

void draw_select(Coords coords) {
    ScreenPosition pos = position_from_coords(coords);
    overlay_img((void*) e_selectBitmap, pos.row - 4, pos.col - 4, 48);
}

void draw_board(Board board) {
    for (Coords c = TOP_LEFT; c <= BOTTOM_RIGHT; c++) {
        if (cell_at(board, c) != EMPTY) {
            const void * side = cell_at(board, c) == CROSS ? c_crossBitmap : d_circleBitmap;
            ScreenPosition pos = position_from_coords(c);
            overlay_img(side, pos.row, pos.col, 40);
        }
    }
}

void clear_game_screen() {
    dmaFillHalfWords(0, BG_BMP_RAM(0), SCREEN_WIDTH * SCREEN_HEIGHT);
}

// === Intermediate Screens ===

void show_game_over() {
    REG_DISPCNT_SUB |= DISPLAY_BG2_ACTIVE;
    REG_DISPCNT_SUB &= ~DISPLAY_BG0_ACTIVE;
}

void hide_game_over() {
    REG_DISPCNT_SUB |= DISPLAY_BG0_ACTIVE;
    REG_DISPCNT_SUB &= ~DISPLAY_BG2_ACTIVE;
}

void show_begin() {
    dmaCopy(h_beginBitmap, BG_BMP_RAM(3), h_beginBitmapLen);
}

void hide_begin() {
    dmaCopy(b_backgroundBitmap, BG_BMP_RAM(3), b_backgroundBitmapLen);
}
