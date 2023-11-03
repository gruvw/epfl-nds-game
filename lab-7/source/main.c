#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "P_Audio.h"
#include "P_Controls.h"
#include "P_Graphics.h"
#include "P_Initializer.h"
#include "P_Map16x16.h"
#include "Tetris/TetrisDefines.h"
#include "Tetris/TetrisGameplay.h"

int main(void) {

    P_InitNDS();

    srand(clock());
    Gameplay_Load();
    int loops = 0;
    while (true) {
        handleInput();
        Gameplay_update();
        Gameplay_draw();
        swiWaitForVBlank();
        loops++;
    }

    return 0;
}
