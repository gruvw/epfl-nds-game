#include <stdlib.h>

#include "../model/board.h"
#include "../view/sprites.h"

#include "wifi/packet.h"
#include "wifi/WiFi_minilib.h"

#include "network.h"
#include "audio.h"
#include "game.h"
#include "setters.h"

// === Globals ===

bool wifi_ok;  // Wi-Fi was setup correctly

bool last_is_connected;  // used to switch connected sprite

// === Wi-Fi helper functions ===

void wifi_reset() {
    last_is_connected = false;
    if (game_mode == TWO_PLAYER_WIFI) {
        show_connection_sprite(last_is_connected);
    }

    local_packet_reset();
}

bool wifi_setup() {
    wifi_reset();

    if (wifi_ok) {
        return true;
    }

    return wifi_ok = (initWiFi() > 0 && openSocket() > 0);
}

// === Wi-Fi messages processing ===

void wifi_process() {
    Message msg = receive_message();

    // Show connected sprite when state changes
    if (!last_is_connected && is_connected()) {
        last_is_connected = true;
        show_connection_sprite(last_is_connected);
    }

    if (msg.type == M_NONE || !is_connected()) {
        return;
    }

    if (game_state == G_BEGIN && msg.type == M_START) {
        // Opponent started the game, they go first
        local_side = OTHER_SIDE(STARTING_SIDE);
        next_game_state = G_RUNNING;
        return;
    }

    // Only process further messages when game is running
    if (game_state != G_RUNNING) {
        return;
    }

    if (msg.type == M_PLAY && active_side != local_side) {
        // Opponent played a move
        board = placed_cell(board, active_side, (Coords) msg.arg);
        select_audio(false);
        active_side = OTHER_SIDE(active_side);

        if (is_board_finished(board)) {
            // Game over
            next_game_state = G_FINISHED;
        } else {
            // Local NDS' turn
            set_timer_state(T_STARTED);
        }

        refresh_game_screen();
    } else if (msg.type == M_STOP) {
        // Opponent terminated game early
        next_game_state = G_FINISHED;
    } else if (msg.type == M_TIME && active_side != local_side) {
        // Opponent has no time left
        time_left = 0;
        next_game_state = G_FINISHED;
    }
}
