/*
 * Template Nintendo DS
 * Sept 2021
 */

#include <nds.h>
#include <stdio.h>
#include <time.h>

#include "P_Graphics.h"
#include "fractal.h"

int main(void) {
    consoleDemoInit();

    // Initialize main screen
    P_Graphics_setup_main();
    P_Graphics_assignBuffer(MAIN, (u16 *) VRAM_A, 256, 192);

    // Exercise 1 Test FxP conversions and operations

    // Exercise 1.5 FxP multiplication

    // Exercise 1.6 16b variables

    // Exercise 2
    double start_time, end_time, exec_time;

    start_time = time(0);
    FixedFractal();
    end_time = time(0);
    exec_time = (end_time - start_time);
    printf("\n Fixed Fractal calculation time (s): %f seconds", exec_time);

    start_time = time(0);
    FixedFractal();
    end_time = time(0);
    exec_time = (end_time - start_time);
    printf("\n Fixed Fractal calculation time (s): %f seconds", exec_time);
    while (1)
        swiWaitForVBlank();
}
