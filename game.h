#ifndef GAME_H
#define GAME_H

// Board initialization functions
int hasEmptyCell(const int *board, const int size);
void fillEmptyCells(int *board, const int size, const int cellsToFill);
void resetBoard(int *board, const int size);
void printBoard(const int *board, const int size);
int countEmptyCells(const int *board, const int size);
void printMenu();
void displayGameStart(int scoreToWin);
void printScore(int *score, int *bestScore);
void printDashes(const int size);
void updateBestScore(int *bestScore, int *score);

// Board movement functions
void shiftCells(int *board, const int size, const int row, const int direction, int *moved, int insertIndex);
void mergeCells(int *board, const int size, const int row, const int direction, int *maxValue, int *moved, int *score);
int *getCell(int *board, const int size, const int row, const int col, const int direction);
void shiftAndMergeInDirection(int *board, const int size, const int row, const int direction, int *maxValue, int *moved, int *score);
int moveDirection(int *board, const int size, int *maxValue, int *moved, const int direction);

// Game play functions
void playGame(int *board, int size, int scoreToWin);
void handleMove(int *board, const int size, int *score, int *maxValue, int moveScore, int moved, int *bestScore);
int gameOver(const int *board, const int size);

#endif /* GAME_H */
