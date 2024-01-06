#ifndef INCLUDE_NETWORK

#define INCLUDE_NETWORK

typedef enum {
    WS_NO_WIFI,  // not connected to AP
    WS_WIFI,  // connected to AP but not paired to other NDS
    WS_PAIRED,  // paired to other NDS
} WifiState;

#endif

extern WifiState wifi_state;

void wifi_reset(bool display);
bool wifi_setup();

void wifi_process();
