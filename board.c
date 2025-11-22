#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"

static int board_status[N_BOARD]; 
static int board_coin[N_BOARD];    
static int shark_position = SHARK_INITPOS;

int board_initBoard(void)
{
    int i;

    for (i = 0; i < N_BOARD; ++i) {
        board_status[i] = BOARDSTATUS_OK;
        board_coin[i] = 0;
    }
    shark_position = SHARK_INITPOS;

    for (i = 0; i < N_COINPOS; ++i) {
        int placed = 0;
        while (!placed) {
            int pos = rand() % N_BOARD;
            if (board_coin[pos] == 0) {
                board_coin[pos] = (rand() % MAX_COIN) + 1;
                placed = 1;
            }
        }
    }

    return 0;
}


void board_printBoardStatus(void)
{
    int i;
    printf("Board status:\n");
    for (i = 0; i < N_BOARD; ++i) {
        char s = (board_status[i] == BOARDSTATUS_OK) ? 'O' : 'X';
        if (i == shark_position) {
            printf("[S]"); 
        } else {
            printf(" %c ", s);
        }
    }
    printf("\nCoins per cell:\n");
    for (i = 0; i < N_BOARD; ++i) {
        if (board_coin[i] > 0)
            printf("[%2d]", board_coin[i]);
        else
            printf("[  ]");
    }
    printf("\n");
}


int board_getBoardStatus(int pos)
{
    if (pos < 0 || pos >= N_BOARD) return BOARDSTATUS_NOK;
    return board_status[pos];
}


int board_getBoardCoin(int pos)
{
    if (pos < 0 || pos >= N_BOARD) return 0;
    int c = board_coin[pos];
    board_coin[pos] = 0;
    return c;
}


int board_getSharkPosition(void)
{
    return shark_position;
}


int board_stepShark(void)
{
    int step = (rand() % MAX_SHARKSTEP) + 1; 
    int newpos = shark_position + step;

    if (newpos >= N_BOARD) newpos = N_BOARD - 1;

  
    int i;
    for (i = shark_position + 1; i <= newpos; ++i) {
        if (i >= 0 && i < N_BOARD) {
            board_status[i] = BOARDSTATUS_NOK;
           
        }
    }

    shark_position = newpos;
    return shark_position;
}
