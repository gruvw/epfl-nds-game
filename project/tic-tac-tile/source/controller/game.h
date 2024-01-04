#include "nds/ndstypes.h"

#include "../model/board.h"

#ifndef INCLUDE_GAME

#define INCLUDE_GAME
#define STARTING_TIME 24

typedef enum {
    SINGLE_PLAYER,
    TWO_PLAYER_LOCAL,
    TWO_PLAYER_WIFI,
} GameMode;

typedef enum {
    SLOW,
    MEDIUM,
    FAST,
} GameSpeed;

typedef enum {
    G_BEGIN,
    G_RUNNING,
    G_FINISHED,
} GameState;

typedef enum {
  T_UNUSED,
  T_STARTED,
  T_OVER,
} TimerState;

#endif

extern GameMode game_mode;
extern GameSpeed game_speed;

extern Board board;
extern Coords selection_coords;

extern GameState game_state;
extern GameState next_game_state;

extern TimerState timer_state;
extern u8 time_left;

extern Cell local_side;
extern Cell active_side;

void refresh_game_screen();

void reset_game();

void game_setup();
void game_loop();
