#include <nds.h>

#include "setters.h"
#include "game.h"

#include "f-sub-background.h"

// Contains a mix of graphics, timer, and controller / game logic
// Game helper functions, contextual setters

// === Palette colors mapping ===

#define SEL_MODE_TO_PALETTE(mode) BG_PALETTE_SUB[((mode) == SINGLE_PLAYER ? 12 : ((mode) == TWO_PLAYER_LOCAL ? 5 : 8))]
#define SEL_SPEED_TO_PALETTE(speed) BG_PALETTE_SUB[((speed) == SLOW ? 7 : ((speed) == MEDIUM ? 11 : 1))]
#define SELECTED_COLOR RGB15(5, 0, 30)

#define SPEED_TO_COLOR(speed) ((speed) == SLOW ? RGB15(7, 26, 14) : ((speed) == MEDIUM ? RGB15(28, 14, 5) : RGB15(21, 6, 6)))
#define PROGRESS_COLOR(mode, speed) ((mode) == T_UNUSED ? RGB15(31, 31, 31) : SPEED_TO_COLOR(speed))
#define PROGRESS_PALETTE 4

// === Selection colors ===

void set_game_mode(GameMode new_mode) {
    // Change selected game mode on screen
    SEL_MODE_TO_PALETTE(game_mode) = SEL_MODE_TO_PALETTE(new_mode);
    game_mode = new_mode;
    SEL_MODE_TO_PALETTE(game_mode) = SELECTED_COLOR;
}

void set_game_speed(GameSpeed new_speed) {
    // Change selected game speed on screen
    SEL_SPEED_TO_PALETTE(game_speed) = SEL_SPEED_TO_PALETTE(new_speed);
    game_speed = new_speed;
    SEL_SPEED_TO_PALETTE(new_speed) = SELECTED_COLOR;

    // Set timer registers for progress speed
    if (new_speed == SLOW) {
        TIMER_DATA(0) = TIMER_FREQ_256(2); // 500 ms per tile
        TIMER_CR(0) = TIMER_ENABLE | TIMER_DIV_256 | TIMER_IRQ_REQ;
    } else if (new_speed == MEDIUM) {
        TIMER_DATA(0) = TIMER_FREQ_64(8); // 125 ms per tile
        TIMER_CR(0) = TIMER_ENABLE | TIMER_DIV_64 | TIMER_IRQ_REQ;
    } else if (new_speed == FAST) {
        TIMER_DATA(0) = TIMER_FREQ_64(25); // 40 ms per tile
        TIMER_CR(0) = TIMER_ENABLE | TIMER_DIV_64 | TIMER_IRQ_REQ;
    }
}

// === Timer and progress ===

void set_timer_state(TimerState new_timer_state) {
    timer_state = new_timer_state;
    BG_PALETTE_SUB[PROGRESS_PALETTE] = PROGRESS_COLOR(new_timer_state, game_speed);
}

void set_time_left(u8 new_time_left) {
    // Change progress bar on screen
    for (size_t t = 0; t < STARTING_TIME; t++) {
        size_t tile_up = 68 + t, tile_down = tile_up + 32;  // 2 vertical tiles for per progress unit
        BG_MAP_RAM_SUB(0)[tile_up] = (t >= new_time_left ? 0 : f_sub_backgroundMap[tile_up]);
        BG_MAP_RAM_SUB(0)[tile_down] = (t >= new_time_left ? 0 : f_sub_backgroundMap[tile_down]);
    }

    time_left = new_time_left;
}
