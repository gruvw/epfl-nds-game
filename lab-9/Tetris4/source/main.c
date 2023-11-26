#include <nds.h>
#include "P_Controls.h"
#include "P_Initializer.h"

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	
    P_InitNDS();

    srand( clock() );
	Gameplay_Load();
	while( true ) {
		handleInput();
		swiWaitForVBlank();
	}

	return 0;
}
