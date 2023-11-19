/////////////////////////////////////////////////////////
////	DO NOT MODIFY THIS FILE
/////////////////////////////////////////////////////////
#ifndef TIMER_MANAGEMENT_H_
#define TIMER_MANAGEMENT_H_
#include <nds.h>

/*
 * initTimer:
 * configures a timer in order to trigger an interrupt every 100 ms
 * and associates the interrupt service routine timerISR() to that
 * line. It also activates the timer
 */
void initTimer();

/*
 * timerISR:
 * Interrupt Service Routine of the timer. When 1.5 seconds have passed,
 * it disables the timer and calls the function plyerLoses()
 */
void timerISR();

#endif /* TIMER_MANAGEMENT_H_ */
