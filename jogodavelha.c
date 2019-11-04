#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char p1[255], p2[255];

char gridChar(int i, int pos)
{
    switch(i)
    {
    case -1:
        return 'x';
    case 0:
        return pos+48;
    case 1:
        return 'o';
    }
}

void draw(int b[9])
{
    printf(" %c | %c | %c\n",gridChar(b[0],0),gridChar(b[1],1),gridChar(b[2],2));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n",gridChar(b[3],3),gridChar(b[4],4),gridChar(b[5],5));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n",gridChar(b[6],6),gridChar(b[7],7),gridChar(b[8],8));
}

int win(const int board[9])
{
//determines if a player has won, returns 0 otherwise.
    unsigned wins[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    int i;
    for(i = 0; i < 8; ++i)
    {
        if(board[wins[i][0]] != 0 &&
                board[wins[i][0]] == board[wins[i][1]] &&
                board[wins[i][0]] == board[wins[i][2]])
            return board[wins[i][2]];
    }
    return 0;
}

int minimax(int board[9], int player)
{
//How is the position like for player (their turn) on board?
    int winner = win(board);
    if(winner != 0) return winner*player;

    int move = -1;
    int score = -2;//Losing moves are preferred to no move
    int i;
    for(i = 0; i < 9; ++i)  //For all moves,
    {
        if(board[i] == 0)  //If legal,
        {
            board[i] = player;//Try the move
            int thisScore = -minimax(board, player*-1);
            if(thisScore > score)
            {
                score = thisScore;
                move = i;
            }//Pick the one that's worst for the opponent
            board[i] = 0;//Reset board after try
        }
    }
    if(move == -1) return 0;
    return score;
}

void board2string(int board[9],char s[10]) {
    //char s[10] = "000000000";
    int i;
    for (i = 0; i < 9; i++) {
        //printf("(%c)",gridChar(board[i],47));
        s[i] = gridChar(board[i],47);
    }
    //printf("(%s)\n",s);
    //return s;
}

void computerMove(int board[9])
{
    int move = -1;
    if (!strcmp(p1,"")) {
        int score = -2;
        int i;
        for(i = 0; i < 9; ++i)
        {
            if(board[i] == 0)
            {
                board[i] = 1;
                int tempScore = -minimax(board, -1);
                board[i] = 0;
                if(tempScore > score)
                {
                    score = tempScore;
                    move = i;
                }
            }
        }
    }
    else {
        char cmd[255];
        //printf("(%s)",board2string(board));
        char s[10];
        board2string(board,s);
        #ifdef _WIN32
        sprintf(cmd,"%s %s",p2,s);
        FILE * f = _popen(cmd,"r");
        #else
        sprintf(cmd,"./%s %s",p2,s);
        FILE * f = popen(cmd,"r");
        #endif
        fscanf(f,"%d",&move);
        printf("Auto-Play - Command: %s Move: %d\n",cmd,move);
        pclose(f);
    }
//returns a score based on minimax tree at a given node.
    if (move >= 0 && move <= 8 && board[move] == 0)
        board[move] = 1;
}



void playerMove(int board[9])
{
    int move = -1;
    //do
    //{
        //printf("(%s)",p1);
        if (!strcmp(p1,"")) {
            printf("\nInput move ([0..8]): ");
            scanf("%d", &move);
            printf("\n");
        }
        else {
            char cmd[255];
            //printf("(%s)",board2string(board));
            char s[10];
            board2string(board,s);
            sprintf(cmd,"%s %s",p1,s);
            FILE * f = popen(cmd,"r");
            fscanf(f,"%d",&move);
            printf("Auto-Play - Command: %s Move: %d\n",cmd,move);
            pclose(f);
        }
    //}
    //while (move >= 9 || move < 0 || board[move] != 0);
    if (move >= 0 && move <= 8 && board[move] == 0)
        board[move] = -1;
}

int main(int argc, char * argv[])
{
    //printf("teste");
    if (argc > 1) strcpy(p1,argv[1]);
    if (argc > 2) strcpy(p2,argv[2]);
    //printf("teste2 (%s) (%s)",p1,p2);

    int board[9] = {0,0,0,0,0,0,0,0,0};
    //printf("%s\n",board2string(board));
//computer squares are 1, player squares are -1.
    //printf("Computer: O, You: X\nPlay (1)st or (2)nd? ");
    int player=1;
    //scanf("%d",&player);
    //printf("\n");
    unsigned turn;
    draw(board);
    for(turn = 0; turn < 9 && win(board) == 0; ++turn)
    {
        if((turn+player) % 2 == 0) {
            computerMove(board);
        }
        else
        {
            playerMove(board);
        }
        printf("\n");
        draw(board);
    }
    char s[255] = "";
    switch(win(board))
    {
    case 0:
        printf("Empate.\n");
        break;
    case 1:
        if (!strcmp(p1,"")) {
            strcat(s,"Jogador 2");
        }
        else {
            strcat(s,p2);
        }
        strcat(s," venceu!");
        printf("%s\n",s);
        break;
    case -1:
        if (!strcmp(p2,"")) {
            strcat(s,"Jogador 1");
        }
        else {
            strcat(s,p1);
        }
        strcat(s," venceu!");
        printf("%s\n",s);
        break;
    }
}
