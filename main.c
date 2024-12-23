#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

#include "game.h"

#define MAX_SIZE 8

int main()
{
	srand((unsigned)time(NULL));

	int board[MAX_SIZE][MAX_SIZE] = {0};

	printf("-----------------------------------------------\n");
	printf("----  Game 1:  board size 3*3 win value 16 -----  \n");
	printf("-----------------------------------------------\n");
	playGame((int *)board, 3, 16);
	printf("-----------------------------------------------\n");
	printf("----  Game 2:  board size 2*2 win value 32 -----  \n");
	printf("-----------------------------------------------\n");
	playGame((int *)board, 2, 32);
	printf("-----------------------------------------------\n");
	printf("---  Game 3:  board size 4*4 win value 2048 ---  \n");
	printf("-----------------------------------------------\n");
	playGame((int *)board, 4, 2048);
}
