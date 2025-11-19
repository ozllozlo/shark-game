#if 0
int board_getSharkPosition(void);
int board_stepShark(void);
#endif

#define N_BOARD            20
#define BOARDSTATUS_OK     1
#define BOARDSTATUS_NOK    0
#define N_COINPOS          12
#define MAX_COIN           4

static int board_status[N_BOARD];
static int board_coin[N_BOARD];
static int shark_position;

void board_printBoardStatus(void)
{
     int i;
     printf("----------BOARD STATUS----------\n");
     for(i=0;i<N_BOARD;i++)
     {
     printf("|");
     if(board_status[i]==BOARDSTATUS_NOK)
     printf("X");
     else
     printf("O");
     }
     
     printf("|\n");
     printf("--------------------------------\n");
     
      printf("----------BOARD COIN----------\n");
     for(i=0;i<N_BOARD;i++)
     {
     printf("|%i", board_coin[i]);

     }
     
     printf("|\n");
     printf("--------------------------------\n");
}


int board_getBoardStatus(int pos)
{
     return board_status[pos];
}

int board_getBoardCoin(int pos)
{
    int coin=board_coin[pos];
    board_coin[pos]=0;
    return board_coin[pos];
}

void board_initBoard(void)
{
    int i;
    for(i=0;i<N_BOARD;i++)
    {
                          board_status[i]=BOARDSTATUS_OK;
                          board_coin[i]=0;
                          }
                          
    for(i=0;i<N_COINPOS;i++)
    
{
    int flag_allocated=0;
    do{
        int coinpos = rand()%N_BOARD;
        if(board_coin[coinpos]==0)
        {
        board_coin[coinpos]=rand()%MAX_COIN+1;
        flag_allocated=1;                            
        }
        } while(flag_allocated==0);
}
     
     }
