/////////////////////////////////////////////////////////
////	DO NOT MODIFY THIS FILE
/////////////////////////////////////////////////////////

#ifndef GAME_H_
#define GAME_H_

#include <nds.h>

#define BLACK	ARGB16(1,0,0,0)
#define WHITE	ARGB16(1,31,31,31)
#define RED		ARGB16(1,31,0,0)
#define GREEN	ARGB16(1,0,31,0)
#define BLUE	ARGB16(1,0,0,31)

/*
 * initGame:
 * This function initializes the game. It can be called several times, and each
 * time it will restart the game. It generates a random 16x12 map of white and
 * black boxes and plots them in the screen.
 * IMPORTANT: It requires the correct implementation of the function
 * fillRectangle(...) prototyped in grahpics_main.h
 */
void initGame();

/*
 * playerPlayBlack:
 * This function is called when the player has chosen to play the Black color
 */
void playerPlaysBlack();

/*
 * playerPlaysWhite:
 * This function is called when the player has chosen to play the White color
 */
void playerPlaysWhite();

/*
 * playerWins:
 * This function is called by playerPlaysBlack() or playerPlaysWhite()
 * when the player has chosen the correct color. It replaces the checked
 * screen (white and black blocks) by a full screen in green
 * IMPORTANT: It requires the correct implementation of the function
 * fillRectangle(...) prototyped in grahpics_main.h
 */
void playerWins();

/*
 * playerLoses:
 * This function is called by playerPlaysBlack() or playerPlaysWhite()
 * when the player has chosen the incorrect color. It replaces the checked
 * screen (white and black blocks) by a full screen in green
 * This function is also used in case of timeout (from the timer ISR)
 * IMPORTANT: It requires the correct implementation of the function
 * fillRectangle(...) prototyped in grahpics_main.h
 */
void playerLoses();

#endif /* GAME_H_ */
