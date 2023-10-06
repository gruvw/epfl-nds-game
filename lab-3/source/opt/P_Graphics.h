/*
 * P_Graphics.h
 *
 *  Created on: Sept 10, 2021
 *      Author: admin
 */

#ifndef PGRAPHICS_H_
#define PGRAPHICS_H_

#pragma once

#include <nds.h>
#define FB0

enum BUFFER_TYPE {
    MAIN,
    SUB,
};

// members
extern u16 * P_Graphics_mainBuffer;
extern u16 * P_Graphics_subBuffer;
extern int P_Graphics_mainW;
extern int P_Graphics_mainH;
extern int P_Graphics_subW;
extern int P_Graphics_subH;

/*
        This function does not receive any input parameter. It initializes the main
         graphical engine to be used with the Framebuffer Mode 0 (FB0).
        It also enables the corresponding VRAM bank to be used.
*/
void P_Graphics_setup_main();

void P_Graphics_assignBuffer(enum BUFFER_TYPE bT, u16 * buffer, int w, int h);
#endif
