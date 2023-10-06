/*
 * P_Graphics.c
 *
 *  Created on: Sept 10, 2021
 *      Author: admin
 */

#include "P_Graphics.h"

u16 * P_Graphics_mainBuffer;
u16 * P_Graphics_subBuffer;
int P_Graphics_mainW;
int P_Graphics_mainH;
int P_Graphics_subW;
int P_Graphics_subH;

void P_Graphics_setup_main() {
#ifdef FB0
    // Graphics (Part 1)
    // Set the mode to FB0
    REG_DISPCNT = MODE_FB0;

    // Set the corresponding VRAM bank
    VRAM_A_CR = VRAM_ENABLE    // Enable the Bank
                | VRAM_A_LCD;  // Assign it to the Main engine in FB0

#endif
}

void P_Graphics_assignBuffer(enum BUFFER_TYPE bT, u16 * buffer, int w, int h) {
    switch (bT) {
        case MAIN:
            P_Graphics_mainBuffer = buffer;
            P_Graphics_mainW = w;
            P_Graphics_mainH = h;
            break;
        case SUB:
            P_Graphics_subBuffer = buffer;
            P_Graphics_subW = w;
            P_Graphics_subH = h;
            break;
    }
}
