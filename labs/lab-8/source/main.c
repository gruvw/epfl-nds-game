/*---------------------------------------------------------------------------------

	$Id: main.c,v 1.5 2007/10/23 00:46:29 wntrmute Exp $

	Simple console print demo
	-- dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "Tetris/TetrisDefines.h"
#include "Tetris/TetrisGameplay.h"
#include "P_Audio.h"
#include "P_Controls.h"
#include "P_Graphics.h"
#include "P_Initializer.h"
#include "P_Map16x16.h"

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

    P_InitNDS();

    srand( clock() );
	Gameplay_Load();
    int loops = 0;
	while( true ) {
		handleInput();
		Gameplay_update();
		Gameplay_draw();
		swiWaitForVBlank();
		loops++;
	}

	return 0;
}
