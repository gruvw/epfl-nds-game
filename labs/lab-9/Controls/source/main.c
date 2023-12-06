#include <nds.h>
#include <stdio.h>


//Keys ISR
void keysISR()
{
	//Read the kEYINPUT register
	u16 keys = ~(REG_KEYINPUT);

	//Identify which key triggered the interrupt and print it in the console
	if(keys & KEY_UP)
		printf("Key UP pressed (ISR)\n");
	if(keys & KEY_DOWN)
		printf("Key DOWN pressed (ISR)\n");
	if(keys & KEY_RIGHT)
		printf("Key RIGHT pressed (ISR)\n");
	if(keys & KEY_LEFT)
		printf("Key LEFT pressed (ISR)\n");
}

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	//Initialize the console
	consoleDemoInit();
	scanKeys();

	/*************
	 * Exercise 2
	 *************/
	//Initialize the interrupt system
//	irqInit();
	/*Configure the keys to throw an interrupt with the keys
	UP, DOWN, LEFT or RIGHT*/
//	REG_KEYCNT = KEY_LEFT | KEY_RIGHT | KEY_DOWN | KEY_UP | 1<<14;
	//Set the ISR to the IRQ line and enable the IRQ line
//	irqSet(IRQ_KEYS, &keysISR);
//	irqEnable(IRQ_KEYS);
	//Do not forget to enable the IRQ line for the VBLANK
//	irqEnable(IRQ_VBLANK);

	/********************/


	//The main infinite loop
	while(1) {
		/*************
		 * Exercise 1
		 *************/
		//Scan the keys
		scanKeys();
		//Identify which key was pressed and print it in the console
		u16 keys = keysDown();
		if(keys & KEY_A)
			printf("Key A pressed \n");
		if(keys & KEY_B)
			printf("Key B pressed \n");
		if(keys & KEY_X)
			printf("Key X pressed \n");
		if(keys & KEY_Y)
			printf("Key Y pressed \n");

		/*******************/


		/*************
		 * Exercise 3 (Do not forget to comment exercise 2)
		 *************/
		//Declare a touchPosition variable
		touchPosition touch;
		//Read the touchscreen
		touchRead(&touch);
		//Identify a valid touched coordinates and print them
		if(touch.px | touch.py)
			printf("Touch in (%i,%i)\n", touch.px, touch.py);

		/**************/

		swiWaitForVBlank();
	}
}
