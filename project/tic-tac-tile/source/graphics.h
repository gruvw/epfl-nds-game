#include <stddef.h>
#include "nds/ndstypes.h"
#include "board.h"
#include "game.h"

void set_backgrounds();
void graphics_setup();

void overlay_sprite(const u16 * sprite, size_t row_pos, size_t col_pos, size_t side);
void draw_board(Board board);
void draw_select(Coords coords);
void clear_game_screen();

void show_game_over();
void hide_game_over();
void show_begin();
void hide_begin();

void set_game_mode(GameMode new_mode);
void set_game_speed(GameSpeed speed);
