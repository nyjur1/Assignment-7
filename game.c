#include "game.h"
#include <stdlib.h>
#include <stddef.h>

/*
Takes as input a pointer to a connect4 game board, a player (0/1), and a int column.
Places a new piece in `col` on the game board.
*/
int make_move(struct game* g, int player, int col) {
    if(player<0 || player>1){
        return -1; 
    }

    if(col<0 || col>6){
        return -1;
    }

    if(player!=g->currPlayer){
        return -1;
    }

    int row =5;

    while(row>=0){
        if(g->board.cells[row][col]==-1){
            g->board.cells[row][col]= player;
            break; 

        }
        row--; 
    }
    if(row==-1){
        return -1; 
    }

    if(player==0){
        g->currPlayer=1;
    }
    else{
        g->currPlayer=0;
    }
    return 0; 

}
 
/*
Takes as input a pointer to a connect4 game board.
Returns 0 or 1 if player 0 or 1 has won respectively.
Returns -1 if there is no winner.
*/
int check_win(struct game* g) {
    for(int row =0; row<6; row++){
        for(int col=0; col<=3; col++){
            int player = g->board.cells[row][col];

            if(player!= -1&&
            g->board.cells[row][col+1]==player &&
            g->board.cells[row][col+2]==player &&
            g->board.cells[row][col+3]==player){

            return player; 
            }
        }
    }

    for(int row =0; row<=2; row++){
        for(int col=0; col<7; col++){
            int player = g->board.cells[row][col];

            if(player!= -1&&
            g->board.cells[row+1][col]==player &&
            g->board.cells[row+2][col]==player &&
            g->board.cells[row+3][col]==player){

            return player; 
            }
        }
    }

    for(int row =0; row<=2; row++){
        for(int col=0; col<=3; col++){
            int player = g->board.cells[row][col];

            if(player!= -1&&
            g->board.cells[row+1][col+1]==player &&
            g->board.cells[row+2][col+2]==player &&
            g->board.cells[row+3][col+3]==player){

            return player; 
            }
        }
    }

     for(int row =0; row<=2; row++){
        for(int col=3; col<7; col++){
            int player = g->board.cells[row][col];

            if(player!= -1&&
            g->board.cells[row+1][col-1]==player &&
            g->board.cells[row+2][col-2]==player &&
            g->board.cells[row+3][col-3]==player){

            return player; 
            }
        }
    }

    return -1;

}


 
/*
Initializes an existing game with an empty board.
*/
void new_game(struct game* g, int gameId) {
    g->gameId= gameId;
    g->currPlayer=0; 
    for(int row =0; row<6; row++){
        for(int column=0; column<7; column++){
            g->board.cells[row][column]=-1; 
        }
    }
   
}


