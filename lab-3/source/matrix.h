/*
 * matrix.h
 *
 *  Created on: Feb 21, 2011
 *      Author: admin
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include "stdio.h"
#include "stdlib.h"

/*
 * The following type is a simple structure to represent a matrix.
 * The fields of the structure are:
 * 		*mat: Pointer to the array of integers
 * 		rows: Number of rows in the matrix
 * 		cols: Number of columns in the matrix
 */
typedef struct{
	int *mat;
	int rows;
	int cols;
}tMatrix;

/*
 * Print the matrix that receives as parameter in a proper way.
 * It receives three parameteres:
 * 		mat[]: an array of integer that represents the matrix
 * 		rows, columns: number of rows and columns of the matrix
 */
void printMatrix(int mat[], int rows, int columns);

/*
 * Function that initializes a square matrix as an identity matrix
 * (matrix with a diagonal of '1' and the rest component set to 0).
 * This function receives two parameters:
 * 		mat[]: An array of integer that represents the matrix
 * 		columns: number of columns of the matrix (equals to the number
 * 		of rows)
 */
void identity(int mat[], int columns);

/*
 * This function return an integer that is equal to the summation
 * of all the components of the matrix
 */
int sumMatrix(int mat[], int rows, int columns);

/*
 * This function multiplies two square matrices (same number of
 * rows and columns). The result is stored in the 5th parameter
 * which is an array of integer representing the resulting matrix
 */
void multMatrices(int mat1[], int mat2[], int rows, int columns,int result[]);

/*
 * This function multiply two matrices represented by the data structure
 * tMatrix. This function receives two pointers to the matrices to be multiplied
 * and returns a pointer to a structure that stores the result.
 *
 * ATTENTION: This function must allocate memory for the structure properly.
 * This memory has to be released by the programmer in a proper way.
 */

tMatrix* multMatrices2(tMatrix* m1, tMatrix* m2);

/*
 * Declare a procedure that takes 3 parameters:
 * 		input: a tMatrix structure
 * 		output: the value of the summation of all the diagonal components
 * 			of the matrix
 */

//void sumDiagonal(..,.. );


#endif /* MATRIX_H_ */
