
#include <nds.h>
#include <stdio.h>
#include <math.h>
#include "chrono_display.h"


//Exercises 1, 2 and 3
int min, sec, msec;

/*
//Exercise 2
void ISR_TIMER0()
{
	msec = (msec + 1)%1000;
	updateChronoDisp_Main(min, sec, msec);
}
//Exercise 2
void ISR_TIMER1()
{
	sec = (sec+1) % 60;
	if(sec == 0) min++;
}
*/


//Exercise 3
/*
void ISR_TIMER0()
{
	msec = (msec + 1)%1000;
	if(msec == 0)
	{
		sec = (sec+1) % 60;
		if(sec == 0)
			min++;
	}
	updateChronoDisp_Main(min, sec, msec);
}
*/


//Exercise 4
/*
void ISR_VBlank()
{
	updateChronoDisp_Main(min, sec, msec);
}
*/


//Exercise 5
/*
int color = 0;
void ISR_TIMER1()
{
	switch(color){
	case 0: changeColorDisp_Main(RED); break;
	case 1: changeColorDisp_Main(GREEN); break;
	case 2: changeColorDisp_Main(BLUE); break;
	case 3: changeColorDisp_Main(YELLOW); break;
	}
	color = (color + 1) % 4;
}
*/


//Exercise 6
/*
void ISR_KEYS()
{
	printf("\nTime = %i:%i.%i\n", min, sec, msec);
}
*/

int main(void) {

	consoleDemoInit();
/*
	//Exercise 1

	u16 start, end;
	int result;

	TIMER0_CR = TIMER_ENABLE | TIMER_DIV_1;
	TIMER_DATA(0) = 0;

	start = TIMER_DATA(0);
	result = floor(sqrt((double)start));
	end = TIMER_DATA(0);
	printf("\nIt took %i cycles\n", end-start);

	start = TIMER_DATA(0);
	result = iSqrt(start);
	end = TIMER_DATA(0);
	printf("\nIt took %i cycles\n", end-start);
*/


	//Exercise 2
/*
	initChronoDisp_Main();
	irqInit();
	min = sec = msec = 0;
	TIMER_DATA(0) = TIMER_FREQ_1024(1000);
	TIMER0_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
	TIMER_DATA(1) = TIMER_FREQ_1024(1);
	TIMER1_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
	irqSet(IRQ_TIMER0, &ISR_TIMER0);
	irqSet(IRQ_TIMER1, &ISR_TIMER1);
	irqEnable(IRQ_TIMER0);
	irqEnable(IRQ_TIMER1);
*/

/*
	//Exercise 3 (Comment exercise 1 and 2)

	initChronoDisp_Main();
	changeColorDisp_Main(RED);
	irqInit();
	min = sec = msec = 0;
	TIMER_DATA(0) = TIMER_FREQ_1024(1000);
	TIMER0_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
	irqSet(IRQ_TIMER0, &ISR_TIMER0);
	irqEnable(IRQ_TIMER0);
*/

/*
	//Exercise 4

	irqSet(IRQ_VBLANK, &ISR_VBlank);
	irqEnable(IRQ_VBLANK);
*/

/*
	//Exercise 5

	TIMER_DATA(1) = (TIMER_FREQ_1024(1));
	TIMER1_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
	irqSet(IRQ_TIMER1, &ISR_TIMER1);
	irqEnable(IRQ_TIMER1);
*/

/*
	//Exercise 6

	REG_KEYCNT = (1<<14)| KEY_A;
	irqSet(IRQ_KEYS, &ISR_KEYS);
	irqEnable(IRQ_KEYS);
*/


	while(1) {
		swiWaitForVBlank();
	}
}
