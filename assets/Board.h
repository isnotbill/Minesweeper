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
#include "Texture.h"
#include "Tile.h"

class Board
{
public:  
    //Board constructor to specific size
    Board();


    ~Board()
    {
        for(std::size_t j = 0; j < Constants::BOARD_SIZE; j++)
        {
            for(std::size_t i = 0; i < Constants::BOARD_SIZE; i++)
            {
                delete m_Board[j][i];
                m_Board[j][i] = nullptr;
                //std::cout << m_Board[j][i];
            }
        }
    }

    Board(Board&& board)
    {
        for(std::size_t j = 0; j < Constants::BOARD_SIZE; j++)
        {
            for(std::size_t i = 0; i < Constants::BOARD_SIZE; i++)
            {
                m_Board[j][i] = board.getBoard()[j][i];
                //std::cout << m_Board[j][i];
            }
        }
    }

    Board& operator=(Board&& other) noexcept 
    {
        if (this != &other) {
            // Clean up existing resources
            for(std::size_t j = 0; j < Constants::BOARD_SIZE; j++) {
                for(std::size_t i = 0; i < Constants::BOARD_SIZE; i++) {
                    delete m_Board[j][i];
                    m_Board[j][i] = nullptr;
                }
            }

            // Transfer ownership from 'other' to 'this'
            for(std::size_t j = 0; j < Constants::BOARD_SIZE; j++) {
                for(std::size_t i = 0; i < Constants::BOARD_SIZE; i++) {
                    m_Board[j][i] = other.m_Board[j][i];
                    other.m_Board[j][i] = nullptr; // Prevent double deletion
                }
            }
        }
        m_tilesRevealed = 0;
        m_firstClick = true;
        m_gameOver = false;
        return *this;
    }
    
    std::vector< std::vector<Tile*> > getBoard() const {return m_Board;}
    bool getGameOver() const {return m_gameOver;}

    //Creating board
    void randomizeBombs(Point2D point);
    void incrementSurrounding( int x , int y);

    //Check if bomb
    bool illegalIncrement(int x, int y);

    //Reveal a clicked tile
    void reveal(Point2D);
    
    void showBombs();

    void handleEvents(SDL_Event* e, int, bool);

    void renderTiles();

    bool win();

private:
    //2D array of board size
    std::vector< std::vector<Tile*> > m_Board;
    bool m_firstClick{true}; // False if the player already clicked before hand.
    bool m_gameOver{false};
    int m_tilesRevealed{0};
    int m_currentTilesRevealed{0};
};

#endif
