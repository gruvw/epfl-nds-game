#include <nds.h>
#include <stddef.h>

#include "../model/board.h"
#include "../controller/game.h"

void set_backgrounds();
void graphics_setup();

void patch_palette(void * data, size_t len, int increment, bool background);

void draw_board(Board board);
void draw_select(Coords coords);
void clear_game_screen();

void show_game_over();
void hide_game_over();
void show_begin();
void hide_begin();
