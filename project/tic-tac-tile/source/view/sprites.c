#include <nds.h>

#include "../controller/network.h"
#include "../model/board.h"

#include "graphics.h"

#include "j-cross-sprite.h"
#include "k-circle-sprite.h"
#include "l-timer-sprite.h"
#include "m-crown-sprite.h"
#include "n-sprite-palette.h"
#include "o-search-sprite.h"
#include "p-connected-sprite.h"
#include "q-wifi-sprite.h"

#include "sprites.h"

// === Palette corrections ===

#define CIRCLE_INCR 6
#define TIMER_INCR (CIRCLE_INCR + 3)
#define CROWN_INCR (TIMER_INCR + 2)
#define SEARCH_INCR (CROWN_INCR + 3)
#define CONNECTED_INCR (SEARCH_INCR + 1)
#define WIFI_INCR (CONNECTED_INCR + 2)

// === OAM Helpers ===

#define OAM_ALLOCATE oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color)
#define OAM_SHOW(sprite, row, col, id) oamSet(&oamSub, id, col, row, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, sprite, -1, 0, 0, 0, 0, 0)
#define OAM_HIDE(sprite, id) oamSet(&oamSub, id, 0, 0, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, sprite, -1, 0, 1, 0, 0, 0)

// === Sprites pointers ===

u16 * cross_sprite;  // id 1
u16 * circle_sprite;  // id 2
u16 * timer_sprite;  // id 3
u16 * crown_sprite;  // id 4
u16 * search_sprite;  // id 5
u16 * connected_sprite;  // id 6
u16 * wifi_sprite;  // id 7

// === Sprite palettes offsets ===

void sprites_palette_correction() {
    patch_palette(k_circle_spriteTiles, k_circle_spriteTilesLen, CIRCLE_INCR, false);
    patch_palette(l_timer_spriteTiles, l_timer_spriteTilesLen, TIMER_INCR, false);
    patch_palette(m_crown_spriteTiles, m_crown_spriteTilesLen, CROWN_INCR, false);
    patch_palette(o_search_spriteTiles, o_search_spriteTilesLen, SEARCH_INCR, false);
    patch_palette(p_connected_spriteTiles, p_connected_spriteTilesLen, CONNECTED_INCR, false);
    patch_palette(q_wifi_spriteTiles, q_wifi_spriteTilesLen, WIFI_INCR, false);
}

// === Sprites functions ===

void sprites_setup() {
    // Setup sprites on SUB display
    VRAM_I_CR = VRAM_ENABLE | VRAM_I_SUB_SPRITE;
    oamInit(&oamSub, SpriteMapping_1D_32, false);

    // Allocate sprites, no need to free, as game loop is infinite
    cross_sprite = OAM_ALLOCATE;
    circle_sprite = OAM_ALLOCATE;
    timer_sprite = OAM_ALLOCATE;
    crown_sprite = OAM_ALLOCATE;
    search_sprite = OAM_ALLOCATE;
    connected_sprite = OAM_ALLOCATE;
    wifi_sprite = OAM_ALLOCATE;

    // Patch sprites palette
    sprites_palette_correction();

    // Copy sprites
    swiCopy(j_cross_spriteTiles, cross_sprite, j_cross_spriteTilesLen / 2);
    swiCopy(k_circle_spriteTiles, circle_sprite, k_circle_spriteTilesLen / 2);
    swiCopy(l_timer_spriteTiles, timer_sprite, l_timer_spriteTilesLen / 2);
    swiCopy(m_crown_spriteTiles, crown_sprite, m_crown_spriteTilesLen / 2);
    swiCopy(o_search_spriteTiles, search_sprite, o_search_spriteTilesLen / 2);
    swiCopy(p_connected_spriteTiles, connected_sprite, p_connected_spriteTilesLen / 2);
    swiCopy(q_wifi_spriteTiles, wifi_sprite, q_wifi_spriteTilesLen / 2);

    // Copy palette
    swiCopy(n_sprite_paletteBitmap, SPRITE_PALETTE_SUB + 1, 18);
}

void show_game_over_sprites(Cell c, bool time) {
    if (c != EMPTY) {
        OAM_SHOW(c == CROSS ? cross_sprite : circle_sprite, 86, 112, 1);
        OAM_SHOW(crown_sprite, 64, 112, 4);
    }

    if (time) {
        OAM_SHOW(timer_sprite, 88, 146, 3);
    }

    oamUpdate(&oamSub);
}

void hide_game_over_sprites() {
    OAM_HIDE(cross_sprite, 1);
    OAM_HIDE(circle_sprite, 2);
    OAM_HIDE(timer_sprite, 3);
    OAM_HIDE(crown_sprite, 4);

    oamUpdate(&oamSub);
}

void hide_wifi_sprites() {
    OAM_HIDE(search_sprite, 5);
    OAM_HIDE(connected_sprite, 6);
    OAM_HIDE(wifi_sprite, 7);

    oamUpdate(&oamSub);
}

void show_connection_sprite(WifiState wifi_state) {
    hide_wifi_sprites();

    if (wifi_state == WS_NO_WIFI) {
        OAM_SHOW(wifi_sprite, 152, 200, 7);
    } else if (wifi_state == WS_WIFI) {
        OAM_SHOW(search_sprite, 152, 200, 5);
    } else if (wifi_state == WS_PAIRED) {
        OAM_SHOW(connected_sprite, 152, 200, 6);
    }

    oamUpdate(&oamSub);
}
