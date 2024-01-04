#include <nds.h>

#include "../model/bot.h"
#include "../view/sprites.h"
#include "../view/graphics.h"

#include "wifi/packet.h"

#include "game.h"
#include "network.h"
#include "setters.h"
#include "audio.h"

// Keys
#define PRESSED_ONCE(key) ((~REG_KEYINPUT & (key)) != 0 && ((pressed_keys & (key)) == 0) && ((pressed_keys |= (key)) || 1))

// === Interrupt handlers (ISRs) ===

// FSM state transitions manager
void game_fsm() {
    if (game_mode == TWO_PLAYER_WIFI && game_state == next_game_state) {
        // Only process Wi-Fi if nothing else to do already
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

        game_music();
    }

    if (next_game_state == G_FINISHED) {  // game over
        hide_wifi_sprites();

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

        game_over_sound();
    }

    game_state = next_game_state;  // commit new FSM state
}

// Handles game's progress bar
void progress_timer_handler() {
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

// Handles Wi-Fi timer increment, avoid spamming messages
void wifi_timer_handler() {
    timer_counter++;
}

// Key presses manager
void keys_handler() {
    u16 pressed_keys = 0;  // reset register used for `PRESSED_ONCE`

    if (game_state == G_BEGIN) {
        if (PRESSED_ONCE(KEY_START)) {  // attempt to start game
            if (game_mode == TWO_PLAYER_WIFI) {
                if (is_connected()) {  // wait for a connection before starting
                    // Players must not start at the same time!

                    // Local started the game, they go first
                    active_side = STARTING_SIDE;
                    register_message((Message) { M_START });
                    next_game_state = G_RUNNING;
                }
            } else {
                next_game_state = G_RUNNING;
            }
        } else if (PRESSED_ONCE(KEY_SELECT) && game_mode == TWO_PLAYER_WIFI) {
            // Use start to fully reinitiate connection establishment protocol
            wifi_reset();
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
            select_sound(false);

            // Next state to do
            if (game_mode == SINGLE_PLAYER && !is_board_finished(board)) {
                board = bot_placed_cell(board);  // bot plays
            } else if (game_mode == TWO_PLAYER_LOCAL) {
                local_side = active_side = OTHER_SIDE(active_side);  // switch local player
            } else if (game_mode == TWO_PLAYER_WIFI) {
                register_message((Message) { M_PLAY, selection_coords });  // send move to opponent
                set_timer_state(T_UNUSED);  // wait for opponent to play
                active_side = OTHER_SIDE(active_side);
            }

            set_time_left(STARTING_TIME);
        }

        if (PRESSED_ONCE(KEY_START)) {
            // Terminate the game early
            next_game_state = G_FINISHED;
            if (game_mode == TWO_PLAYER_WIFI) {
                register_message((Message) { M_STOP });
            }
        }

        if (is_board_finished(board)) {
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

// === Public interrupts interface ===

void interrupts_setup() {
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
    REG_KEYCNT = BIT(14) | KEY_UP | KEY_DOWN | KEY_RIGHT | KEY_LEFT | KEY_A | KEY_START | KEY_SELECT;
    irqSet(IRQ_KEYS, &keys_handler);
    irqEnable(IRQ_KEYS);
}
