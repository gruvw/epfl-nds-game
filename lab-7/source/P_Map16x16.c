
#include "P_Map16x16.h"
#include "controls.h"

u16* mapMemory;
int MapCols;
int MapRows;

void P_Map16x16_configureBG2()
{
    //Initialize Background
    BGCTRL[2] = BG_MAP_BASE(0) | BgSize_B16_256x256;

    //Affine Marix Transformation
    REG_BG2PA = 256;
    REG_BG2PC = 0;
    REG_BG2PB = 0;
    REG_BG2PD = 256;

    //Initialize pointer to the graphic memory
    //mapMemory = ...
    mapMemory = BG_GFX;
}

void P_Map16x16_configureBG2_Sub()
{
	//Configure BG 2 Sub. (Do not forget the BG_BMP_BASE configuration)
    BGCTRL_SUB[2] = BG_BMP_BASE(0) | BgSize_B8_256x256;

    //Affine Marix Transformation
    REG_BG2PA_SUB = 256;
    REG_BG2PC_SUB = 0;
    REG_BG2PB_SUB = 0;
    REG_BG2PD_SUB = 256;

    swiCopy(controlsPal, BG_PALETTE_SUB, controlsPalLen/2);
    swiCopy(controlsBitmap, BG_GFX_SUB, controlsBitmapLen/2);
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
#ifdef ROTOSCALE
    //Configure BG2
    P_Map16x16_configureBG2();
    // Configure Bottom background
    P_Map16x16_configureBG2_Sub();
#endif
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

#ifdef ROTOSCALE
     
     if (full==1)
         FillRectangle(MAIN,x*16, x*16+16, y*16, y*16+16,ARGB16(1,31,0,0));
     else
         FillRectangle(MAIN,x*16, x*16+16, y*16, y*16+16,ARGB16(1,0,0,0));
#endif

}

