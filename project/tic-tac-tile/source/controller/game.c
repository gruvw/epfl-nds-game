#include <stdio.h>
#include <nds.h>

#include "../model/bot.h"
#include "../view/sprites.h"
#include "../view/graphics.h"

#include "wifi/packet.h"

#include "game.h"
#include "network.h"
#include "setters.h"
#include "audio.h"

// === Inputs / Touch Areas ===

// Game mode
#define SINGLE_PLAYER_TOUCHED(pos) (56 <= pos.px && pos.px <= 199 && 96 <= pos.py && pos.py <= 112)
#define TWO_PLAYER_TOUCHED(pos) (56 <= pos.px && pos.px <= 199 && 128 <= pos.py && pos.py <= 144)
#define TWO_PLAYER_WIFI_TOUCHED(pos) (56 <= pos.px && pos.px <= 199 && 160 <= pos.py && pos.py <= 176)

// Game speed
#define FAST_TOUCHED(pos) (32 <= pos.px && pos.px <= 63 && 48 <= pos.py && pos.py <= 79)
#define MEDIUM_TOUCHED(pos) (112 <= pos.px && pos.px <= 143 && 48 <= pos.py && pos.py <= 79)
#define SLOW_TOUCHED(pos) (192 <= pos.px && pos.px <= 223 && 48 <= pos.py && pos.py <= 79)

// Keys
#define PRESSED_ONCE(key) ((~REG_KEYINPUT & (key)) != 0 && ((pressed_keys & (key)) == 0) && ((pressed_keys |= (key)) || 1))

// === Globals ===

// State
GameState game_state;
GameState next_game_state;
TimerState timer_state;

Board board;
Coords selection_coords;
Cell local_side;
Cell active_side;

u8 time_left;  // in progress bar tiles (from 0 to STARTING_TIME)

// Settings
GameMode game_mode;
GameSpeed game_speed;

// === Utilities ===

void reset_game() {
    board = START_BOARD;
    selection_coords = MID_MID;  // selection on center on start
    local_side = active_side = STARTING_SIDE;  // `local_side` overwritten by Wi-Fi

    local_packet_reset();

    set_timer_state(T_UNUSED);
    set_time_left(STARTING_TIME);

    clear_game_screen();

    hide_game_over();
    hide_game_over_sprites();
    show_begin();

    menu_audio();
}

void refresh_game_screen() {
    clear_game_screen();
    draw_select(selection_coords);
    draw_board(board);
}

// === Interrupt handlers ===

// FSM state transitions manager
void game_fsm() {
    if (game_mode == TWO_PLAYER_WIFI && game_state == next_game_state) {
        // Only process Wi-Fi if nothing else to do
        wifi_process();
    }

    if (game_state == next_game_state) {  // nothing to do
        return;
    }

    if (next_game_state == G_BEGIN) {  // settings selection
        reset_game();
    }

    if (next_game_state == G_RUNNING) {  // start the game
        if (local_side == active_side) {
            set_timer_state(T_STARTED);
        }

        hide_begin();
        refresh_game_screen();

        game_audio();
    }

    if (next_game_state == G_FINISHED) {  // game over
        set_timer_state(T_OVER);
        show_game_over();

        Winner a = winner_of(board);
        clear_game_screen();

        // Draw selection around winning side
        if (a.side != EMPTY) {
            for (size_t i = 0; i < SIDE; i++) {
                draw_select((Coords) (a.start + i * a.direction));
            }
        }

        draw_board(board);

        // Show end sprites: winner and lost by time or not
        if (time_left > 0) {
            show_game_over_sprites(a.side, false);
        } else {
            show_game_over_sprites(OTHER_SIDE(active_side), true);
        }

        game_over_audio();
    }

    game_state = next_game_state;  // commit new FSM state
}

// Handles game's progress bar
void progress_timer_handler() {
    timer_counter++;
}

// Handles Wi-Fi timer increment, avoid spamming messages
void wifi_timer_handler() {
    if (game_state == G_RUNNING && timer_state == T_STARTED) {
        if (time_left > 0) {
            set_time_left(time_left - 1);  // updates time_left
        }
        if (time_left <= 0) {
            if (game_mode == TWO_PLAYER_WIFI) {
                // Local has no time left
                register_message((Message) { M_TIME });
            }

            next_game_state = G_FINISHED;
        }
    }
}

// Key presses manager
void keys_handler() {
    u16 pressed_keys = 0;  // reset register used for PRESSED_ONCE

    if (game_state == G_BEGIN && PRESSED_ONCE(KEY_START)) {  // attempt to start game
        if (game_mode == TWO_PLAYER_WIFI) {
            if (wifi_state == W_PAIRED) {  // wait for a connection
                // Local started the game, first to press START starts the game
                register_message((Message) { M_START });
                next_game_state = G_RUNNING;
            }
        } else {
            next_game_state = G_RUNNING;
        }
    }

    if (game_state == G_RUNNING) {
        // Direction selected control
        if (PRESSED_ONCE(KEY_RIGHT) && selection_coords < BOTTOM_RIGHT) {
            selection_coords += COL_INCR;
        } else if (PRESSED_ONCE(KEY_LEFT) && selection_coords > TOP_LEFT) {
            selection_coords -= COL_INCR;
        } else if (PRESSED_ONCE(KEY_DOWN) && selection_coords < BOTTOM_LEFT) {
            selection_coords += ROW_INCR;
        } else if (PRESSED_ONCE(KEY_UP) && selection_coords > TOP_RIGHT) {
            selection_coords -= ROW_INCR;
        }

        // Place cell on selection
        if (PRESSED_ONCE(KEY_A) && cell_at(board, selection_coords) == EMPTY && local_side == active_side) {
            board = placed_cell(board, active_side, selection_coords);
            select_audio(false);

            // Next state to do
            if (game_mode == SINGLE_PLAYER && !is_finished(board)) {
                board = bot_placed_cell(board);  // bot plays
            }
            if (game_mode == TWO_PLAYER_LOCAL) {
                local_side = active_side = OTHER_SIDE(active_side);  // switch local player
            }
            if (game_mode == TWO_PLAYER_WIFI) {
                register_message((Message) { M_PLAY, selection_coords });  // send move to opponent
                active_side = OTHER_SIDE(active_side);
                set_timer_state(T_UNUSED);  // wait for opponent to play
            }

            set_time_left(STARTING_TIME);
        }

        if (PRESSED_ONCE(KEY_START)) {
            // Terminate the game early
            next_game_state = G_FINISHED;
            if (game_mode == TWO_PLAYER_WIFI) {
                register_message((Message) { M_STOP });  // local terminated game early
            }
        }

        if (is_finished(board)) {
            next_game_state = G_FINISHED;
        }

        refresh_game_screen();
    }

    if (game_state == G_FINISHED) {
        if (PRESSED_ONCE(KEY_START)) {  // restart game
            next_game_state = G_BEGIN;
        }
    }
}

// Touch screen handler
void touch_handler() {
    if (game_state == G_BEGIN) {
        scanKeys();

        touchPosition pos;
        touchRead(&pos);

        // Settings menu
        if (SINGLE_PLAYER_TOUCHED(pos)) {
            set_game_mode(SINGLE_PLAYER);
            select_audio(true);
            local_packet_reset();
        } else if (TWO_PLAYER_TOUCHED(pos)) {
            set_game_mode(TWO_PLAYER_LOCAL);
            select_audio(true);
            local_packet_reset();
        } else if (TWO_PLAYER_WIFI_TOUCHED(pos)) {
            select_audio(true);
            if (wifi_state != W_PAIRED && wifi_setup()) {
                set_game_mode(TWO_PLAYER_WIFI);
            }
        } else if (FAST_TOUCHED(pos)) {
            set_game_speed(FAST);
            select_audio(true);
        } else if (MEDIUM_TOUCHED(pos)) {
            set_game_speed(MEDIUM);
            select_audio(true);
        } else if (SLOW_TOUCHED(pos)) {
            set_game_speed(SLOW);
            select_audio(true);
        }
    }
}

// === Game ===

void game_setup() {
    graphics_setup();
    audio_setup();

    // Golbals
    reset_game();
    next_game_state = game_state = G_BEGIN;

    // Default game settings, not reset on game over
    set_game_mode(SINGLE_PLAYER);
    set_game_speed(SLOW);

    // Timer Interrupts
    TIMER_DATA(1) = TIMER_FREQ_64(60);  // timer FSM (update game 60 times per seconds)
    TIMER_CR(1) = TIMER_ENABLE | TIMER_DIV_64 | TIMER_IRQ_REQ;
    irqSet(IRQ_TIMER(1), &game_fsm);
    irqEnable(IRQ_TIMER(1));

    // Registers are set in `set_game_speed`
    irqSet(IRQ_TIMER(0), &progress_timer_handler);
    irqEnable(IRQ_TIMER(0));

    TIMER_DATA(2) = TIMER_FREQ_64(10);  // Wi-Fi delay messages spam (increment every 100ms)
    TIMER_CR(2) = TIMER_ENABLE | TIMER_DIV_64 | TIMER_IRQ_REQ;
    irqSet(IRQ_TIMER(2), &wifi_timer_handler);
    irqEnable(IRQ_TIMER(2));

    // Button Interrupts
    REG_KEYCNT = BIT(14) | KEY_UP | KEY_DOWN | KEY_RIGHT | KEY_LEFT | KEY_A | KEY_START;
    irqSet(IRQ_KEYS, &keys_handler);
    irqEnable(IRQ_KEYS);
}

void game_loop() {
    while (1) {
        touch_handler();

        swiWaitForVBlank();
    }
}
