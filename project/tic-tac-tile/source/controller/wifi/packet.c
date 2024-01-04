#include "WiFi_minilib.h"

#include "packet.h"
#include "queue.h"

// === Macros / Types ===

typedef enum {
    BOP_NOT_PAIRED,
    BOP_LEADER_PAIRED,
    BOP_BIDIRECTIONALLY_PAIRED,
} BOPState;

typedef u16 DeviceID;
#define MAX_ID 0xFFFE

#define MSBYTE(id) ((id) >> 8)
#define ID_FROM(msbyte, lsbyte) ((((DeviceID) (msbyte)) << 8) | (lsbyte))

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

#define COUNTER_MAX 5 // number of 100ms delays before Wi-Fi packet resent (could implement linear/exponential backoff)
#define COUNTER_DONE (timer_counter >= COUNTER_MAX)

// === Globals ===

BOPState bop_state;

// P2P-BOP IDs
DeviceID local_id;  // this device
DeviceID paired_id;  // paired device

// Packet resend timer
u8 timer_counter;  // must be incremented by external timer every 100ms

Queue packet_queue;

u8 next_packet_id;  // for sending packets
u8 last_acked_packet_id;  // for received packets

// === Setup function ===

void local_packet_reset() {
    bop_state = BOP_NOT_PAIRED;  // reset P2P-BOP

    // Generate random ID between [1, MAX_ID), must already be seeded!
    local_id = (DeviceID) ((rand() % MAX_ID) + 1);
    paired_id = 0;  // opponent not yet connected

    timer_counter = 0;
    packet_queue = (Queue) {{0}, 0, 0};

    next_packet_id = 1;
    last_acked_packet_id = 0;
}

// === Helper functions ===

bool valid_packet_data(PacketData pd) {
    return ((pd).packet_type == P_DATA || (pd).packet_type == P_ACK) && (pd).sender_id == paired_id && (pd).reciever_id == local_id;
}

// === Packet exchange functions ===

void send_packet(Packet packet) {
    char data[PACKET_SIZE] = {
        GAME_ID,
        packet.type,
        (char) packet.id,
        (char) MSBYTE(local_id),  // sender
        (char) local_id,
        (char) MSBYTE(paired_id),  // receiver
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
        return (PacketData) {
            data[1], data[2],
            ID_FROM(data[3], data[4]),  // sender
            ID_FROM(data[5], data[6]),  // receiver
            data[7], data[8],
        };
    }

    return NULL_PACKET_DATA;
}

// Peer-to-peer Broadcast Ordered Pairing (P2P-BOP) protocol
// Returns `true` if the packet_data can be used after call, `false` otherwise (packet consumed, or if not paired)
bool p2p_bop(PacketData packet_data) {
    if (bop_state == BOP_NOT_PAIRED) {
        // Look for a discovering BOP agent, order requests to avoid simultaneous mutual connection
        if (packet_data.packet_type == P_DISCOVERY && packet_data.sender_id < local_id) {
            // Local device is BOP leader, the one responsible for connection establishment

            // Locally pair to BOP agent
            paired_id = packet_data.sender_id;
            bop_state = BOP_LEADER_PAIRED;

            // Inform other device of pairing
            send_packet((Packet) { P_CONNECT });
        } else if (packet_data.packet_type == P_CONNECT && packet_data.reciever_id == local_id) {
            // Local device is BOP agent, reacting to BOP leader's pairing request

            // BOP leader is already locally paired to this device, pair to it too
            paired_id = packet_data.sender_id;
            bop_state = BOP_BIDIRECTIONALLY_PAIRED;

            // Inform other device of connection establishment
            send_packet((Packet) { P_ESTABLISHED });
        } else if (COUNTER_DONE) {
            // Continuously inform BOP leader of discovering state
            send_packet((Packet) { P_DISCOVERY });
        }

        return false;
    }

    if (bop_state == BOP_LEADER_PAIRED) {
        // Note: protocol can be stuck (in unconnected state) here if locally paired BOP agent
        // never sends additional packets (e.g.: because reset), or packets never reach local device.
        // Use timeout or application reset (call `local_packet_reset`).

        if (packet_data.sender_id != paired_id) {
            // Ignore packets sent by peers other than BOP agent
        } else if (packet_data.packet_type == P_DISCOVERY) {  // NOLINT(bugprone-branch-clone)
            // Locally paired BOP agent is still discovering
            // Do no check receiver id yet, it is invalid as not known by BOP agent at this point

            // Inform BOP agent (sender) that BOP leader (local) is locally paired to them
            send_packet((Packet) { P_CONNECT });  // previous `P_CONNECT` packet was probably dropped
        } else if (packet_data.reciever_id != local_id) {
            // Connection establishment race resolution
            // When packet comes from BOP agent but is not destined to local device (does not depend on `packet_type`)

            // It means local device lost a connection establishment race for locally paired BOP agent to another BOP leader
            // BOP agent is paired with another BOP leader, local device lost BOP agent :(
            local_packet_reset();
        } else if (packet_data.packet_type == P_ESTABLISHED) {
            // Connection establishment confirmed by BOP agent
            bop_state = BOP_BIDIRECTIONALLY_PAIRED;
        } else if (COUNTER_DONE) {
            // Continuously inform BOP agent that we want to connect
            send_packet((Packet) { P_CONNECT });
        }

        return false;
    }

    if (packet_data.packet_type == P_CONNECT) {
        // BOP leader (sender) is still waiting for connection confirmation
        // Or another leader wants to connect (connection establishment race)

        send_packet((Packet) { P_ESTABLISHED });  // previous `P_ESTABLISHED` packet was probably dropped
        return false;
    }

    // Bidirectional connection successfully established and packet not consumed
    return true;
}

Packet decode_packet_data(PacketData packet_data) {
    // Supposes successful P2P-BOP bidirectional pairing established

    // Check packet attributes, ignore unsuitable packets
    if (!valid_packet_data(packet_data)) {
        return NULL_PACKET;
    }

    // Decode valid packet
    Packet packet = { packet_data.packet_type, packet_data.packet_id, packet_data.content };

    if (packet.type == P_ACK) {
        // Check if received packet is an ACK for our pending packet
        if (packet.id == peek(&packet_queue).id) {
            // Wait to receive ACK for pending packet before sending the next one
            dequeue(&packet_queue);  // pending packet has been ACKed
        }
        return NULL_PACKET;  // do not ACK or process ACKs
    }

    // ACK the received (valid && non-ACK) packet
    send_packet((Packet) { P_ACK, packet.id });

    // Make sure not to process a packet twice
    if (packet.id == last_acked_packet_id) {
        // Packet was already ACKed, ACK packet was probably dropped
        return NULL_PACKET;  // don't process a second time
    }
    last_acked_packet_id = packet.id;

    return packet;
}

void send_first_pending_packet() {
    timer_counter = 0;

    if (is_queue_empty(&packet_queue)) {
        return;  // no pending packet to send
    }

    // Only send one packet at a time, continuously send pending packet till ACKed
    send_packet(peek(&packet_queue));
}

// === Wi-Fi packet interface ===

bool is_connected() {
    return bop_state == BOP_BIDIRECTIONALLY_PAIRED;
}

void register_message(Message message) {
    // Supposes P2P-BOP is bidirectionally paired

    enqueue(&packet_queue, (Packet) {
        P_DATA,
        next_packet_id++,
        message,
    });
}

Message receive_message() {
    PacketData packet_data = receive_packet_data();

    if (!p2p_bop(packet_data)) { // not paired or packet consumed by P2P-BOP
        return NULL_MESSAGE;
    }

    Packet packet = decode_packet_data(packet_data);

    if (packet.type == P_NONE) {
        // No incoming packet to process
        if (COUNTER_DONE) {  // continuously send pending non-ACKed packet
            send_first_pending_packet();
        }

        return NULL_MESSAGE;
    }

    return packet.content;
}
