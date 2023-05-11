//
//  Othello.hpp
//  OthelloAssignment
//
//  Created by Najmuddin Nazary on 2023-03-23.
//

#ifndef Othello_hpp
#define Othello_hpp
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;



#include <stdio.h>
// -------------------------------------------------Position .h----------------------------------------------------

class Position {
    char piece;
public:
    virtual bool canPlay() const = 0;
    static const char EMPTY = ' ';
    static const char BLACK = 'B';
    static const char WHITE = 'W';
    void setPiece(char newPiece) {
        piece = newPiece;
    }
    char getPiece() const {
        return piece;
    }

};
/* ------------------------------------------------UnplayablePosition .h----------------------------------------------------*/

class UnplayablePosition : public Position {
public:
    static const char UNPLAYABLE = '*';

    bool canPlay() const override {
        return false;
    }
};


/* -------------------------------------------PlayablePosition .h--------------------------------------------------------------*/

class PlayablePosition : public Position {
public:
    bool canPlay() const override {
        return true;
    }
};
/*-----------------------------------------------------Player .h---------------------------------------------------------*/

class Player {
    string name;
    
    
public:
    Player(const string& name) : name(name) {}

    string getName() const {
        return name;
    }
};

/* -------------------------------------------------Board.h--------------------------------------------------------------*/
class Player;
class Game;

class Board {
private:
    vector<vector<Position*>> positions;

public:
    const vector<vector<Position*>>& getPositions() const;





    bool loadMapFromFile(const string &filename);




    Board() ;
    Board(const string& boardState);



    bool validMoves(char playerPiece) const;

    void drawBoard() const ;

    void takeTurn(Game& game, Player& current);


    pair<int, int> counter() const {
        int blackPieces = 0, whitePieces = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (positions[i][j]->getPiece() == Position::BLACK) {
                    blackPieces++;
                } else if (positions[i][j]->getPiece() == Position::WHITE) {
                    whitePieces++;
                }
            }
        }
        return make_pair(blackPieces, whitePieces);
    }


    bool isInBounds(int row, int col) {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }
    void flipPieces(int row, int col, char playerPiece) ;


};

/* -------------------------------------------------Game .h--------------------------------------------------------------------*/


class Game {
private:
    Board board;
    Player first;
    Player second;
    Player* current;
    
    void save() ;
    
public:

    Game(const Player& p1, const Player& p2, const Board& customboard) : first(p1), second(p2), current(&first), board(customboard) {}

    
    Game(const Player& p1, const Player& p2) : first(p1), second(p2) {}
        
    void start();
        
    bool gameOver() ;
    
    void switchTurns() {
        current = (current == &first) ? &second : &first;
    }
    
    static Game load(const string& filename);

    
    void play();
    
    
    const Player& getFirstPlayer() const {
           return first;
       }
       
       const Board& getBoard() const {
           return board;
       }
};







#endif /* Othello_hpp */
