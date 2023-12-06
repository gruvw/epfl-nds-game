/*
 * fractal.h
 *
 *  Created on: Sept 10, 2021
 *      Author: admin
 */

#ifndef FRACTAL_H_
#define FRACTAL_H_

#include "P_Graphics.h"
#include "stdio.h"
#include "stdlib.h"

// Decimal units
#define shift 20
#define radious 4

#define MAXX 255
#define MAXY 191

#define CENTER_X 0.01
#define CENTER_Y 0.01
#define OFF_X 1.75
#define OFF_Y 1.0

#define MAXCOUNT 8000000
#define FRACTAL_LIMIT 800

// Define fp to fixed conversion
// Integers
#define FixedToLong(i) ((i) >> shift)
#define LongToFixed(i) ((i) << shift)

// Fixed operations
#define FixedMult(i, j) ((int32_t) ((int64_t) (i) * (int64_t) (j) >> shift))

// Floats
#define FixedToFloat(i) ((i) / (float) (1 << shift))
#define FloatToFixed(i) ((i) * (float) (1 << shift))

// short FixedMult(short x, short y);
int FixedDiv(int x, int y);

void FixedMultiplicationTest(long x, long y);
void FloatMultiplicationTest(float x, float y);
void FixedFractal();
void FloatFractal();

#endif
