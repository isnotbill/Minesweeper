#include <vector>
#include "Tile.h"

#ifndef BOARD_H
#define BOARD_H

class Board
{
public:  
    //Board constructor to specific size
    Board();

    //Creating board
    void randomizeBombs();
    void incrementSurrounding( int x , int y);

    //Check if bomb
    bool illegalIncrement(int x, int y);

    //Reveal a clicked tile
    bool reveal();

    void display();

private:
    //2D array of board size
    std::vector< std::vector<Tile*> > m_Board;
};

#endif