
@------------------------------------------------------------------------------
@-------------------------------------FIRfilter--------------------------------
@void FIRfilter(int *x, int	n, int *coef, int m, int *y){
@    int i=0;
@    int j=0;
@   for (i=0;i<m;i++){
@        for (j=0;j<(i+1);j++){
@           y[i]+=x[i-j]*coef[j];
@       }
@		y[i]=y[i]>>12;
@    }
@
@    for (i=m;i<n;i++){
@        for (j=0;j<m;j++){
@            y[i]+=x[i-j]*coef[j];
@        }
@		y[i]=y[i]>>12;
@    }
@}
@------------------------------------------------------------------------------

.ARM

.ALIGN

.GLOBL  FIRfilter

.TYPE   FIRfilter,function      @ Not always required but useful for .map files
.text
FIRfilter:

    stmdb sp!, {r4 - r11, lr}   @ instruction to push an entire specified set of
                                @ register onto the stack in one instruction

    ldr r4, [sp, #36]	        @ load 'y' (5th parameter) in r4
    mov r5, #0 				    @ r5 = i = 0

@-----------------------filtering-first-m-inputs-optional--------------------------------

.FirstLabelForI:			    @ for (i...)
    cmp r5, r3				    @ i < m ?
    bge .EndFirstLabelForI
    mov r6, #0			  	    @ r6  = j = 0
    mov r11, #0				    @ r11 = temporary result
    add r7, r5, #1			    @ r7  = i+1

.FirstLabelForJ:			    @ for (j...)
    cmp r6, r7				    @ j < i+1 ?
    bge .EndFirstLabelForJ
    sub r8, r5, r6              @ r8  = i-j

    push {r6, r8}
    mov r8, r8, lsl #1			@ using short values ((i-j) * 2 bytes)
    ldrsh r9, [r0, r8]    		@ r9  = x[i-j]
    mov r6, r6, lsl #1			@ using short values (j * 2 bytes)
    ldrsh r10, [r2, r6]   		@ r10 = coef[j]
    pop {r6, r8}

    MLA r11, r9, r10, r11       @ r11 =  x[i-j] * Coef[j] + output[i]

    add r6, r6, #1			    @ j++

    b .FirstLabelForJ

.EndFirstLabelForJ:
	mov r11,r11, asr #12		@ shifting the result 12 bits right to obtain
								@ the correct fixed point representation

    mov r5, r5, lsl #1			@ using short values (i * 2 bytes)
    strh r11, [r4, r5]          @ y[i] = r11
    mov r5, r5, lsr #1			@ resetting the i index

    add r5, r5, #1			 	@ i++
    b .FirstLabelForI

.EndFirstLabelForI:

@--------------------------filtering-the-remaining-inputs-------------------------------

.SecondLabelForI:				@ for (i...)
    cmp r5, r1				    @ i < n ?
    bge .EndSecondLabelForI
    mov r6, #0				    @ r6  = j = 0
    mov r11, #0				    @ r11 = temporary result

.SecondLabelForJ:				@ for (j...)
    cmp r6, r3				    @ j < m ?
    bge .EndSecondLabelForJ
    sub r8, r5, r6              @ r8  = i-j

    push {r6, r8}
    mov r8, r8, lsl #1			@ using short values ((i-j) * 2 bytes)
    ldrsh r9, [r0, r8]    		@ r9  = x[i-j]
    mov r6, r6, lsl #1			@ using short values (j * 2 bytes)
    ldrsh r10, [r2, r6]   		@ r10 = coef[j]
    pop {r6, r8}

    MLA r11, r9, r10, r11       @ r11 =  x[i-j] * Coef[j] + output[i]

    add r6, r6, #1			    @ j++

    b .SecondLabelForJ

.EndSecondLabelForJ:
	mov r11,r11, asr #12		@ shifting the result 12 bits right to obtain
								@ the correct fixed point representation

    mov r5, r5, lsl #1			@ using short values (i * 2 bytes)
    strh r11, [r4, r5]          @ y[i] = r11
    mov r5, r5, lsr #1			@ resetting the i index

    add r5, r5, #1			 	@ i++
    b .SecondLabelForI

.EndSecondLabelForI:
    ldmia sp!, {r4 - r11, lr} 	@ instruction to restore the previous saved registers
    bx lr 					  	@ jump to the return address
.end



