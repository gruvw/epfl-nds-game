#include "../model/board.h"
#include "../view/sprites.h"

#include "wifi/packet.h"
#include "wifi/WiFi_minilib.h"

#include "audio.h"
#include "game.h"
#include "setters.h"

#include "network.h"

#define STATE_FROM(connected) (connected ? WS_PAIRED : WS_WIFI)

// === Globals ===

WifiState wifi_state = WS_NO_WIFI;  // used to switch connected sprite

// === Wi-Fi helper functions ===

void wifi_reset(bool display) {
    packet_connection_reset();

    if (display) {
        show_connection_sprite(wifi_state);
    } else {
        hide_wifi_sprites();
    }
}

// Tries to fully setup Wi-Fi connection to AP, returns `true` if successful, `false` otherwise
bool wifi_setup() {
    //// AS WIFI DISCONNECT IS NOT WORKING CORRECTLY
    wifi_reset(true);
    if (wifi_state != WS_NO_WIFI) {
        return true;
    }

    //// IF WIFI DISCONNECT WAS WORKING CORRECTLY
    // wifi_state = WS_NO_WIFI;
    // wifi_reset(true);
    // closeSocket();
    // disconnectFromWiFi();

    if (initWiFi() > 0 && openSocket() > 0) {
        wifi_state = WS_WIFI;
        show_connection_sprite(wifi_state);
        return true;
    }

    hide_wifi_sprites();

    return false;
}

// === Wi-Fi messages processing ===

void wifi_process() {
    // Must be connected to Wi-Fi AP to continue
    if (wifi_state == WS_NO_WIFI) {
        return;
    }

    Message msg = receive_message();

    // Show connected sprite when state changes
    WifiState current_state = STATE_FROM(packet_is_connected());
    if (wifi_state != current_state) {
        wifi_state = current_state;
        show_connection_sprite(wifi_state);
    }

    if (msg.type == M_NONE || !packet_is_connected()) {
        return;
    }

    if (game_state == G_BEGIN && msg.type == M_START) {
        // Opponent started the game, they go first
        local_side = OTHER_SIDE(STARTING_SIDE);
        next_game_state = G_RUNNING;
        return;
    }

    // Simultaneous start game conflict, leader keeps the game started
    if (game_state == G_RUNNING && msg.type == M_START && !is_connection_leader()) {
        // Reset current game state
        local_side = OTHER_SIDE(STARTING_SIDE);
        active_side = STARTING_SIDE;
        board = START_BOARD;
        set_timer_state(T_UNUSED);
        set_progress_time_left(STARTING_TIME);
        refresh_game_screen();
    }

    // Only process further messages when game is running
    if (game_state != G_RUNNING) {
        return;
    }

    if (msg.type == M_PLAY && active_side != local_side) {
        // Opponent played a move
        board = placed_cell(board, active_side, (Coords) msg.arg);
        select_sound(false);
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
        progress_time_left = 0;
        next_game_state = G_FINISHED;
    }
}
