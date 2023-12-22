#include <stdio.h>

#include "bot.h"
#include "sprites.h"
#include "graphics.h"
#include "controller.h"
#include "nds.h"
#include "nds/arm9/input.h"
#include "nds/input.h"
#include "nds/interrupts.h"

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

// === Gloabals ===

// State
GameState game_state;
GameState next_game_state;
TimerState timer_state;
Board board;
Coords selection_coords;
Cell active_side;
u16 pressed_keys;
u8 time_left;  // in progress bar tiles (from 0 to STARTING_TIME)

// Settings
GameMode game_mode;
GameSpeed game_speed;

// === Utilities ===

void reset_game() {
    board = START_BOARD;
    selection_coords = MID_MID;
    game_state = BEGIN;
    active_side = STARTING_SIDE;

    set_timer_state(UNUSED);
    set_time_left(STARTING_TIME);

    clear_game_screen();
    show_begin();
}

void refresh_game_screen() {
    clear_game_screen();
    draw_select(selection_coords);
    draw_board(board);
}

// === Interrupt handlers ===

void timer_fsm() {
    if (game_state == next_game_state) {
        return;
    }

    if (next_game_state == BEGIN) {
        reset_game();
        hide_game_over();
        hide_game_over_sprites();
        show_begin();
    }

    if (next_game_state == RUNNING) {
        set_timer_state(STARTED);
        hide_begin();
        refresh_game_screen();
    }

    if (next_game_state == FINISHED) {
        set_timer_state(OVER);
        show_game_over();

        Winner a = winner_of(board);
        clear_game_screen();

        if (a.side != EMPTY) {
            for (size_t i = 0; i < SIDE; i++) {
                draw_select(a.start + i * a.direction);
            }
        }

        draw_board(board);

        // Show end sprites (winner and lost by time or not)
        if (time_left > 0) {
            show_game_over_sprites(a.side, false);
        } else {
            show_game_over_sprites(OTHER_SIDE(active_side), true);
        }
    }

    game_state = next_game_state;
}

void timer_handler() {
    if (game_state == RUNNING && timer_state == STARTED) {
        if (time_left > 0) {
            set_time_left(time_left - 1);
        }
        if (time_left <= 0) {
            next_game_state = FINISHED;
        }
    }
}

void keys_handler() {
    pressed_keys = 0;

    if (game_state == BEGIN) {
        if (PRESSED_ONCE(KEY_START)) {
            next_game_state = RUNNING;
        }
    }

    if (game_state == RUNNING) {
        if (PRESSED_ONCE(KEY_RIGHT) && selection_coords < BOTTOM_RIGHT) {
            selection_coords += COL_INCR;
        } else if (PRESSED_ONCE(KEY_LEFT) && selection_coords > TOP_LEFT) {
            selection_coords -= COL_INCR;
        } else if (PRESSED_ONCE(KEY_DOWN) && selection_coords < BOTTOM_LEFT) {
            selection_coords += ROW_INCR;
        } else if (PRESSED_ONCE(KEY_UP) && selection_coords > TOP_RIGHT) {
            selection_coords -= ROW_INCR;
        } else if (PRESSED_ONCE(KEY_A) && cell_at(board, selection_coords) == EMPTY) {
            board = placed_cell(board, active_side, selection_coords);
            if (game_mode == SINGLE_PLAYER && !is_finished(board)) {
                board = bot_placed_cell(board);
            } else if (game_mode == TWO_PLAYER_LOCAL) {
                active_side = OTHER_SIDE(active_side);
            }
            set_time_left(STARTING_TIME);
        }

        if (is_finished(board) || PRESSED_ONCE(KEY_START)) {
            next_game_state = FINISHED;
        }

        refresh_game_screen();
    }

    if (game_state == FINISHED) {
        if (PRESSED_ONCE(KEY_START)) {
            next_game_state = BEGIN;
        }
    }
}

void touch_handler() {
    if (game_state == BEGIN) {
        scanKeys();

        touchPosition pos;
        touchRead(&pos);

        if (SINGLE_PLAYER_TOUCHED(pos)) {
            set_game_mode(SINGLE_PLAYER);
        } else if (TWO_PLAYER_TOUCHED(pos)) {
            set_game_mode(TWO_PLAYER_LOCAL);
        } else if (TWO_PLAYER_WIFI_TOUCHED(pos)) {
            set_game_mode(TWO_PLAYER_WIFI);
        } else if (FAST_TOUCHED(pos)) {
            set_game_speed(FAST);
        } else if (MEDIUM_TOUCHED(pos)) {
            set_game_speed(MEDIUM);
        } else if (SLOW_TOUCHED(pos)) {
            set_game_speed(SLOW);
        }
    }
}

// === Game ===

void game_setup() {
    // Golbals
    reset_game();

    // Default game settings (not reset on game over)
    set_game_mode(SINGLE_PLAYER);
    set_game_speed(SLOW);

    // Timer Interrupts
    TIMER_DATA(1) = TIMER_FREQ_64(50);  // timer FSM (update game 50 times per seconds)
    TIMER_CR(1) = TIMER_ENABLE | TIMER_DIV_64 | TIMER_IRQ_REQ;
    irqSet(IRQ_TIMER(1), &timer_fsm);
    irqEnable(IRQ_TIMER(1));

    // Registers are set in `set_game_speed`
    irqSet(IRQ_TIMER(0), &timer_handler);
    irqEnable(IRQ_TIMER(0));

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
