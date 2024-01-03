#include <stdlib.h>

#include "wifi/WiFi_minilib.h"
#include "wifi/packet.h"

#include "../model/board.h"

#include "network.h"
#include "game.h"
#include "setters.h"
#include "audio.h"

// === Globals ===

bool wifi_ok;  // Wi-Fi was setup correctly

// === Wi-Fi helper functions ===

bool wifi_setup() {
    local_packet_reset();

    if (wifi_ok) {
        return true;
    }

    return wifi_ok = (initWiFi() > 0 && openSocket() > 0);
}

// === Wi-Fi messages processing ===

void wifi_process() {
    Message rec_msg = receive_message();

    if (rec_msg.type == M_NONE) {
        return;
    }

    // TODO
    if (wifi_state == W_PLAY) {
        if (m.type == M_PLAY && active_side != local_side) {
            // Oponent played a move
            board = placed_cell(board, active_side, (Coords) m.arg);
            select_audio(false);
            active_side = OTHER_SIDE(active_side);

            if (is_finished(board)) {
                // Game over
                next_game_state = G_FINISHED;
            } else {
                // Local NDS' turn
                set_timer_state(T_STARTED);
            }

            refresh_game_screen();
        }

        if (m.type == M_STOP) {
            // Oponent terminated game early
            next_game_state = G_FINISHED;
        }

        if (m.type == M_TIME && active_side != local_side) {
            // Oponent has no time left
            time_left = 0;
            next_game_state = G_FINISHED;
        }
    }
}
