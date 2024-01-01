
#include "P_Score.h"
#include "numbers.h"
#include <stdio.h>
#include <fat.h>

int score = 0, max_score = 0;

void updateScore(int rows)
{
	/*
	 * Depending on the lines increase the score
	 *  1 point - one single line
	 *  4 points - 2 lines
	 *  8 points - 3 lines
	 *  10 points - 4 lines
	 */
	switch(rows)
	{
	case 1: score+=1; break;
	case 2: score+=4; break;
	case 3: score+=8; break;
	case 4: score+=15; break;
	}

	//Display the score
	displayScore();

	//Update highest score
	if(score > max_score)
	{
		max_score = score;
		writeMaxScore();
		displayMaxScore();
	}
}


/*
 * This function displays the score counter on the right of the
 * screen (looking sidewise) using the palette 9 (green color preloaded)
 *
 * The map (24) is modified accordingly to the points stored in the
 * variable store
 */

void displayScore()
{
	int j,i, number;
	//i, digit to display
	for(i = 5; i>0; i--)
	{
		//calculate the power of i-1 in j
		j = i-1; number = 1;
		while(j--)number = number*10;
		//Obtain the number to render
		number = score / number;
		number = number % 10;
		//Render the number
		BG_MAP_RAM_SUB(24)[1+(23-i)*32] = numbersMap[(10 - number)*2] | TILE_PALETTE(9);
		BG_MAP_RAM_SUB(24)[1+(23-i)*32+1] = numbersMap[(10 - number)*2+1] | TILE_PALETTE(9);
	}
}


/*
 * This function reads the highest score obtained in previous games and
 * stored in a text file called /Tetrisrecord.txt. The read value is assigned
 * to the variable max_score
 */
void readMaxScore()
{
	//Open the file in read mode
	FILE* file = fopen("/Tetrisrecord.txt","r");
	if(file != NULL)
	{
		//Read the value and put it into the variable max_score
		fscanf(file,"%i\n",&max_score);
		// close the file
		fclose(file);
	}
}

/*
 * This function writes the value of the variable max_score
 * into the text file /tetrisrecord
 */
void writeMaxScore()
{
	//Open the file in write mode to overwrite
	FILE* file = fopen("/Tetrisrecord.txt","w+");
	if(file != NULL)
	{
		//Print the value in the file
		fprintf(file,"%i\n",max_score);
		//Close the file
		fclose(file);
	}
}

/*
 * This function displays the highest score obtained in previous games
 * on the left part of bottom the screen (looking sideways). It uses the
 * palette 8 (with red color preloaded)
 */
void displayMaxScore()
{
	int i, j, number;
	for(i = 5; i>0; i--)
	{
		j = i-1; number = 1;
		while(j--)number = number*10;
		number = max_score / number;
		number = number % 10;
		BG_MAP_RAM_SUB(24)[1+(6-i)*32] = numbersMap[(10 - number)*2] | TILE_PALETTE(8);
		BG_MAP_RAM_SUB(24)[1+(6-i)*32+1] = numbersMap[(10 - number)*2+1] | TILE_PALETTE(8);
	}
}
