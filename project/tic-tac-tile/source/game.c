#include <stdio.h>

#include "bot.h"
#include "graphics.h"
#include "nds.h"
#include "nds/arm9/input.h"
#include "nds/input.h"
#include "nds/interrupts.h"

#define PRESSED(key) ((~REG_KEYINPUT & key) != 0)

// === Types ===

typedef enum {
    SINGLE_PLAYER,
    TWO_PLAYERS_LOCAL,
    TWO_PLAYERS_WIFI,
} GameMode;

typedef enum {
    BEGIN,
    RUNNING,
    FINISHED,
} GameState;

// === Gloabals ===

Board board;
Coords selection_coords;

GameMode game_mode;
GameState game_state;

Cell active_side;

// === Utilities ===

void reset_game() {
    board = START_BOARD;
    selection_coords = TOP_LEFT;
    game_state = BEGIN;
    active_side = CROSS;

    clear_game_screen();
}

void refresh_game_screen() {
    clear_game_screen();
    draw_select(selection_coords);
    draw_board(board);
}

// === Interrupt handlers ===

void keys_handler() {
    if (game_state == BEGIN) {
        if (PRESSED(KEY_START)) {
            game_state = RUNNING;
            refresh_game_screen();
        }
    }

    else if (game_state == RUNNING) {
        if (PRESSED(KEY_RIGHT) && selection_coords < BOTTOM_RIGHT) {
            selection_coords += COL_INCR;
        } else if (PRESSED(KEY_LEFT) && selection_coords > TOP_LEFT) {
            selection_coords -= COL_INCR;
        } else if (PRESSED(KEY_DOWN) && selection_coords < BOTTOM_LEFT) {
            selection_coords += ROW_INCR;
        } else if (PRESSED(KEY_UP) && selection_coords > TOP_RIGHT) {
            selection_coords -= ROW_INCR;
        } else if (PRESSED(KEY_A) && cell_at(board, selection_coords) == EMPTY) {
            board = placed_cell(board, active_side, selection_coords);
            if (game_mode == SINGLE_PLAYER) {
                board = bot_placed_cell(board);
            } else if (game_mode == TWO_PLAYERS_LOCAL) {
                active_side = (active_side == CROSS) ? CIRCLE : CROSS;
            }
        }

        if (winner_of(board).side != EMPTY || is_full(board) || PRESSED(KEY_START)) {
            game_state = FINISHED;
        }

        refresh_game_screen();
    }

    if (game_state == FINISHED) {
        Winner a = winner_of(board);
        clear_game_screen();

        if (a.side != EMPTY) {
            for (size_t i = 0; i < SIDE; i++) {
                draw_select(a.start + i * a.direction);
            }
        }

        draw_board(board);

        if (PRESSED(KEY_START)) {
            reset_game();
        }
    }
}

// === Game ===

void game_setup() {
    // Golbals
    reset_game();
    game_mode = SINGLE_PLAYER;

    // Interrupts
    REG_KEYCNT = BIT(14) | KEY_UP | KEY_DOWN | KEY_RIGHT | KEY_LEFT | KEY_A | KEY_START;

    irqSet(IRQ_KEYS, &keys_handler);
    irqEnable(IRQ_KEYS);
}

void game_loop() {
    while (1) {
        swiWaitForVBlank();
    }
}
