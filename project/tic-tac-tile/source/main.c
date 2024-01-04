#include <nds.h>
#include <stdio.h>

#include "controller/game.h"

int main(void) {
    game_setup();

    game_loop();
}
