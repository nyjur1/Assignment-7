#include <arpa/inet.h>
#include <sys/socket.h>


#ifndef COMMON_H
#define COMMON_H

// Define any structs you may find helpful here.

struct request{
    int type;
    int gameId;
    int col;
};

struct result{
    int gameId;
    int board[42];
    int winner;
};

struct board{
    int cells[6][7];
};

struct game{
    int gameId;
    struct board board; 
    int currPlayer; 
};

#define MAX_GAMES 100

struct gameDB{
    struct game games[MAX_GAMES];
    struct sockaddr_in player0_address[MAX_GAMES];
    struct sockaddr_in player1_address[MAX_GAMES];
    int player0_joined[MAX_GAMES];
    int player1_joined[MAX_GAMES];
    int nextGameId;
};

#endif