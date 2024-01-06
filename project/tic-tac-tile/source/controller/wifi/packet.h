#include <nds.h>

// Random GAME_ID (should be unique to every NDS game)
#define GAME_ID ((char) 0b1110110)

// Number of chars in a packet (6 + number of chars in a Message)
#define PACKET_SIZE (6 + 3)

// === Wi-Fi Types ===

#ifndef INCLUDE_PACKET

#define INCLUDE_PACKET

typedef enum {
    M_NONE,
    M_START,
    M_PLAY,
    M_TIME,
    M_STOP,
} MessageType;

typedef struct {
    MessageType type;
    u8 arg;
} Message;

typedef u8 PacketID;

typedef enum {
    P_NONE,

    // P2P-BOP
    P_DISCOVERY,
    P_CONNECT,
    P_ESTABLISHED,

    // Application
    P_DATA,
    P_ACK,
} PacketType;

typedef struct {
    PacketType type;
    PacketID id;
    Message content;
} Packet;  // valid packet

#endif

extern u8 timer_counter;

// Utilities
void packet_connection_reset();
bool packet_is_connected();
bool is_connection_leader();

// Sending side
void register_message(Message message);

// Receiving side
Message receive_message();
