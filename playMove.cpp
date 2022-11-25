//
//  playMove.cpp
//  TicTacToe
//
//  Created by Tarek Abdelrahman on 2019-06-07.
//  Modified by Tarek Abdelrahman on 2020-09-17.
//  Copyright © 2019-2020 Tarek Abdelrahman. All rights reserved.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//
//  ECE244 Student: write your code for playMove in this file

#include "globals.h"
#include "GameState.h"


void playMove(GameState& game_state) {
    int row;
    int col; //easier to type 
    row=game_state.get_selectedRow();
    col=game_state.get_selectedColumn();  //get the input row, col
    
    //range of row&col has been checked in tictactoe.cpp
    //when the input location is empty
    //the move is valid
    int value; //value to place on the board X or O
    if(game_state.get_gameBoard(row,col)==Empty){
        game_state.set_moveValid(true);
        //the moveValid is true
        //check turn, get turn, and change the turn
        //set the value to the board
        if(game_state.get_turn()) //if true,its X turn
        {    value=X;
             game_state.set_turn(false);  //if it is X, turn change to O
        }
        else //if false, its O turn
        {   value=O;
            game_state.set_turn(true);  //if value!=X, change to X
        }
        game_state.set_gameBoard(row,col,value); //set the value to the board
       
        
    }
    else 
        game_state.set_moveValid(false);
    
    
    //check if there is a win in the board after the move
    //if yes, set the winCode 1- 8
    //gameOver is true
    //if no, winCode=0
    //while two wins, print the higher win code
    if((game_state.get_gameBoard(2,0)==game_state.get_gameBoard(1,1))&&(game_state.get_gameBoard(1,1)==game_state.get_gameBoard(0,2))
            &&game_state.get_gameBoard(2,0)!=Empty){
        game_state.set_winCode(8);
        game_state.set_gameOver(true);
    } 
    else if((game_state.get_gameBoard(0,0)==game_state.get_gameBoard(1,1))&&(game_state.get_gameBoard(1,1)==game_state.get_gameBoard(2,2))
            &&game_state.get_gameBoard(0,0)!=Empty){
        game_state.set_winCode(7);
        game_state.set_gameOver(true);
    }
    else if((game_state.get_gameBoard(0,2)==game_state.get_gameBoard(1,2))&&(game_state.get_gameBoard(1,2)==game_state.get_gameBoard(2,2))
            &&game_state.get_gameBoard(0,2)!=Empty){
        game_state.set_winCode(6);
        game_state.set_gameOver(true);
    } 
    
    else if((game_state.get_gameBoard(0,1)==game_state.get_gameBoard(1,1))&&(game_state.get_gameBoard(1,1)==game_state.get_gameBoard(2,1))
            &&game_state.get_gameBoard(0,1)!=Empty){
        game_state.set_winCode(5);
        game_state.set_gameOver(true);
    }  
    
    else if((game_state.get_gameBoard(0,0)==game_state.get_gameBoard(1,0))&&(game_state.get_gameBoard(1,0)==game_state.get_gameBoard(2,0))
            &&game_state.get_gameBoard(0,0)!=Empty){
        game_state.set_winCode(4);
        game_state.set_gameOver(true);
    }    
    
    else if((game_state.get_gameBoard(2,0)==game_state.get_gameBoard(2,1))&&(game_state.get_gameBoard(2,1)==game_state.get_gameBoard(2,2))
            &&game_state.get_gameBoard(2,0)!=Empty){
        game_state.set_winCode(3);
        game_state.set_gameOver(true);
    }   
    
    else if((game_state.get_gameBoard(1,0)==game_state.get_gameBoard(1,1))&&(game_state.get_gameBoard(1,1)==game_state.get_gameBoard(1,2))
            &&game_state.get_gameBoard(1,0)!=Empty){
        game_state.set_winCode(2);
        game_state.set_gameOver(true);
    }    
    
    else if((game_state.get_gameBoard(0,0)==game_state.get_gameBoard(0,1))&&(game_state.get_gameBoard(0,1)==game_state.get_gameBoard(0,2))
            &&game_state.get_gameBoard(0,0)!=Empty){
        game_state.set_winCode(1);
        game_state.set_gameOver(true);
    }    
    
    else{
        game_state.set_winCode(0); //no line has made
        game_state.set_gameOver(true); //assume this is a draw and the game ends
    }                                               
    
    
    
    //when winCode is 0
    //no draw, game continues if there is empty space in the grid
    if(game_state.get_winCode()==0){
        for(int i = 0; i < boardSize; i++ ){
            for(int j = 0; j < boardSize; j++ ){
                if (game_state.get_gameBoard(i,j)== Empty){
                    game_state.set_gameOver(false);  //once it sees empty space,game continues
                }
            }
        }
    }  
}

