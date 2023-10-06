/*
 * fractal.c
 *
 *  Created on: Sept 10, 2021
 *      Author: admin
 */

#include "fractal.h"

#include "P_Graphics.h"

/*
 * Fixed point division
 */
int FixedDiv(int x, int y) {
    return ((x) / (y)) << shift;
}

/*
 * Fixed point multiplication test
 * Iterate over a MAXCOUNT maximum to show performance difference between
 * FP and Fixed Point numbers
 */
void FixedMultiplicationTest(long x, long y) {
    int counter = 0;
    int temp = 0;
    printf("\nFixed");
    while (counter < MAXCOUNT) {
        temp = FixedMult(x, y);
        counter++;
    }
    printf("%d", temp);
}

/*
 * Floating point multiplication test
 */
void FloatMultiplicationTest(float x, float y) {
    int counter = 0;
    float temp = 0;
    printf("\nFloat");
    while (counter < MAXCOUNT) {
        temp = x * y;

        counter++;
    }
    printf("%f", temp);
}

/*
 * Fixed point fractal calculation
 * To adapt the fractal to the NDS screen, keep ci and cj scale to
 * i: x*0.01 - 1.75
 * j: y*0.01 - 1.0
 * Also keep the bias to center the figure in the screen
 *
 * This function calculates the convergence of the Mandelbrot set using fixed point multplication
 * and prints in black and white colors
 */
void FixedFractal() {
    // Main variable definition. Here you can control the variable size to test your experiments
    int32_t ci, cj, zi, zj, t_zi, fxycenter, fxxcenter, xoff, yoff, fxradious, fxptwo;

    // Auxiliary variables for iterating
    int count, x, y;

    // Color definition for printing the screen
    u16 black = RGB15(0, 0, 0);
    u16 white = RGB15(31, 31, 31);

    // Screen index. Required to prevent over operations to be performed in runtime
    int g_index = 0;

    // Counters to compare the convergents with non-convergent points
    int over, no = 0;

    // Fixed point initializations and definitions for the offsets
    fxycenter = FloatToFixed(CENTER_Y);
    fxxcenter = FloatToFixed(CENTER_X);
    xoff = FloatToFixed(OFF_X);
    yoff = FloatToFixed(OFF_Y);
    fxradious = FloatToFixed(radious);
    fxptwo = FloatToFixed(2.0);

    // Iterate over the screen pixels (192*256 pixels
    for (y = 0; y <= MAXY; y++) {
        for (x = 0; x <= MAXX; x++) {
            // Define fractal offset
            // ci = X axis offset ==  x*xcenter - xoffset
            // cj = Y axis offset == y*ycenter - yoffset

            zi = 0;
            zj = 0;
            count = 0;

            // Calculate convergence of the fractal

            /*
             * The array that represents the screen pixels is linear. Thus, X,Y coordinates must be
            represented as a single one (y*(MAXX+1))+x
             * if Converges choose one color. Instead of calling any function, is more efficient to
            assign the color to the corresponding pixel in every iteration.
            {
                    P_Graphics_mainBuffer[g_index] = white;
                    over++;
            }
            else choose another one
            {
                    P_Graphics_mainBuffer[g_index] = black;
                    no++;
            }

            g_index++;
            */
        }
    }
    printf("\nConverged: %d ---- Overflow: %d \n", over, no);
}

/*
 * This function calculates the convergence of the Mandelbrot set using floating point multplication
 * and prints in black and white colors
 */
void FloatFractal() {
    float ci, cj, zi, zj, t_zi;
    int count, x, y;
    int g_index = 0;
    u16 black = RGB15(0, 0, 0);
    u16 white = RGB15(31, 31, 31);

    for (y = 0; y <= MAXY; y++) {
        for (x = 0; x <= MAXX; x++) {
            ci = ((x) *0.01 - 1.75);
            cj = ((y) *0.01 - 1.0);

            zi = (0.0);
            zj = (0.0);
            count = 0;

            // Calculate convergence of the fractal

            /*
             * The array that represents the screen pixels is linear. Thus, X,Y coordinates must be
            represented as a single one (y*(MAXX+1))+x if Converges choose one color
            {
                    P_Graphics_mainBuffer[g_index] = white;
                    over++;
            }
            else choose another one
            {
                    P_Graphics_mainBuffer[g_index] = black;
                    no++;
            }

            g_index++;
            */
        }
    }
}
