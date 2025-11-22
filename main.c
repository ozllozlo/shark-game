#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "board.h"

/* 게임 관련 매크로 (필요시 변경) */
#define N_PLAYER 4

#define PLAYERSTATUS_LIVE  0
#define PLAYERSTATUS_DIE   1
#define PLAYERSTATUS_END   2

/* 플레이어 정보 */
char player_name[N_PLAYER][32];
int player_pos[N_PLAYER];
int player_coin[N_PLAYER];
int player_status[N_PLAYER];

/* 함수 선언 (main.c 내부 보조 함수들) */
void initPlayers(void);
void printPlayerStatus(void);
int rollDice(void);
void checkDie(void);
int getAlivePlayer(void);
int getWinner(void);

int main(void)
{
    int turn = 0; /* 현재 턴(플레이어 인덱스) */
    int roundCount = 0;

    srand((unsigned)time(NULL));

    board_initBoard();
    initPlayers();

   printf("===============================================\n");
   printf("====@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@====\n");
   printf("====           SHARK GAME START!           ====\n");
   printf("====@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@====\n");
   printf("===============================================\n");

    /* 메인 게임 루프: 모든 플레이어가 살아있지 않을 때까지(자료에 따라 변경 가능) */
    while (1) {
        /* 게임 상태 출력 */
        printf("\n--- ROUND %d ---\n", roundCount + 1);
        board_printBoardStatus();
        printPlayerStatus();

        /* 현재 플레이어 턴 */
        if (player_status[turn] == PLAYERSTATUS_LIVE) {
            printf("\n[%s] turn!! Press any key to roll a die!", player_name[turn]);
            getchar(); /* Enter 대기 (편하면 주석 처리 가능) */
            int dice = rollDice();
            printf("%s moves forward %d spaces.\n", player_name[turn], dice);

            player_pos[turn] += dice;
            if (player_pos[turn] >= N_BOARD) player_pos[turn] = N_BOARD - 1;

            printf("%s's position: %d\n", player_name[turn], player_pos[turn]);

            /* 도착 칸의 동전 습득 */
            int got = board_getBoardCoin(player_pos[turn]);
            if (got > 0) {
                player_coin[turn] += got;
                printf("  -> get %d coin! (total %d)\n", got, player_coin[turn]);
            }

            /* 마지막 칸 도달 시 상태 변경 */
            if (player_pos[turn] == N_BOARD - 1) {
                player_status[turn] = PLAYERSTATUS_END;
                printf("  -> %s reached the finish\n", player_name[turn]);
            }
        } else {
            /* 죽었거나 이미 끝난 플레이어는 턴 스킵 */
            if (player_status[turn] == PLAYERSTATUS_DIE) {
                printf("\n%s is DIE status. Pass the turn.\n", player_name[turn]);
            } else {
                printf("\n%s has reached the finish already. Pass the turn.\n", player_name[turn]);
            }
        }

        /* 다음 플레이어로 */
        turn = (turn + 1) % N_PLAYER;

        /* 플레이어가 모두 한 번씩 돌았으면(즉 turn == 0이면) 상어가 이동 */
        if (turn == 0) {
            printf("\n>>> SHARK MOVE!\n");
            int spos = board_stepShark();
            printf("SHARK POSITION: %d (이후 해당 칸은 파손 처리됨)\n", spos);
            /* 상어가 이동한 뒤 플레이어 사망 체크 */
            checkDie();
            roundCount++;
        }

        /* 게임 종료 조건: 살아있는 플레이어가 하나도 없으면 종료 (자료에 따라 변경 가능) */
        if (getAlivePlayer() == 0) {
            printf("\nNo players can continue. FINISH.\n");
            break;
        }

        /* (선택) 라운드 제한 등 추가 조건을 원하면 여기서 걸 수 있음 */
    }

    /* 승자 결정: 살아있는 플레이어 중 동전이 가장 많은 플레이어 */
    int winner = getWinner();
    if (winner >= 0) {
        printf("\n=== WINNER ===\n");
        printf("WINNER: %s (coin %d)\n", player_name[winner], player_coin[winner]);
    } else {
        printf("\nNo live. No winner.\n");
    }

   printf("\n\n\n\n\n\n\n"); 
   printf("===============================================\n");
   printf("====@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@====\n");
   printf("====            SHARK GAME END!            ====\n");
   printf("====@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@+@====\n");
   printf("===============================================\n");
    return 0;
}

/* 플레이어 초기화 및 이름 입력 */
void initPlayers(void)
{
    int i;
    for (i = 0; i < N_PLAYER; ++i) {
        printf("Player %d's name: ", i + 1);
        if (fgets(player_name[i], sizeof(player_name[i]), stdin) != NULL) {
            size_t ln = strlen(player_name[i]);
            if (ln > 0 && player_name[i][ln - 1] == '\n') player_name[i][ln - 1] = '\0';
            if (strlen(player_name[i]) == 0) sprintf(player_name[i], "Player%d", i + 1);
        } else {
            sprintf(player_name[i], "Player%d", i + 1);
        }
        player_pos[i] = 0;
        player_coin[i] = 0;
        player_status[i] = PLAYERSTATUS_LIVE;
    }
}

/* 현재 플레이어 상태 출력 */
void printPlayerStatus(void)
{
    int i;
    printf("Players:\n");
    for (i = 0; i < N_PLAYER; ++i) {
        const char *s = (player_status[i] == PLAYERSTATUS_LIVE) ? "LIVE" :
                        (player_status[i] == PLAYERSTATUS_DIE) ? "DIE" : "END";
        printf(" %s: pos=%2d coins=%2d status=%s\n", player_name[i], player_pos[i], player_coin[i], s);
    }
}

/* 주사위 굴리기 (1..6) */
int rollDice(void)
{
    return (rand() % 6) + 1;
}

/* 상어 이동 후 플레이어 사망 체크: 플레이어가 파손된 칸(BOARDSTATUS_NOK)에 있으면 사망처리 */
void checkDie(void)
{
    int i;
    for (i = 0; i < N_PLAYER; ++i) {
        if (player_status[i] == PLAYERSTATUS_LIVE) {
            int st = board_getBoardStatus(player_pos[i]);
            if (st == BOARDSTATUS_NOK) {
                player_status[i] = PLAYERSTATUS_DIE;
                printf("  !!! %s 님이 상어(파손된 칸)로 인해 사망했습니다! 위치=%d\n", player_name[i], player_pos[i]);
            }
        }
    }
}

/* 살아있는 플레이어 수 반환 */
int getAlivePlayer(void)
{
    int i, cnt = 0;
    for (i = 0; i < N_PLAYER; ++i)
        if (player_status[i] == PLAYERSTATUS_LIVE) cnt++;
    return cnt;
}

/* 승자 찾기: 살아있는 플레이어 중 동전이 가장 많은 사람의 인덱스 반환, 없으면 -1
   (동전 수가 같으면 먼저 찾은(인덱스가 작은) 플레이어를 승자로 선택) */
int getWinner(void)
{
    int i;
    int best = -1;
    int bestCoin = -1;
    for (i = 0; i < N_PLAYER; ++i) {
        if (player_status[i] == PLAYERSTATUS_LIVE || player_status[i] == PLAYERSTATUS_END) {
            if (player_coin[i] > bestCoin) {
                bestCoin = player_coin[i];
                best = i;
            }
        }
    }
    return best;
}
