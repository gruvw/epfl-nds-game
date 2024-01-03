#include <stdlib.h>
#include "WiFi_minilib.h"

#include "packet.h"
#include "queue.h"

// === Macros / Types ===

#define MAX_ID 0xFE

typedef u8 DeviceID;

typedef struct {
    // char game_id;
    PacketType packet_type;
    PacketID packet_id;
    DeviceID sender_id;
    DeviceID reciever_id;
    Message content;
} PacketData;  // raw packet, not yet valid, only checked for correct game and size

#define NULL_PACKET_DATA (PacketData) { P_NONE }
#define NULL_PACKET (Packet) { P_NONE }
#define NULL_MESSAGE (Message) { M_NONE }

#define COUNTER_MAX 5 // number of 100ms delays before Wi-Fi message resent
#define COUNTER_DONE (timer_counter >= COUNTER_MAX && (timer_counter = 0 || 1))

#define VALID_PACKET_DATA(pd) (((pd).packet_type == P_DATA || (pd).packet_type == P_ACK) && (pd).sender_id == paired_id && (pd).reciever_id == local_id)

// === Globals ===

WiFiState wifi_state;

DeviceID local_id;  // this NDS
DeviceID paired_id;  // opponent NDS

u8 timer_counter;  // incremented by timer every 100ms

Queue packet_queue;

u8 next_packet_id;  // for sending packets
u8 last_acked_packet_id;  // for received packets

// === Setup function ===

void local_packet_reset() {
    wifi_state = W_ALONE;

    // Generate random ID between [1, MAX_ID]
    local_id = (DeviceID) ((rand() % MAX_ID) + 1);
    paired_id = 0;  // opponent not yet connected

    timer_counter = 0;
    packet_queue = (Queue) {{0}, 0, 0};

    next_packet_id = 1;
    last_acked_packet_id = 0;
}

// === Utility functions ===

void send_packet(Packet packet) {
    char data[PACKET_SIZE] = {
        GAME_ID,
        packet.type,
        (char) packet.id,
        (char) local_id,
        (char) paired_id,
        packet.content.type,
        (char) packet.content.arg,
    };
    sendData(data, PACKET_SIZE);
}

PacketData receive_packet_data() {
    char data[PACKET_SIZE];

    // Check for correct game and size
    if (receiveData(data, PACKET_SIZE) == PACKET_SIZE && data[0] == GAME_ID) {
        return (PacketData) { data[1], data[2], data[3], data[4], data[5], data[6] };
    }

    return NULL_PACKET_DATA;
}

Packet process_packet_data(PacketData packet_data) {
    // Supposes local has been paired

    if (packet_data.packet_type == P_NONE) {
        return NULL_PACKET;
    }

    if (packet_data.packet_type == P_DISCOVERY && packet_data.sender_id == paired_id) {
        // Inform broadcaster that I have been paired to them
        send_packet((Packet) { P_CONNECT });
        return NULL_PACKET;
    }

    // Connection establishment race resolution
    if (packet_data.sender_id == paired_id && packet_data.reciever_id != local_id) {
        // Packet sender is opponent but packet not destined to us
        // It means we lost a connection establishment race with sender with another NDS => we are not really paired
        local_packet_reset();
        return NULL_PACKET;
    }

    // Check packet attributes
    if (!VALID_PACKET_DATA(packet_data)) {
        return NULL_PACKET;
    }

    // Decode valid packet
    Packet packet = { packet_data.packet_type, packet_data.packet_id, packet_data.content };

    // Check if received packet is an ACK for our packet
    if (packet.type == P_ACK && packet.id == peek(&packet_queue).id) {
        // Last packet has been ACKed
        dequeue(&packet_queue);
        return NULL_PACKET;
    }

    // ACK the received (valid && non-ACK) packet
    send_packet((Packet) { P_ACK, packet.id });

    // Make sure not to process a packet twice
    if (packet.id == last_acked_packet_id) {
        // Packet was already ACKed, ACK was probably dropped
        return NULL_PACKET;  // don't process a second time
    }
    last_acked_packet_id = packet.id;

    return packet;
}

void send_pending_packets() {
    if (is_empty(&packet_queue)) {
        return;  // no pending packet to send
    }

    Packet packet = peek(&packet_queue);
    send_packet(packet);
}

// === Wi-Fi packet interface ===

void register_message(Message message) {
    enqueue(&packet_queue, (Packet) {
        P_DATA,
        next_packet_id++,
        message,
    });
}

Message receive_message() {
    PacketData packet_data = receive_packet_data();

    if (wifi_state == W_ALONE) {
        // Peer-to-peer Broadcast Ordered Connection Pairing (PBOCP) protocol

        // LEFT HERE (check tablet diagram + full packet review + TODOs)
        if (packet_data.packet_type == P_CONNECT && packet_data.reciever_id == local_id) {
            // Connection successfully established, both ways
            paired_id = packet_data.sender_id;
            wifi_state = W_PAIRED;
        }

        // Look for other discovering NDS, order requests to avoid simultaneous mutual connection
        if (packet_data.packet_type == P_DISCOVERY && packet_data.sender_id < local_id) {
            // We now have an paired NDS
            paired_id = packet_data.sender_id;
            wifi_state = W_PAIRED;
        } else if (COUNTER_DONE) {
            send_packet((Packet) { P_DISCOVERY });
        }

        return NULL_MESSAGE;
    }

    Packet packet = process_packet_data(packet_data);

    if (packet.type == P_NONE) {
        // No incoming packet to process

        if (COUNTER_DONE) {
            send_pending_packets();
        }

        return NULL_MESSAGE;
    }

    return packet.content;
}

//     if (wifi_state == W_WAIT) {
//         if (m.type == M_HELLO) {  // local was first online
//             local_side = STARTING_SIDE;
//             send_message((Message) { M_CONNECT, local_id });  // inform opponent
//         } else if (m.type == M_CONNECT) { // opponent was first online
//             local_side = OTHER_SIDE(STARTING_SIDE);
//         }
//     }

//     if (wifi_state == W_CONNECTED && m.type == M_START) {  // oponent started the game
//         wifi_state = next_wifi_state = W_PLAY;
//         next_game_state = G_RUNNING;
//     }

//     if (next_wifi_state == W_WAIT) { // started wifi game
//         send_message((Message) { M_HELLO, local_id });
//     }

//     char msg[MSG_SIZE];

//     // Process if Wi-Fi message was sent to us (same game, correct recipient)
//     if (receiveData(msg, MSG_SIZE) == MSG_SIZE && msg[0] == GAME_ID) {

//         if (msg[])

//         Message m = { msg[2], msg[3] };

//         if (!opponent_id && (m.type == M_HELLO || m.type == M_CONNECT)) {  // unregistered communication
//             opponent_id = m.arg;
//             next_wifi_state = W_CONNECTED;
//             process_message(m);
//         } else if ((DeviceID) msg[1] != local_id) {  // check destination ID, registered communication
//             process_message(m);
//         }
//     }

// void send_message(Message m) {

//     char msg[MSG_SIZE] = { GAME_ID, current_send_message, (char) opponent_id, m.type, (char) m.arg };
//     sendData(msg, MSG_SIZE);
//     do {

//         for (size_t i = 0; i < 60; i++) {
//             swiWaitForVBlank();
//         }

//         char ack[MSG_SIZE];
//         int received = receiveData(ack, MSG_SIZE);
//         acked = received == MSG_SIZE && msg[0] == GAME_ID && msg[1] == current_send_message && msg[3] == M_ACK;
//     } while (!acked);
// }
