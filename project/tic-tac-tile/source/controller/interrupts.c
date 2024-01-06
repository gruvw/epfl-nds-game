#include <nds.h>

#include "../model/bot.h"
#include "../view/sprites.h"
#include "../view/graphics.h"

#include "nds/input.h"
#include "wifi/packet.h"

#include "game.h"
#include "network.h"
#include "setters.h"
#include "audio.h"

#include "interrupts.h"

// === Helper Macros ===

#define DIFFICULTY_TO_PERC(diff) ((diff) == SLOW ? 100 : ((diff) == MEDIUM ? 130 : 160))

// === Keys Debounce Helper ===

typedef u16 DebounceTimer;
DebounceTimer debounce_timer = 1;
DebounceTimer last_debounce_timer = 0;

u16 last_key = 0;

bool is_pressed(u16 key) {
    int diff = abs((int) debounce_timer - (int) last_debounce_timer);

    // If pressed same button make sure it is not too close in time
    // At least 100ms between two presses of same button
    if ((~REG_KEYINPUT & key) == 0 || (key == last_key && diff <= 1)) {
        return false;
    }

    // Wait 3 times more for START and SELECT
    if ((key == KEY_START || key == KEY_SELECT) && diff <= 3) {
        return false;
    }

    last_key = key;
    last_debounce_timer = debounce_timer;
    return true;
}

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

        game_music(DIFFICULTY_TO_PERC(game_speed));
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
        if (progress_time_left > 0) {
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
        if (progress_time_left > 0) {
            set_progress_time_left(progress_time_left - 1);  // updates time_left
        }
        if (progress_time_left <= 0) {
            if (game_mode == TWO_PLAYER_WIFI) {
                // Local has no time left
                register_message((Message) { M_TIME });
            }

            next_game_state = G_FINISHED;
        }
    }
}

// Periodic timer increment handler
void timers_handler() {
    timer_counter++;  // handles Wi-Fi timer increment, avoid spamming messages
    debounce_timer++;  // handles button debounce increment
}

// Key presses interrupt handler
void keys_handler() {
    if (game_state == G_BEGIN) {
        if (is_pressed(KEY_START)) {  // attempt to start game
            if (game_mode == TWO_PLAYER_WIFI) {
                if (packet_is_connected()) {  // wait for a connection before starting
                    // Local started the game, they go first
                    active_side = STARTING_SIDE;
                    register_message((Message) { M_START });
                    next_game_state = G_RUNNING;
                    // See conflict resolution in `network.c::wifi_process` if start at same time
                }
            } else {
                next_game_state = G_RUNNING;
            }
        } else if (is_pressed(KEY_SELECT) && game_mode == TWO_PLAYER_WIFI) {
            // Use select to fully reinitiate Wi-Fi
            wifi_setup();
        }
    } else if (game_state == G_RUNNING) {
        // Selected cell direction control
        if (is_pressed(KEY_RIGHT) && selection_coords < BOTTOM_RIGHT) {
            selection_coords += COL_INCR;
        } else if (is_pressed(KEY_LEFT) && selection_coords > TOP_LEFT) {
            selection_coords -= COL_INCR;
        } else if (is_pressed(KEY_DOWN) && selection_coords < BOTTOM_LEFT) {
            selection_coords += ROW_INCR;
        } else if (is_pressed(KEY_UP) && selection_coords > TOP_RIGHT) {
            selection_coords -= ROW_INCR;
        }

        // Place cell on selection
        if (is_pressed(KEY_A) && cell_at(board, selection_coords) == EMPTY && local_side == active_side) {
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

            set_progress_time_left(STARTING_TIME);
        }

        if (is_pressed(KEY_START)) {
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
    } else if (game_state == G_FINISHED && is_pressed(KEY_START)) {
        next_game_state = G_BEGIN;  // back to menu
    }
}

// === Public interrupts interface ===

void interrupts_setup() {
    // --- Timer Interrupts ---

    // Game timer FSM
    TIMER_DATA(1) = TIMER_FREQ_64(60);  // update game state 60 times per seconds
    TIMER_CR(1) = TIMER_ENABLE | TIMER_DIV_64 | TIMER_IRQ_REQ;
    irqSet(IRQ_TIMER(1), &game_fsm);
    irqEnable(IRQ_TIMER(1));

    // Progress bar timer
    // Registers are set in `setters.c::set_game_speed`
    irqSet(IRQ_TIMER(0), &progress_timer_handler);
    irqEnable(IRQ_TIMER(0));

    // Wi-Fi delay messages spam external timer & Buttons debounce counter
    TIMER_DATA(2) = TIMER_FREQ_64(10); // increment every 100ms
    TIMER_CR(2) = TIMER_ENABLE | TIMER_DIV_64 | TIMER_IRQ_REQ;
    irqSet(IRQ_TIMER(2), &timers_handler);
    irqEnable(IRQ_TIMER(2));

    // Cannot use Wi-Fi with IRQ_TIMER(3) !

    // --- Button Interrupts ---

    REG_KEYCNT = BIT(14) | KEY_UP | KEY_DOWN | KEY_RIGHT | KEY_LEFT | KEY_A | KEY_START | KEY_SELECT;
    irqSet(IRQ_KEYS, &keys_handler);
    irqEnable(IRQ_KEYS);
}
