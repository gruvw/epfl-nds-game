
@------------------------------------------------------------------------------
@--------------------------------------Sum-------------------------------------
@------------------------------------------------------------------------------

.ARM

.ALIGN

.GLOBL  Sum

.TYPE   Sum,function   @ Not always required but useful for .map files

Sum:

	stmdb sp!, {r4 - r11, lr} @ instruction to push an entire specified set of
							  @ register onto the stack in one instruction


	add r0, r1 				  @ r0=r0 + r1; r0 is used also for the return value



	ldmia sp!, {r4 - r11, lr} @ instruction to restore the previous saved registers
	bx lr 					  @ jump to the return address

