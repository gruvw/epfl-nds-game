#include <stdio.h>
#include "matrix.h"

void printMatrix(int * const matrix, const int rows, const int columns) {
    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < columns; c++) {
            printf("%d ", matrix[r * columns + c]);
        }
        printf("\n\n");
    }
}
