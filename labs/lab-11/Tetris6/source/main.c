#include <nds.h>
#include "P_Controls.h"
#include "P_Initializer.h"
#include "P_Score.h"
#include <fat.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	
	fatInitDefault();
    P_InitNDS();

    srand( clock() );
	Gameplay_Load();
	readMaxScore();
	displayScore();
	displayMaxScore();

	while( true ) {
		handleInput();
		swiWaitForVBlank();
	}

	return 0;
}
