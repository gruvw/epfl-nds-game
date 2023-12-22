#include "nds/ndstypes.h"

#ifndef INCLUDE_GAME

#define INCLUDE_GAME
#define STARTING_TIME 24

typedef enum {
    SINGLE_PLAYER,
    TWO_PLAYER_LOCAL,
    TWO_PLAYER_WIFI, // TODO
} GameMode;

typedef enum {
    SLOW,
    MEDIUM,
    FAST,
} GameSpeed;

typedef enum {
    BEGIN,
    RUNNING,
    FINISHED,
} GameState;

typedef enum {
  UNUSED,
  STARTED,
  OVER,
} TimerState;

#endif

extern GameMode game_mode;
extern GameSpeed game_speed;

extern TimerState timer_state;
extern u8 time_left;

void game_setup();
void game_loop();
