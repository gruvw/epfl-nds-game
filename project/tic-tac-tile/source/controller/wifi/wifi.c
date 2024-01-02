#include <stdlib.h>
#include "WiFi_minilib.h"

#include "../game.h"
#include "../setters.h"
#include "../audio.h"
#include "../../model/board.h"

#include "wifi.h"

#define MAX_ID 0xFE
#define MSG_SIZE 4
#define GAME_ID ((char) 0b1110110)

typedef u8 DeviceID;

typedef enum {
    M_HELLO,
    M_CONNECT,
    M_START,
    M_PLAY,
    M_TIME, // TODO
    M_STOP,
} MessageType;

typedef struct {
    MessageType type;
    u8 arg;
} Message;

bool wifi_ok;  // Wi-Fi was setup correctly

WifiState wifi_state;
WifiState next_wifi_state;

DeviceID local_id;  // this NDS
DeviceID opponent_id;  // opponent NDS

void wifi_reset() {
    wifi_state = W_TERMINATED;
    next_wifi_state = W_WAIT;  // force first FSM transition

    // Generate random ID between [1, MAX_ID]
    local_id = (DeviceID) ((rand() % MAX_ID) + 1);
    opponent_id = 0;  // opponent not yet connected
}

bool wifi_setup() {
    if (wifi_ok) {
        return true;
    }

    return wifi_ok = initWiFi() > 0 && openSocket() > 0;
}

void send_message(Message m) {
    char msg[MSG_SIZE] = { GAME_ID, (char) opponent_id, m.type, (char) m.arg };
    sendData(msg, sizeof(*msg));
}

void send_move(Coords coords) {
    send_message((Message) { M_PLAY, coords });
}

void send_stop() {
    send_message((Message) { M_STOP });
}

void process_message(Message m) {
    if (wifi_state == W_WAIT && !opponent_id && (m.type == M_HELLO || m.type == M_CONNECT)) {
        // We now have an opponent NDS
        opponent_id = m.arg;
        next_wifi_state = W_CONNECTED;

        // First one who sent `M_HELLO` starts playing
        local_side = m.type == M_HELLO ? STARTING_SIDE : OTHER_SIDE(STARTING_SIDE);

        if (m.type == M_HELLO) {
            send_message((Message) { M_CONNECT, local_id });
        }
    }

    if (wifi_state == W_CONNECTED && m.type == M_START) {
        wifi_state = next_wifi_state = W_PLAY;
        next_game_state = G_RUNNING;
    }

    if (wifi_state == W_PLAY) {
        if (m.type == M_PLAY && active_side != local_side) {
            board = placed_cell(board, active_side, (Coords) m.arg);
            select_audio(false);
            active_side = OTHER_SIDE(active_side);

            if (is_finished(board)) {
                next_game_state = G_FINISHED;
                wifi_state = next_wifi_state = W_TERMINATED;
            } else {
                set_timer_state(T_STARTED);
            }
        }

        if (m.type == M_STOP) {
            next_game_state = G_FINISHED;
            wifi_state = next_wifi_state = W_TERMINATED;
        }

        if (m.type == M_TIME) {
            // TODO
        }
    }
}

void receive_messages() {
    if (wifi_state == W_TERMINATED) {
        return;
    }

    char msg[MSG_SIZE];

    // Process if Wi-Fi message was sent to us (same game, correct recipient)
    if (receiveData(msg, sizeof(*msg)) == MSG_SIZE &&
        msg[0] == GAME_ID &&
        (DeviceID) msg[1] == local_id) {
        process_message((Message) { msg[2], msg[3] });
    }
}

// FSM Wi-Fi state transitions manager
void wifi_fsm() {
    receive_messages();

    if (wifi_state == next_wifi_state) {
        return;
    }

    if (next_wifi_state == W_WAIT) {
        send_message((Message) { M_HELLO, local_id });
    }

    if (next_wifi_state == W_PLAY) {
        // Send `M_START`, first to press START starts the game
        send_message((Message) { M_START });
    }

    wifi_state = next_wifi_state;  // commit new FSM Wi-Fi state
}
