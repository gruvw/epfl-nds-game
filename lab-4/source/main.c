/*---------------------------------------------------------------------------------

	Basic template code for starting a DS app

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include <math.h>

extern int Sum(int a, int b);
extern int SumArray(int *array, int length);
extern int MultMatrix(int *Mat1,int *Mat2, int MatrixLength,int *MatR);
extern int iSqrt(int n);
extern int sum6(int a, int b, int c, int d, int e, int f);
extern int sumMatrix6(int *Mat);
extern void FIRfilter(short *x, int n, short *coef, int m, short *y);

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	consoleDemoInit();

	printf("\n inline assembly example \n");

	int a=6;
	int b=4;
	int c=0;
	asm volatile("add %0, %1, %2" :
				  "=r" (c) :
				 "r" (a),"r" (b)
				 );

	printf("\n %d + %d = %d \n", a, b, c);

	//Exercise 1

	printf("\n Exercise 1 \n");
	a=(-25);
	b=5;
	c=2;
	int d=0;

	asm volatile("mul r4, %1, %2\n\t"
				"add %0, r4, %3\n\t"
				"mov r4, #0\n\t"
				"cmp %0, r4 \n\t"
				"bge .else_label\n\t"
				"sub %0, r4, %0\n\t"
				".else_label:\n\t"
				: "=r" (d)
				: "r" (b), "r" (c), "r" (a)
				: "r4");
	printf("\n abs( %d + %d * %d ) = %d \n", a, b, c, d);


	//Exercise with steps

	printf("\n Exercise with steps \n");
	a=60;
	b=40;
	c= Sum(a,b);
	printf("\n %d + %d = %d \n", a, b, c);


	//Exercise 2

	printf("\n Exercise 2\n");
	int length=10;
	int array1[10]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	c= SumArray(array1,length);
	printf("\n SumArry = %d \n", c);


	//exercise 3

	printf("\n Exercise 3 \n");
	int Mat1[9]={1,2,3,4,5,6,7,8,9};
	int MatR[9];
	MultMatrix(Mat1 , Mat1, 3, MatR);
	printf("\n MatR = \n\n"
			"\t%d\t%d\t%d\n\n"
			"\t%d\t%d\t%d\n\n"
			"\t%d\t%d\t%d\n\n",
			MatR[0], MatR[1], MatR[2], MatR[3], MatR[4], MatR[5], MatR[6], MatR[7], MatR[8]);


	//exercise 4

	printf("\n Exercise 4 \n");
	a = sum6(1,2,3,4,5,6);
	printf("\n 1 + 2 + 3 + 4 + 5 + 6 = %i\n", a);

	//exercise 5

	printf("\n Exercise 5 \n");
	int Mat6[36];
	int i;
	for(i=1;i<37;i++) Mat6[i-1]=i;
	a = sumMatrix6(Mat6);
	printf("\n sumMatrix6 = %d \n", a);


	//Exercise 6

	printf("\n Exercise 6 \n");
	a = 0;
	printf("\nCalculating 500000 normal Sqrt\n\n");
	for(i=0;i<500000;i++)
	{
		if(i % 25000 == 0) printf(">");
		a += floor(sqrt((double)i));
	}
	printf("\n\nFinished!  Result = %i\n", a);
	a=0;
	printf("\nCalculating 500000 handwritten Sqrt\n\n");
	for(i=0;i<500000;i++)
	{
		if(i % 25000 == 0) printf(">");
		a += iSqrt(i);
	}
	printf("\n\nFinished!  Result = %i\n", a);

	//Exercise 7

	printf("\n Exercise 7 \n");
	a = 3;
	b = 2;
	c = -3;
	asm volatile("mlas %0, %1, %2, %3\n\t"
				"mov r1, #0\n\t"
				"sublt %0, r1, %0\n\t"
				: "=r" (d)
				: "r" (b), "r" (c), "r" (a)
				);
	printf("\n abs( %d + %d * %d ) = %d \n", a, b, c, d);

	//Exercise 9
	// The fixed point representation of the filter coefficients
	// and of the input was chosen as follow:
	// 3 bits for the integer part, 12 for the decimal and one for the sign.
	// The assembler code that will be implemented has to consider this fixed
	// point representation in order to have the correct output

	printf("\n Exercise 9 \n\n");
    short input[50]={ 1840, 2701, 3085, 3296, 119, 3194, 2323,
                     311, 1030, 546, 2312, 2215, 282, 4048,
                    1028, 1292, 1231, 172, 2162, 1048, 1674,
                    3881, 3765, 496, 2424, 1473, 2946, 2144,
                    1068, 2019, 3505, 2967, 815, 644, 1517,
                    3531, 2804, 2597, 578, 324, 3588, 1722,
                    1997, 1885, 2112, 1114, 948, 3684, 3722,
                    2472};
    short output[50];
    short coef[11]={ -235, -246, -272, -310, -343, 3738, -343,-310, -272, -246, -235};

    FIRfilter(input, 50, coef, 11, output);

    for (i=0;i<10;i++){
        printf("%5d %5d %5d %5d %5d\n", output[5*i], output[5*i+1], output[5*i+2], output[5*i+3], output[5*i+4] );
    }

	while(1) {
		swiWaitForVBlank();
	}
}
