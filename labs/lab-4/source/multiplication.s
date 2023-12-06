@------------------------------------------------------------------------------
@--------------------------------------MultMatrix------------------------------
@------------------------------------------------------------------------------
@int MultMatrix(int *Mat1,int *Mat2, int MatrixLength,int *MatR){
@	int r,c,i;
@	for(r=0; r<MatrixLength; r++)
@	{
@		for(c=0; c<MatrixLength; c++)
@		{
@			MatR[r*MatrixLength+c]=0;
@			for(i = 0; i<MatrixLength; i++)
@				MatR[r*MatrixLength+c]+= mat1[r*MatrixLength+i] * mat2[i*MatrixLength+c];
@		}
@	}
@}
@------------------------------------------------------------------------------

.ARM

.ALIGN

.GLOBL  MultMatrix

.TYPE   MultMatrix,function     @ Not always required but useful for .map files

MultMatrix:

	stmdb sp!, {r4 - r11, lr} @ instruction to push an entire specified set of
							  @ register onto the stack in one instruction


	mov r4, #0 				 @ int r=0
	mov r5, #0				 @ int c=0
	mov r6, #0				 @ int i=0

.LabelForR:					 @ for (r...)
	cmp r4, r2				 @ r < MatrixLength ?
	bge .EndLabelForR
	mov r5, #0				 @ c = 0

.LabelForC:					 @ for (c...)
	cmp r5, r2				 @ c < MatrixLength ?
	bge .EndLabelForC

	mov r6, #0				 @ i = 0
	mov r11, #0				 @ initializing r11 (it will compute  MatR[r*MatrixLength+c])

.LabelForI:					 @ for (i...)
	cmp r6, r2				 @ i < MatrixLength ?
	bge .EndLabelForI

	mul r10, r4, r2
	add r10, r10, r6
	ldr r7, [r0, r10, lsl #2] @ r7 = r0[r10] -> r7 = mat1[r*MatrixLength+i]

	mul r10, r6, r2
	add r10, r10, r5		  @ r10 = i * MatrixLength + c

	ldr r8, [r1, r10, lsl #2] @ r8 = r1[r10] -> r8 = mat2[i*MatrixLength+c]

	mul r10, r7 ,r8           @ r10=r7*r8 = mat1[r*MatrixLength+i] * mat2[i*MatrixLength+c]
	add r11, r11, r10         @ r11 = r11 + r10 (accumulating)

	add r6, r6, #1			  @ i++
	b .LabelForI

.EndLabelForI:

	mul r10, r4, r2
	add r10, r10, r5			@ r10 = r*MatrixLength + c
	str r11, [r3, r10, lsl #2]  @ MatR[r*MatrixLength + c] = r11

	add r5, r5, #1			 	@ c++
	b .LabelForC

.EndLabelForC:

	add r4, r4, #1			 	@ r++
	b .LabelForR

.EndLabelForR:

	ldmia sp!, {r4 - r11, lr} 	@ instruction to restore the previous saved registers
	bx lr 					  	@ jump to the return address
