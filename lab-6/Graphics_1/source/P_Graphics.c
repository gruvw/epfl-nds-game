#include "P_Graphics.h"

u16* P_Graphics_mainBuffer;
u16* P_Graphics_subBuffer;
int P_Graphics_mainW;
int P_Graphics_mainH;
int P_Graphics_subW;
int P_Graphics_subH;

void P_Graphics_setup_main()
{
#ifdef FB0
	//Graphics (Part 1)
	//Set the mode to FB0
	//REG_DISPCNT = ...

	//Set the coresponding VRAM bank
	//VRAM_A_CR = ...

#endif

#ifdef ROTOSCALE
	//Graphics (Part 2)
#endif

#ifdef TILES
	//Graphics (Part 3)
#endif
}

void P_Graphics_assignBuffer(enum BUFFER_TYPE bT, u16* buffer, int w, int h)
{

    switch(bT)
    {
        case MAIN: P_Graphics_mainBuffer = buffer;
            P_Graphics_mainW = w;
            P_Graphics_mainH = h;
            break;
        case SUB: P_Graphics_subBuffer = buffer;
            P_Graphics_subW = w;
            P_Graphics_subH = h;
            break;
    }
}

void FillScreen(enum BUFFER_TYPE t, u16 color)
{
	int i;
	switch(t)
	{
	case MAIN:
		//Fill the whole screen (256*192) with the given color
		for(i = 0; i<P_Graphics_mainH*P_Graphics_mainW; i++)
			P_Graphics_mainBuffer[i] = color;
		break;
	case SUB:

		break;
	default:
		return;
		break;
	}
}

void FillRectangle(enum BUFFER_TYPE bT, int top, int bottom, int left, int right, u16 color)
{

}

void DrawHorizontalLine(enum BUFFER_TYPE bT, int x, int x2, int y, u16 color)
{

}
void DrawVerticalLine(enum BUFFER_TYPE bT, int y, int y2, int x, u16 color)
{

}

void DrawRectangle(enum BUFFER_TYPE bT, int top, int bottom, int left, int right, u16 color)
{

}
