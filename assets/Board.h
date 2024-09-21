#ifndef BOARD_H
#define BOARD_H

#include <cassert>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "Constants.h"
#include "Tile.h"

class Board
{
public:  
    //Board constructor to specific size
    Board();

    //Creating board
    void randomizeBombs(Point2D point);
    void incrementSurrounding( int x , int y);

    //Check if bomb
    bool illegalIncrement(int x, int y);

    //Reveal a clicked tile
    void reveal(Point2D);

    void display();

    void handleEvents(SDL_Event* e);

    void renderTiles();

private:
    //2D array of board size
    std::vector< std::vector<Tile*> > m_Board;
    bool m_firstClick{true}; // False if the player already clicked before hand.
    bool m_gameOver{false};
};

#endif
