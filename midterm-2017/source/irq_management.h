/////////////////////////////////////////////////////////
////	DO NOT MODIFY THIS FILE
/////////////////////////////////////////////////////////

#ifndef IRQ_MANAGEMENT_H_
#define IRQ_MANAGEMENT_H_
#include <nds.h>
#include <stdio.h>
#include "countdown_timer_display.h"
#include "graphics_sub.h"
#include "clk_hand.h"


extern uint32 min, sec, msec;

/*
 * IRQ_initialize:
 * configures the timer and IRQs of the system
 */
void IRQ_initialize();

/*
 * ISR_countdown_timer:
 * Interrupt Service Routine of the timer.
 * After each 1 ms, it decrements the countdown timer.
 */
void ISR_countdown_timer();

/*
 * ISR_VBlank:
 * Interrupt Service Routine of the screens, to refresh them properly.
 */
void ISR_VBlank();

/*
 * This function handles the input by monitoring the keys and the touchscreen.
 */
void handleInput();

/*
 * This function handles the touchscreen.
 */
void handleTouchPad();


#endif /* IRQ_MANAGEMENT_H_ */
