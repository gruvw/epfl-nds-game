
#include "irq_management.h"

#include "graphics_sub.h"
#include "nds/arm9/input.h"
#include "nds/input.h"
#include "nds/interrupts.h"

// ------ Exercise 2 ------ //

/*
 * IRQ_initialize:
 * configures the timer and IRQs of the system
 */
void IRQ_initialize() {
    // Configure timer 0 to trigger an interrupt every 1 ms
    TIMER_DATA(0) = TIMER_FREQ(1000);
    TIMER_CR(0) = TIMER_ENABLE | TIMER_DIV_1 | TIMER_IRQ_REQ;

    // Countdown timer value decrementing:
    // Associate the ISR (ISR_countdown_timer) to the interrupt line of timer 0
    // (Do not enable it!)
    irqSet(IRQ_TIMER(0), &ISR_countdown_timer);

    // Main screen refreshing:
    // Associate the ISR (ISR_VBlank) to the interrupt line VBLANK and enable it
    irqSet(IRQ_VBLANK, &ISR_VBlank);
    irqEnable(IRQ_VBLANK);
}

// ------ Exercise 4 ------ //

// Initial value of the countdown timer (in sec)
// (set by the user before starting the countdown)
uint32 sec_init = 0;  // Reset

// Current value of the countdown timer (in mini-sec, sec and min)
uint32 min = 0, sec = 0, msec = 0;  // Reset

// Countdown timer states list
enum { STOP, PLAY, PAUSE };

// Countdown timer state
u8 countdown_state = STOP;  // Initial state

/*
 * This function handles the input by monitoring the keys and the touchscreen.
 */
void handleInput() {
    // Scan the keys
    scanKeys();

    /* Identify the keys and map the action according to the instructions
	 * presented in the exam sheet. */
    uint32 keys = keysDown();

    // If the arrows have been touched and countdown_state is STOP
    if (countdown_state == STOP && ((keys & KEY_UP) || (keys & KEY_DOWN))) {
        // Save the initial value of the countdown timer in seconds
        if ((keys & KEY_UP) && sec_init <= 59 * 60)
            sec_init += 30;
        if ((keys & KEY_DOWN) && sec_init >= 30)
            sec_init -= 30;
        min = sec_init / 60;
        sec = sec_init % 60;
    } else if (keys & KEY_TOUCH)  // The touchscreen has been touched
    {
        handleTouchPad();
    }
}

// ------ Exercise 5 ------ //

/*
 * This function handles the touchscreen.
 */
void handleTouchPad() {
    // Read the touch position (structure).

    /* Get the coordinates of the touched position:
	   ▪ If STOP (square) button/region touched:
	     - Disable the countdown timer IRQ (IRQ_TIMER0).
	     - Reset timer value to zero.
	     - Change the state of "countdown_state".
	     - Set background color to WHITE and the digits color to BLACK on the MAIN screen,
	       with the function: changeMainColorDisp(uint16 bg_color, uint16 digit_color).
	   ▪ If PLAY (triangle) button/region touched:
	     - Start countdown timer by enabling its IRQ (IRQ_TIMER0).
	     - Change the state of "countdown_state".
	     - Set background color to YELLOW and the digits color to BLACK on the MAIN screen,
	       with the function: changeMainColorDisp(uint16 bg_color, uint16 digit_color).
	   ▪ If PAUSE (double bars) button/region touched:
	     - Pause countdown timer by disabling its IRQ (IRQ_TIMER0).
	     - Change the state of "countdown_state".
	     - Set background color to GREEN and the digits color to BLACK on the MAIN screen,
	       with the function: changeMainColorDisp(uint16 bg_color, uint16 digit_color).
	   ▪ If the touch position is not in one of those 3 regions, do nothing.
	*/
    touchPosition pos; touchRead(&pos);

    if (187 <= pos.px && pos.px <= 187 + 38) {
        if (35 <= pos.py && pos.py <= 35 + 38) { // STOP
            irqDisable(IRQ_TIMER(0));
            // min = sec_init / 60;
            // sec = sec_init % 60;
            min = sec = msec = 0;
            countdown_state = STOP;
            changeMainColorDisp(WHITE, BLACK);
        }
        if (35 + 38 + 4 <= pos.py && pos.py <= 35 + 2*38 + 4) { // PLAY
            irqEnable(IRQ_TIMER(0));
            countdown_state = PLAY;
            changeMainColorDisp(YELLOW, BLACK);
        }
        if (35 + 2*38 + 2*4 <= pos.py && pos.py <= 35 + 3*38 + 2*4) { // PAUSE
            irqDisable(IRQ_TIMER(0));
            countdown_state = PAUSE;
            changeMainColorDisp(GREEN, BLACK);
        }
    }

    // If we are in the region of the 3 buttons (STOP, PLAY and PAUSE),
    // perform the actions presented above.
    //... TO COMPLETE EXERCISE 5
    /*	if (...)
	{


	}
*/
}

/////////////////////////////////////////////////////////
////	DO NOT MODIFY THE FUNCTION BELOW
/////////////////////////////////////////////////////////

/*
 * ISR_countdown_timer:
 * Interrupt Service Routine of the timer.
 * After each 1 ms, it decrements the countdown timer.
 */
void ISR_countdown_timer() {
    if (msec || sec || min)  // if the countdown timer did not reach 00:00.000
    {
        if (!msec && (sec || min)) {
            msec = 999;  // Reset to the max of the ms range
            if (!sec) {
                sec = 59;  // Reset to the max of the sec range
                if (min)
                    min--;  // Decrement 1 minutes
            } else
                sec -= 1;  // Decrement 1 seconds
        } else
            msec -= 1;  // Decrement 1 ms
    } else  // Countdown finished!!!
    {
        // Change the color of the MAIN screen
        changeMainColorDisp(RED, WHITE);

        // Disable the Timer IRQ
        irqDisable(IRQ_TIMER0);
    }
}

/////////////////////////////////////////////////////////
////	DO NOT MODIFY THE FUNCTION BELOW
/////////////////////////////////////////////////////////

/*
 * ISR_VBlank:
 * Interrupt Service Routine of the screens, to refresh them properly.
 */
void ISR_VBlank() {
    // Handles the inputs (keys / touchscreen)
    handleInput();

    // Update the time value printed on the MAIN screen
    updateTimerDisp(BG_MAP_RAM(0), min, sec, msec);

    // Update the rotation angle of the countdown timer's hand
    updateClkHandPosition(sec_init, min * 60 + sec);
}
