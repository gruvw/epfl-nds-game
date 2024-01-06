#include "../controller/network.h"
#include "../model/board.h"

void sprites_setup();

void show_game_over_sprites(Cell c, bool time);
void hide_game_over_sprites();

void hide_wifi_sprites();
void show_connection_sprite(WifiState wifi_state);
