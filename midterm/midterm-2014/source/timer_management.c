#include "timer_management.h"
#include "game.h"

//Number of timer ticks (periods of 100 ms) passed
int timer_ticks;

void initTimer() {
	// Initialize timer_ticks
	timer_ticks = 0;

	// Configure timer to trigger an interrupt every 100 ms
    TIMER_CR(0) = TIMER_ENABLE | TIMER_DIV_64 | TIMER_IRQ_REQ;
    TIMER_DATA(0) = TIMER_FREQ_64(10);

	// Associate the ISR (timerISR) to the interrupt line and enable it
    irqSet(IRQ_TIMER(0), &timerISR);
    irqEnable(IRQ_TIMER(0));
}

void timerISR() {
	// Disable the timer when 1.5 seconds have passed and call the function
	// playerLoses() to finish the game (player did not play on time)
    timer_ticks++;

    if (timer_ticks >= 1.5 * 10) {
        irqDisable(IRQ_TIMER(0));
        playerLoses();
    }
}
