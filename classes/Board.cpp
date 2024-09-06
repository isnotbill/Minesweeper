
#include "Board.h"
#include "Random.h"

namespace Constants
{
    constexpr int BOARD_SIZE = 16;
    constexpr int BOMBS_MAX = 40;
}

Board::Board()
        : m_Board{std::vector< std::vector<Tile*> >(Constants::BOARD_SIZE)}
    {
        for(std::size_t j = 0; j < Constants::BOARD_SIZE; j++)
        {
            for(std::size_t i = 0; i < Constants::BOARD_SIZE; i++)
            {
                m_Board[j].push_back(new NumberedTile{static_cast<int>(j), static_cast<int>(i)});
                m_Board[j][i]->display();
            }
        }
    };

void Board::randomizeBombs()
{
    int bombCount { 0 };
    while (bombCount < Constants::BOMBS_MAX)
    {
        int x{Random::get(0, Constants::BOARD_SIZE - 1)};
        int y{Random::get(0, Constants::BOARD_SIZE - 1)};    /*  
        std::cout << static_cast<std::size_t>(x) << " " << static_cast<std::size_t>(y) << '\n';
        bombCount++;*/
        
        if( !m_Board[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)]->isBomb() )
        {
            m_Board[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)] = new Bomb{ x, y };
            incrementSurrounding( x, y );
            bombCount++;
        }
        
        
        
        
    }

}

void Board::incrementSurrounding(int x, int y)
{
    for (int i = x - 1; i <= x + 1; ++i )
    {
        for (int j = y - 1; j <= y + 1; ++j )
        {
            if( i == x && j == y )
                continue;
            if(!illegalIncrement( i, j ) )
            {
                std::cout<<"called Illegalincrement\n";
                NumberedTile* tile{dynamic_cast<NumberedTile*>(m_Board[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)])};
                tile->increment();
            }
        }
    }

    
}

bool Board::illegalIncrement(int x, int y) {
    // Check if x or y is out of bounds
    if (x < 0 || x >= Constants::BOARD_SIZE || y < 0 || y >= Constants::BOARD_SIZE) {
        return true;  // Out of bounds, cannot increment
    }

    // If within bounds, check if the tile is a bomb
    if (m_Board[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)]->isBomb()) {
        return true;  // It's a bomb, don't increment
    }

    return false;  // Tile is valid for incrementing
}

void Board::display()
{
    for(std::size_t j = 0; j < Constants::BOARD_SIZE; j++)
    {
        for(std::size_t i = 0; i < Constants::BOARD_SIZE; i++)
        {
            if(m_Board[j][i]->isBomb())
                std::cout << " b ";
            else
            {
                NumberedTile* tile{dynamic_cast<NumberedTile*>(m_Board[j][i])};
                std::cout << " " << tile->getSurrounding() << " ";                
            }
        }
        std::cout << '\n';
    }
}