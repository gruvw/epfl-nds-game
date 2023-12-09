#include <stddef.h>
#include "nds/ndstypes.h"

void graphics_setup();
void set_background();
void overlay_sprite(const u16 * sprite, size_t row_pos, size_t col_pos, size_t side);

void main_graphics();
