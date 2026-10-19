#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "common.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 4096


// prints the 7x6 flattened board array in the correct format
void print_board(int board[]) {
 
    printf("\n");
    for (int col = 0; col < 7; col++)
        printf(" %d", col);
    printf("\n");
 
    for (int row = 0; row < 6; row++) {
        printf("|");
        for (int col = 0; col < 7; col++) {
            int cell = board[row * 7 + col];
            printf("%c|", cell == 0 ? 'X' : cell == 1 ? 'O' : '.');
        }
        printf("\n");
    }
    printf("\n");
}


int main() {
    setbuf(stdout, NULL);
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);


    printf("Connect 4\n");
    printf("  1) Create a new game\n");
    printf("  2) Join a game\n");
    printf("> ");
    int choice = 0;
    scanf("%d", &choice);
    if(choice==1){
        printf("\n");

    }
   

    // TODO: implement client logic

    struct request req;
    req.type=choice;
    req.gameId=0;
    req.col=0;

    if(choice==2){
        printf("Game ID? >");
        scanf("%d", &req.gameId);
        printf("\n");
    }

    sendto(sock_fd, &req, sizeof(req), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    int gameId;
    socklen_t server_len = sizeof(server_addr);


    recvfrom(sock_fd, &gameId, sizeof(gameId), 0, (struct sockaddr *)&server_addr, &server_len);

    if(gameId==-1){
        printf("Game does not exist.\n");
        close(sock_fd);
        return 0;
    }


    int board[42];
    for(int i=0; i<42; i++){
        board[i]=-1;
    }

    printf("Current Game\n");
    printf("---------------------\n");
    printf("GameID: %d\n", gameId);
    print_board(board);

    if(choice==1){
        printf("waiting for other player...\n\n");

        int joinedGameId;
        recvfrom(sock_fd, &joinedGameId, sizeof(joinedGameId), 0, (struct sockaddr *)&server_addr, &server_len);
        
        printf("Current Game\n");
        printf("---------------------\n");
        printf("GameID: %d\n", gameId);
        print_board(board);

        
    }

    int player; 
    if(choice==1){
        player = 0;
    }
    else{
        player=1;
    }

    int currentPlayer = 0;

    while(1){


    if(currentPlayer==player){
        int col;

        printf("\nInput Move Row: > ");
        scanf("%d", &col);
        printf("\n");

        req.type=3;
        req.gameId=gameId;
        req.col=col;

        sendto(sock_fd, &req, sizeof(req), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        
    }

    else{
        printf("waiting for other player...\n\n");
    }

    struct result res;
    recvfrom(sock_fd, &res, sizeof(res), 0, (struct sockaddr *)&server_addr, &server_len);

   
    printf("Current Game\n");
    printf("---------------------\n");
    printf("GameID: %d\n", gameId);
    print_board(res.board);

    int tie = 1;
    for(int i=0; i<42; i++){
        if(res.board[i]==-1){
            tie=0;
        }
    }

    if(tie==1 && res.winner==-1){
        printf("TIE\n");
        break; 
    }

    if(res.winner!=-1){
        if(res.winner==player){
            printf("YOU WIN!\n");
        }
        else{
            printf("YOU LOSE!\n");
        }

        break;
    }

    if(currentPlayer==0){
        currentPlayer=1;
    }
    else{
        currentPlayer=0;
    }

}


    close(sock_fd);


}