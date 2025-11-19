#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include "board.h"

#define MAX_DIE 6

int rolldie(void)
{
    return rand()%MAX_DIE+1;
}

int main(int argc, char *argv[])
{
    int cnt;
    int pos;
    int coinResult;
    
  srand((unsigned)(time(NULL)));
  
  printf("===============================================\n");
  printf("====@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@====\n");
  printf("====           SHARK GAME START!           ====\n");
  printf("====@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@====\n");
  printf("===============================================\n");
  
  board_initBoard();
  
  cnt=0;
  pos=0;
  coinResult=0;
  do{
        int die_result;
        int coinResult;
        
        board_printBoardStatus();
        
        die_result = rolldie();
        
        pos+=die_result;
        
        printf("pos:%i(die:%i)\n", pos, die_result);
        
        coinResult+=board_getBoardCoin(pos);
        
        printf("coin:%i\n", coinResult);
        
        cnt++;
        
} while(cnt<5);
  
  
  printf("\n\n\n\n\n\n\n"); 
  printf("===============================================\n");
  printf("====@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@====\n");
  printf("====            SHARK GAME END!            ====\n");
  printf("====@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@====\n");
  printf("===============================================\n");
  system("PAUSE");	
  return 0;
}
