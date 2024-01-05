#include <nds.h>
#include <time.h>

#include "../view/sprites.h"
#include "../view/graphics.h"

#include "wifi/packet.h"

#include "interrupts.h"
#include "network.h"
#include "setters.h"
#include "audio.h"

#include "game.h"

// === Touch Areas ===

// Game mode
#define SINGLE_PLAYER_TOUCHED(pos) (game_mode != SINGLE_PLAYER && 56 <= pos.px && pos.px <= 199 && 96 <= pos.py && pos.py <= 112)
#define TWO_PLAYER_TOUCHED(pos) (game_mode != TWO_PLAYER_LOCAL && 56 <= pos.px && pos.px <= 199 && 128 <= pos.py && pos.py <= 144)
#define TWO_PLAYER_WIFI_TOUCHED(pos) (game_mode != TWO_PLAYER_WIFI && 56 <= pos.px && pos.px <= 199 && 160 <= pos.py && pos.py <= 176)

// Game speed
#define SLOW_TOUCHED(pos) (game_speed != SLOW && 192 <= pos.px && pos.px <= 223 && 48 <= pos.py && pos.py <= 79)
#define MEDIUM_TOUCHED(pos) (game_speed != MEDIUM && 112 <= pos.px && pos.px <= 143 && 48 <= pos.py && pos.py <= 79)
#define FAST_TOUCHED(pos) (game_speed != FAST && 32 <= pos.px && pos.px <= 63 && 48 <= pos.py && pos.py <= 79)

// === Globals ===

// Game State
GameState game_state;
GameState next_game_state;
TimerState timer_state;

// Model state
Board board;
Coords selection_coords;
Cell local_side;
Cell active_side;

u8 progress_time_left;  // in progress bar tiles (from 0 to STARTING_TIME)

// Game Settings
GameMode game_mode;
GameSpeed game_speed;

// === Utilities ===

void reset_game() {
    // Reset model state
    board = START_BOARD;
    selection_coords = MID_MID;  // selection on center on start
    local_side = active_side = STARTING_SIDE;  // `local_side` overwritten by Wi-Fi

    wifi_reset(game_mode == TWO_PLAYER_WIFI);

    // Reset progress bar
    set_timer_state(T_UNUSED);
    set_progress_time_left(STARTING_TIME);

    clear_game_screen();

    // Begin menu
    hide_game_over();
    hide_game_over_sprites();
    show_begin();

    menu_music();
}

// Makes sure the current model state is accurately represented on the top screen
void refresh_game_screen() {
    clear_game_screen();
    draw_select(selection_coords);
    draw_board(board);
}

// Touch screen handler
void touch_handler() {
    // Only allow the touch screen when the game has not been started yet
    if (game_state != G_BEGIN) {
        return;
    }

    scanKeys();

    touchPosition pos;
    touchRead(&pos);

    // Change settings menu
    if (SINGLE_PLAYER_TOUCHED(pos)) {
        set_game_mode(SINGLE_PLAYER);
        select_sound(true);
        wifi_reset(false);
    } else if (TWO_PLAYER_TOUCHED(pos)) {
        set_game_mode(TWO_PLAYER_LOCAL);
        select_sound(true);
        wifi_reset(false);
    } else if (TWO_PLAYER_WIFI_TOUCHED(pos)) {
        select_sound(true);
        if (wifi_setup()) {
            // Successful Wi-Fi setup
            set_game_mode(TWO_PLAYER_WIFI);
        }
    } else if (FAST_TOUCHED(pos)) {
        set_game_speed(FAST);
        select_sound(true);
    } else if (MEDIUM_TOUCHED(pos)) {
        set_game_speed(MEDIUM);
        select_sound(true);
    } else if (SLOW_TOUCHED(pos)) {
        set_game_speed(SLOW);
        select_sound(true);
    }
}

// === Game ===

void game_setup() {
    graphics_setup();
    audio_setup();

    // Setup random seed
    srand(time(NULL));

    // Golbals
    reset_game();
    next_game_state = game_state = G_BEGIN;

    // Default game settings, not reset on game over
    set_game_mode(SINGLE_PLAYER);
    set_game_speed(SLOW);

    interrupts_setup();
}

void game_loop() {
    while (1) {
        touch_handler();

        swiWaitForVBlank();
    }
}
