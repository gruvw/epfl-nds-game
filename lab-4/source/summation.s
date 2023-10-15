@------------------------------------------------------------------------------
@--------------------------------------SumArray--------------------------------
@------------------------------------------------------------------------------

.ARM

.ALIGN

.GLOBL  SumArray

.TYPE   SumArray,function     @ Not always required but useful for .map files

SumArray:

	stmdb sp!, {r4 - r11, lr} @ instruction to push an entire specified set of
							  @ register onto the stack in one instruction

	mov r2, #0 				 @ int i=0
	mov r3, #0				 @ int tempoary register to store the summation

.LabelFor:
	cmp r2, r1				 @ i<length ?
	bge .EndLabelFor
	ldr r4, [r0, r2, lsl #2] @ r4 = r0[r2] -> r4 = Array[i]
	add r3, r3, r4			 @ r3 = r3 + Array[i]
	add r2, r2, #1			 @ i++

	b .LabelFor

.EndLabelFor:
	mov r0, r3				 @ storing the result in r0

	ldmia sp!, {r4 - r11, lr} @ instruction to restore the previous saved registers
	bx lr 					  @ jump to the return address
