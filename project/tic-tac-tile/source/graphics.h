#include <stddef.h>
#include "nds/ndstypes.h"
#include "board.h"

void set_backgrounds();
void graphics_setup();
void overlay_sprite(const u16 * sprite, size_t row_pos, size_t col_pos, size_t side);
void draw_board(Board board);
void draw_select(Coords coords);
void clear_game_screen();
