
@------------------------------------------------------------------------------
@-------------------------------------isqrt------------------------------------
@------------------------------------------------------------------------------

.ARM

.ALIGN

.GLOBL  iSqrt

.TYPE   iSqrt,function   @ Not always required but useful for .map files
.text
iSqrt:

	push {r4 - r6, lr} 	@ instruction to push an entire specified set of
						@ register onto the stack in one instruction
	@ sub sp, sp, #0xC


	mov r4, #1			@ Put 0x1 in r4
	lsl r4, r4, #16		@ The maximum possible value for the sqrt(n) when n
						@ is a 32 bit integer number is below 2^16

	cmp r0, #0			@ Compare input number with 0
	movles r0, #0		@ If input is less or equal to 0 return 0 as result
	ble .negative_end
	mov r5, #0			@ r5 will store the temporary result
	b .loop_end

.loop_init:
	add r5, r5, r4		@ Add r4 to the temporary result
	mul r6, r5, r5		@ Compute the squared value of the temporary result
	cmp r0, r6
	sublt r5, r5, r4	@ If the squared value is bigger than the input, undo
						@ the additino
.loop_end:
	lsrs r4, r4, #1		@ r4 is right sifted (divided by 2)
	bne .loop_init		@ if r4 contains 0 the final result remains in r5



@Additional Exercise (Return the rounded sqrt)
	add r4, r5, #1
	mul r6, r4, r4		@ r6 stores the square of the (candidate result + 1)
	mul r4, r5, r5		@ r4 stores the square of the candidate result
	sub r4, r0, r4		@ absolute difference with the real input
	sub r6, r6, r0		@ absolute difference with the real input
	cmp r4, r6			@Compare differences
	addgt r5, r5, #1	@If the upper diffference is smaller then return
						@ (candidate +1) that is closer to the real sqrt
@End of Additional Exercise

	mov r0, r5			@ copy the result to r0 to be returned

.negative_end:
	@ add sp, sp, #0xC
	pop {r4 - r6, lr} 	@ instruction to restore the previous saved registers
	bx lr 				@ jump to the return address

.end
