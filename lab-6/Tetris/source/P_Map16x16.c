#include "P_Map16x16.h"

u16* mapMemory;
int MapCols;
int MapRows;

void P_Map16x16_configureBG2()
{

}

void P_Map16x16_configureBG2_Sub()
{

}

void P_Map16x16_configureBG0()
{

}

void P_Map16x16_configureBG3()
{

}

void P_Map16x16_Init( int cols, int rows )
{
    MapCols=cols;
    MapRows=rows;
}


 void SetMap16x16To(int index16, bool full)
 {
	    //switch x and y
	    int x = index16 % MapCols;
	    int y = index16 / MapCols;
	    //now inverse x
	    y = -(y-MapRows+1);
	    index16 = x * MapRows + y;
#ifdef FB0
	    u16 value = full ? RGB15(31,0,0) : RGB15(0,0,0);
	    FillRectangle(MAIN,x*16, x*16+16, y*16, y*16+16,value);
#endif
}
