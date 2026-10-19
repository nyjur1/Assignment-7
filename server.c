#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "game.h"
#include "save.h"

#define PORT 8080
#define MAX_GAMES 100



int main(int argc, char* argv[]) {
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Message board server listening on port %d\n", PORT);

    //TODO: implement game server logic

    struct gameDB db = {0}; 

    if(argc >1 && strcmp(argv[1], "1")==0){
        loadgame(&db, "games.dat");
    }
   


    while (1) {
        struct request req; 
        socklen_t client_len= sizeof(client_addr);
        ssize_t bytes = recvfrom(sock_fd, &req, sizeof(req), 0, (struct sockaddr *)&client_addr, &client_len);

        if(bytes!=sizeof(req)){
            continue;
        }

        if(req.type==1){
            int gameId=-1;

            if(db.nextGameId<MAX_GAMES){
                gameId= db.nextGameId;
                db.nextGameId++;

                new_game(&db.games[gameId], gameId);
                db.player0_address[gameId]= client_addr;
                db.player0_joined[gameId]=1;

            }
        

       

        sendto(sock_fd, &gameId, sizeof(gameId) , 0, (struct sockaddr *)&client_addr, client_len);
    
        }

    else if(req.type==2){
        int gameId=-1;
        
        if(req.gameId>=0 && req.gameId<db.nextGameId && db.player0_joined[req.gameId]&& !db.player1_joined[req.gameId]){
            gameId=req.gameId;
            db.player1_address[gameId]= client_addr;
            db.player1_joined[gameId]=1;

        }

         sendto(sock_fd, &gameId, sizeof(gameId), 0, (struct sockaddr *)&client_addr, client_len);

         if(gameId!=-1){
            sendto(sock_fd, &gameId, sizeof(gameId), 0, (struct sockaddr *)&db.player0_address[gameId], sizeof(db.player0_address[gameId]));

         }

        }
        else if (req.type== 3 ){
            int gameId= req.gameId;
            
            if(gameId<0 || gameId>=db.nextGameId){
                continue;
            }

            if(!db.player0_joined[gameId]|| !db.player1_joined[gameId]){
                continue;
            }

            int player = -1;

            if(client_addr.sin_addr.s_addr == db.player0_address[gameId].sin_addr.s_addr && client_addr.sin_port==db.player0_address[gameId].sin_port){
                player=0; 
            }

            else if(client_addr.sin_addr.s_addr == db.player1_address[gameId].sin_addr.s_addr && client_addr.sin_port==db.player1_address[gameId].sin_port){
                player=1; 
            }

            if(make_move(&db.games[gameId], player,req.col)!=0){
                continue; 
            }

            savegame(&db, "games.dat");

            struct result res; 
            res.gameId = gameId;
            res.winner= check_win(&db.games[gameId]);

            for(int row=0; row<6; row++){
                for(int col=0; col<7; col++){
                    res.board[row*7+col]=db.games[gameId].board.cells[row][col];
                }
            }

            sendto(sock_fd, &res, sizeof(res), 0, (struct sockaddr *)&db.player0_address[gameId], sizeof(db.player0_address[gameId]));

            sendto(sock_fd, &res, sizeof(res), 0, (struct sockaddr *)&db.player1_address[gameId], sizeof(db.player1_address[gameId]));

        }

    }

    close(sock_fd);
    return 0;
}

    
