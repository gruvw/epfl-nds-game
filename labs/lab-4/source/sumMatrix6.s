
@------------------------------------------------------------------------------
@-------------------------------------sum6-------------------------------------
@------------------------------------------------------------------------------

.ARM

.ALIGN

.GLOBL  sum6

.TYPE   sum6,function   @ Not always required but useful for .map files
.text
sum6:
	push {r4, r5, lr}	@ Save r4, r5 and lr
	ldr r4, [sp, #12]	@ load 'e' (5th parameter) in r4
	ldr r5, [sp, #16]	@ load 'f' (6th parameter) in r5
	add r0, r0, r1		@ a + b
	add r0, r0, r2		@ a + b + c
	add r0, r0, r3		@ a + b + c + d
	add r0, r0, r4		@ a + b + c + d + e
	add r0, r0, r5		@ a + b + c + d + e + f
	pop {r4, r5, lr}	@ Restore r4, r5 and lr
	bx lr				@ return




@------------------------------------------------------------------------------
@-------------------------------------sumMatrix6--------------------------------
@------------------------------------------------------------------------------

.ALIGN

.GLOBL  sumMatrix6

.TYPE   sumMatrix6,function   @ Not always required but useful for .map files
sumMatrix6:
    stmdb sp!, {r4 - r11, lr} @ instruction to push an entire specified set of
                              @ register onto the stack in one instructionmv r4, r0
    mov r4, r0				  @ r4 = Mat
    mov r5, #0				  @ r5 = index of the element
    mov r6, #0				  @ r6 = i (row counter)
    mov r10, #0 			  @ accmulated result

.LabelForI:					  @ for (i...)
    cmp r6, #6				  @ i < 6 ?
    bge .EndLabelForI

    ldr r0,[r4, r5, lsl #2]   @ r0 = r4[r5] -> r0 = mat1[index]
    add r5, r5, #1			  @ incrementing the index
    ldr r1,[r4, r5, lsl #2]   @ r1 = r4[r5] -> r1 = mat1[index]
    add r5, r5, #1			  @ incrementing the index
    ldr r2,[r4, r5, lsl #2]   @ r2 = r4[r5] -> r2 = mat1[index]
    add r5, r5, #1			  @ incrementing the index
    ldr r3,[r4, r5, lsl #2]   @ r3 = r4[r5] -> r3 = mat1[index]
    add r5, r5, #1			  @ incrementing the index
    ldr r7,[r4, r5, lsl #2]   @ r7 = r4[r5] -> r7 = mat1[index]
    sub sp, sp, #8
    str r7, [sp]			  @ storing the 5th parameter "mat1[index]" to the stack
    add r5, r5, #1			  @ incrementing the index
    ldr r7,[r4, r5, lsl #2]   @ r7 = r4[r5] -> r7 = mat1[index]
    str r7, [sp, #4]			  @ storing the 6th parameter "mat1[index]" to the stack
    add r5, r5, #1			  @ incrementing the index
    bl sum6					  @ Calling Sum6 function
    pop {r7}				  @ Rearranging the stack pointer
    pop {r7}				  @ Rearranging the stack pointer
    add r10, r10, r0		  @ acccumulating the result
    add r6, r6, #1			  @ incrementing i (row counter)

    b .LabelForI

.EndLabelForI:
    mov r0, r10					@ moving the final result to R0
    ldmia sp!, {r4 - r11, lr} 	@ instruction to restore the previous saved registers
    bx lr						@ return
.end

