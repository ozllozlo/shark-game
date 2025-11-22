#ifndef BOARD_H
#define BOARD_H


#define N_BOARD         20   
#define N_COINPOS       6     
#define MAX_COIN        5     
#define MAX_SHARKSTEP   3     
#define SHARK_INITPOS  -1    

#define BOARDSTATUS_OK  1
#define BOARDSTATUS_NOK 0


int board_initBoard(void);
void board_printBoardStatus(void);
int board_getBoardStatus(int pos);     
int board_getBoardCoin(int pos);       
int board_getSharkPosition(void);
int board_stepShark(void);            

#endif 
