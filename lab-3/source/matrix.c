#include "matrix.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void printMatrix(int * const matrix, const int rows, const int columns) {
    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < columns; c++) {
            printf("%d ", matrix[r * columns + c]);
        }
        printf("\n\n");
    }
}

void identity(int * const mat, const int columns) {
    for (size_t c = 0; c < (size_t) columns * columns; c++) {
        mat[c] = c / columns == c % columns;
    }
}

int sumMatrix(int * const mat, const int rows, const int columns) {
    int sum = 0;

    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < columns; c++) {
            sum += mat[r * columns + c];
        }
    }

    return sum;
}

void multMatrices(
    int * const mat1,
    int * const mat2,
    const int rows,
    const int columns,
    int * const result
) {
    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < columns; c++) {
            int sum = 0;
            for (size_t k = 0; k < columns; k++) {
                sum += mat1[r * rows + k] * mat2[k * columns + c];
            }
            result[r * columns + c] = sum;
        }
    }
}

tMatrix * multMatrices2(tMatrix * m1, tMatrix * m2) {
    tMatrix * res = malloc(sizeof(*res));
    res->rows = m1->rows;
    res->cols = m2->cols;
    res->mat = malloc((size_t) (res->rows * res->cols) * sizeof(*res->mat));

    for (size_t r = 0; r < res->rows; r++) {
        for (size_t c = 0; c < res->cols; c++) {
            int sum = 0;
            for (size_t k = 0; k < m1->cols; k++) {
                sum += m1->mat[r * m1->cols + k] * m2->mat[k * m2->cols + c];
            }
            res->mat[r * res->cols + c] = sum;
        }
    }

    return res;
}

void sumDiagonal(const tMatrix * m1, int * sum) {
    *sum = 0;
    for (size_t c = 0; c < m1->cols; c++) {
        *sum += m1->mat[c * m1->cols + c];
    }
}
