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

//Board class represents the Minesweeper board
class Board
{
public:  
    // Default constructor
    Board();

    // Destructor, deallocates vector
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

    // Move constructor
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

    // Move assignment
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
    
    // Accessors
    std::vector< std::vector<Tile*> > getBoard() const {return m_Board;}
    bool getGameOver() const {return m_gameOver;}

    // Creating board
    void randomizeBombs(Point2D point);
    void incrementSurrounding( int x , int y);

    // Validity checking
    bool outOfBounds(Point2D);
    int countFlags(Point2D);

    // Revealing tiles
    void reveal(Point2D);
    void showBombs();

    // SDL2 Implementation
    void handleEvents(SDL_Event* e);
    void renderTiles();

    // Win checking
    bool win();

private:
    std::vector< std::vector<Tile*> > m_Board; // 2D array of board size
    bool m_firstClick{true}; // False if the player already clicked before hand.
    bool m_gameOver{false}; // Game over flag
    int m_tilesRevealed{0}; // Number of tiles revealed
    int m_currentTilesRevealed{0}; // Numbered of tiles revealed in a single sequence
};

#endif
