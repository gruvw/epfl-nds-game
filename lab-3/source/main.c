/*---------------------------------------------------------------------------------

	Basic template code for starting a DS app

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include "matrix.h"

#define SIDE_SIZE 54
#define LENGTH SIDE_SIZE*SIDE_SIZE

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	consoleDemoInit();

	//Exercise 2
/*
	printf("Exercise 2\n");

	//Matrix with 9 components preinitialized
	int myMatrix[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	//Print the matrix
	printMatrix(myMatrix,3,3);
*/

	//Exercise 3
/*
	printf("Exercise 3\n");

	//'Empty' (or not initialized) matrix with 9 components
	int result[9];

	//Initialize it as the identity matrix and print it
	identity(result,3);
	printMatrix(result,3,3);
*/

	//Exercise 4
/*
	printf("Exercise 4\n");

	//Print the summation of the components of the matrix (should be 3)
	printf("The sum is %i \n", sumMatrix(result,3,3));
*/

	//Exercise 5
/*
	printf("Exercise 5\n");

	//Vector declaration (10 components)
	int vector[] = {10,11,12,3,5,7,-5,4,-3,6};

	//TO COMPLETE
	// Write the code to sort the vector (Note that the vector
	// has 10 elements)
	....
	....
	....

	//Print the sorted Vector
	printMatrix(vector,1,10);
*/


	//Exercise 6
/*
	printf("Exercise 6\n");

	//Multiply my matrix (declared in exercise 2) by itself and put the
	//result in the matrix declared in exercise 3
	multMatrices(myMatrix,myMatrix,3,3,result);

	//Print the result and the summation
	printMatrix(result, 3, 3);
	printf("The sum is %i \n", sumMatrix(result,3,3));
*/

	//Exercise 7
/*
 	 //Declare 3 matrices with a 54 rows and 54 columns
 	int result2[LENGTH], mat1[LENGTH], mat2[LENGTH];

 	//Initialize mat1 and mat2 as the identity
 	identity(mat2,SIDE_SIZE);
	identity(mat1,SIDE_SIZE);

	//Multiply the two matrices and print the result and the corresponding summation
	multMatrices(mat1,mat2,SIDE_SIZE,SIDE_SIZE,result2);
	printMatrix(result2,SIDE_SIZE,SIDE_SIZE);
	printf("The sum is %i \n", sumMatrix(result2,SIDE_SIZE,SIDE_SIZE));
*/

	//Exercise 8
/*
	//Allocate space in dynamic memory for 3 54x54 matrices
	int *m1 = malloc(sizeof(int)*LENGTH);
	int *m2 = malloc(sizeof(int)*LENGTH);
	int *result3 = malloc(sizeof(int)*LENGTH);

	//Initialize 2 of them as the identity matrix
	identity(m1,SIDE_SIZE);
	identity(m2,SIDE_SIZE);

	//Multiply the two matrices and print the result and the corresponding summation
	multMatrices(m1,m2,SIDE_SIZE,SIDE_SIZE,result3);
	printMatrix(result3,SIDE_SIZE,SIDE_SIZE);
	printf("The sum is %i \n", sumMatrix(result3,SIDE_SIZE,SIDE_SIZE));

	//Release the memory
	free(m1);
	free(m2);
	free(result3);
*/

	//Exercise 9
/*
	printf("Exercise 9\n");
	// TO COMPLETE:
	// Allocate memory for two matrices and initialize them as follows
	// m3 = [ 	1 2			2 Columns x 3 rows
	// 			3 4
	// 			5 6 ]
	//
	// m4 = [	1 2 3		3 Columns x 2 rows
	// 			4 5 6 ]
	//
	// Pay attention in allocating memory for the structures and for the
	// corresponding arrays
	//

	tMatrix *m3 = ...
	m3->cols = ...
	m3->rows = ...
	m3->mat = ...
	tMatrix *m4 = ...
	...

	//Initialization
	for(i=0;i<6;i++)
		m3->mat[i] = m4->mat[i] = i+1;

	//Multiplication of the 2 matrices
	tMatrix *result4 = multMatrices2(m3,m4);
	printMatrix(result4->mat,result4->rows,result4->cols);
	printf("The sum is %i \n", sumMatrix(result4->mat,result4->rows,result4->cols));

	// TO COMPLETE: Release properly the memory corresponding only to the
	// matrices m3 and m4. Do not forget the memory of the arrays
	//
	free(...);
	free(...);
	free(...);
	free(...);
*/

	//Exercise 10
/*
	printf("Exercise 10\n");

	int diagonal_value;

	 // TO COMPLETE: After implementing sumDiagonal(...) call this procedure using
	 // result4 as input parameter and diagonal_value as output parameter
	 // sumDiagonal(....)
	 //
	 // sumDiagonal(.., ...);

	printf("The sum of the diagonal is %i\n",diagonal_value);

	// TO COMPLETE: Release properly the memory corresponding to the mmatrix
	// result4. Do not forget the memory of the arrays
	//
	free...
*/

	while(1) {
		swiWaitForVBlank();
	}
}
