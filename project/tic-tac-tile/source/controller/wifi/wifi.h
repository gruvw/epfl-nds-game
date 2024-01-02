#include "nds/ndstypes.h"

#include "../../model/board.h"

#ifndef INCLUDE_WIFI

#define INCLUDE_WIFI

typedef enum {
    W_WAIT,
    W_CONNECTED,
    W_PLAY,
    W_TERMINATED,
} WifiState;

#endif

extern WifiState wifi_state;
extern WifiState next_wifi_state;

void wifi_reset();

bool wifi_setup();
void wifi_fsm();

void send_stop();
void send_move(Coords coords);
