/////////////////////////////////////////////////////////
////	DO NOT MODIFY THIS FILE
/////////////////////////////////////////////////////////

#include "game.h"
#include "graphics_main.h"
#include "graphics_sub.h"
#include "timer_management.h"

bool gameFinished = false;
int whiteBlocks;

void initGame() {
	int i,j;
	bool grid[16*12];
	whiteBlocks = 0;

	for(i=0; i<12; i++)
		for(j=0;j<16;j++)
			grid[i*16+j] = (i%2)^(j%2);

	for(i=0;i<400;i++)
		grid[rand()%192] ^=1;

	for(i=0; i<12; i++)
		for(j=0;j<16;j++)
			if(grid[i*16+j]) {
				whiteBlocks++;
				fillRectangle(j*16,(j+1)*16-1,i*16,(i+1)*16-1, WHITE);
			}
			else {
				fillRectangle(j*16,(j+1)*16-1,i*16,(i+1)*16-1, BLACK);
			}
	gameFinished = false;
}

void playerPlaysBlack() {
	if(whiteBlocks > 96)
		playerLoses();
	else
		playerWins();
}

void playerPlaysWhite() {
	if(whiteBlocks < 96)
		playerLoses();
	else
		playerWins();
}

void playerWins() {
	if(!gameFinished){
		gameFinished = true;
		fillRectangle(0,255,0,191,GREEN);
	}
}

void playerLoses() {
	if(!gameFinished){
		gameFinished = true;
		fillRectangle(0,255,0,191,RED);
	}
}



