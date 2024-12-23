#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "game.h"

// Helper function to check if board has empty cells
int hasEmptyCell(const int *board, const int size)
{
    for (int i = 0; i < size * size; i++, board++)
    {
        if (*board == 0)
            return 1;
    }
    return 0;
}

// Helper function to count empty cells
int countEmptyCells(const int *board, const int size)
{
    int emptyCells = 0;
    for (int i = 0; i < size * size; i++, board++)
    {
        if (*board == 0)
            emptyCells++;
    }
    return emptyCells;
}

// Helper function that fills a random empty cell with 2 or 4
void fillEmptyCells(int *board, const int size, const int cellsToFill)
{
    if (!hasEmptyCell(board, size))
        return;

    int emptyCells;
    int *current = board;

    // Count empty cells
    emptyCells = countEmptyCells(board, size);

    // Fill cells with 2 or 4
    for (int filled = 0; filled < cellsToFill; filled++)
    {
        int randomIndex = rand() % emptyCells;
        int currentEmpty = 0;
        current = board;

        // Find and fill the nth empty cell
        for (int i = 0; i < size * size; i++, current++)
        {
            if (*current == 0)
            {
                if (currentEmpty == randomIndex)
                {
                    *current = (rand() % 10 < 7) ? 2 : 4;
                    emptyCells--;
                    break;
                }
                currentEmpty++;
            }
        }
    }
}

// Helper function to print dashes
void printDashes(const int size)
{
    printf("-");
    for (int j = 0; j < size; j++)
    {
        printf("-------");
    }
    printf("\n");
}

// Print the board
void printBoard(const int *board, const int size)
{
    // Print top border
    printDashes(size);

    for (int i = 0; i < size; i++)
    {
        printf("|");
        for (int j = 0; j < size; j++)
        {
            if (*(board + i * size + j) == 0)
            {
                printf("      |");
            }
            else
            {
                printf(" %4d |", *(board + i * size + j));
            }
        }
        printf("\n");

        // Print separator between rows
        printDashes(size);
    }
    printf("\n\n");
}

// Print the menu
void printMenu()
{
    printf("Please choose one of the following options:\n");
    printf("N/n - New Game\n");
    printf("R/r - Move Right\n");
    printf("L/l - Move Left\n");
    printf("U/u - Move Up\n");
    printf("D/d - Move Down\n");
    printf("E/e - Exit\n");
}

// Reset the board
void resetBoard(int *board, const int size)
{
    for (int i = 0; i < size * size; i++, board++)
    {
        *(board) = 0;
    }
}

// Validate next move
void handleMove(int *board, const int size, int *score, int *maxValue, int moveScore, int moved, int *bestScore)
{
    if (!moved)
    {
        printf("Nothing to move in this direction, choose another option\n\n\n");
        return;
    }
    *score += moveScore;
    fillEmptyCells(board, size, 1);
    updateBestScore(bestScore, score);
    printScore(score, bestScore);
    printBoard(board, size);
}

// Check possible merges horizontally
int checkMergesHorizontal(const int *board, const int size, int row, int col)
{
    for (row = 0; row < size; row++)
    {
        for (col = 0; col < size - 1; col++)
        {
            if (*(board + size * row + col) == *(board + size * row + col + 1))
            {
                return 0;
            }
        }
    }
    return -1;
}

// Check possible merges Vertically
int checkMergesVertical(const int *board, const int size, int row, int col)
{
    for (row = 0; row < size; row++)
    {
        for (col = 0; col < size - 1; col++)
        {
            if (*(board + size * row + col) == *(board + size * (row + 1) + col))
            {
                return 0;
            }
        }
    }
    return -1;
}

// Check if the game is over
int gameOver(const int *board, const int size)
{
    int resVertical;
    int resHorizontal;
    if (hasEmptyCell(board, size))
    {
        return 0;
    }

    resHorizontal = checkMergesHorizontal(board, size, 0, 0);
    if (resHorizontal != 0)
    {
        resVertical = checkMergesVertical(board, size, 0, 0);
        if (resVertical != 0)
            return 1; // Game over
    }
    return 0;
}

// Get a cell in a given direction
int *getCell(int *board, const int size, const int row, const int col, const int direction)
{
    if (direction == 0) // LEFT
        return board + row * size + col;
    else if (direction == 1) // RIGHT
        return board + row * size + (size - 1 - col);
    else if (direction == 2) // UP
        return board + col * size + row;
    else // DOWN
        return board + (size - 1 - col) * size + row;
}

// Merge cells in a given direction
void mergeCells(int *board, const int size, const int row, const int direction, int *maxValue, int *moved, int *score)
{
    for (int i = 0; i < size - 1; i++)
    {
        int *current = getCell(board, size, row, i, direction);
        int *next = getCell(board, size, row, i + 1, direction);
        if (*current != 0 && *current == *next)
        {
            *current *= 2;
            *score += *current; // Add merged value to score
            if (*current > *maxValue)
            {
                *maxValue = *current;
            }
            *next = 0;
            *moved = 1;
            break;
        }
    }
}

// Shift cells in a given direction
void shiftCells(int *board, const int size, const int row, const int direction, int *moved, int insertIndex)
{
    for (int i = 0; i < size; i++)
    {
        if (*getCell(board, size, row, i, direction) != 0)
        {
            if (insertIndex != i)
            {
                *getCell(board, size, row, insertIndex, direction) = *getCell(board, size, row, i, direction);
                *getCell(board, size, row, i, direction) = 0;
                *moved = 1;
            }
            insertIndex++;
        }
    }
}

// Shift and merge in a given direction
void shiftAndMergeInDirection(int *board, const int size, const int row, const int direction, int *maxValue, int *moved, int *score)
{
    int insertIndex = 0;
    // First shift
    shiftCells(board, size, row, direction, moved, insertIndex);

    // Merge and calculate score
    mergeCells(board, size, row, direction, maxValue, moved, score);

    if (*moved)
        // Second shift
        shiftCells(board, size, row, direction, moved, insertIndex);
}

// Move in a given direction
int moveDirection(int *board, const int size, int *maxValue, int *moved, const int direction)
{
    *moved = 0;
    int score = 0;

    for (int i = 0; i < size; i++)
    {
        shiftAndMergeInDirection(board, size, i, direction, maxValue, moved, &score);
    }

    return score;
}

// Print game start
void displayGameStart(int scoreToWin)
{
    printf("---------------------------------------\n");
    printf("-----------  STARTING A GAME ----------\n");
    printf("           Score for a win %d\n", scoreToWin);
    printf("---------------------------------------\n\n");
}

// Update best score
void updateBestScore(int *bestScore, int *score)
{
    if (*score > *bestScore)
    {
        *bestScore = *score;
    }
}

// Print score
void printScore(int *score, int *bestScore)
{
    printf("Score %d Best %d\n", (*score), (*bestScore));
}

// Game play function
void playGame(int *board, const int size, const int scoreToWin)
{
    int bestScore = 0;
    int playAgain = 1;
    char flag;
    resetBoard(board, size);
    printBoard(board, size);

    while (playAgain)
    {
        char move = ' ';
        int score = 0;
        int moved;
        int maxValue = 0;
        int gameStarted = 0;

        while ((move != 'e' || move != 'E') && maxValue < scoreToWin)
        {

            printMenu();
            scanf(" %c", &move);

            if (!gameStarted && move != 'n')
            {
                printf("Need to start game first\n\n\n");
                continue;
            }

            int moveScore = 0;
            switch (move)
            {
            case 'n':
            case 'N':
                score = 0;
                resetBoard(board, size);
                fillEmptyCells(board, size, 2);
                displayGameStart(scoreToWin);
                printScore(&score, &bestScore);
                printBoard(board, size);
                gameStarted = 1;
                break;
            case 'l':
            case 'L':
                moveScore = moveDirection(board, size, &maxValue, &moved, 0);
                handleMove(board, size, &score, &maxValue, moveScore, moved, &bestScore);
                break;
            case 'r':
            case 'R':
                moveScore = moveDirection(board, size, &maxValue, &moved, 1);
                handleMove(board, size, &score, &maxValue, moveScore, moved, &bestScore);
                break;
            case 'u':
            case 'U':
                moveScore = moveDirection(board, size, &maxValue, &moved, 2);
                handleMove(board, size, &score, &maxValue, moveScore, moved, &bestScore);
                break;
            case 'd':
            case 'D':
                moveScore = moveDirection(board, size, &maxValue, &moved, 3);
                handleMove(board, size, &score, &maxValue, moveScore, moved, &bestScore);
                break;
            case 'e':
            case 'E':
                printf("Ending previous game - your score %d best score %d\n\n Bye Bye\n", score, bestScore);
                break;
            default:
                printf("Invalid option. Please try again.\n");
            }

            if (gameOver(board, size))
            {
                printf("Game over your score is %d.\n", score);
            }
        }

        // Handle all game end scenarios (win, lose, or exit)
        if (move == 'e' || move == 'E' || maxValue >= scoreToWin)
        {
            if (maxValue >= scoreToWin)
            {
                printf("You won %d! - Ending Game\n", maxValue);
            }

            printf("Would you like to play again? (Y/N): ");
            scanf(" %c", &flag);

            switch (flag)
            {
            case 'y':
            case 'Y':
                playAgain = 1;
                break;
            case 'n':
            case 'N':
                playAgain = 0;
                break;
            default:
                printf("Invalid option. Exiting game.\n");
                playAgain = 0;
            }
        }
    }
}
