#ifndef INCLUDE_GAME
#define INCLUDE_GAME
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
#endif

extern GameMode game_mode;
extern GameSpeed game_speed;

void game_setup();
void game_loop();
