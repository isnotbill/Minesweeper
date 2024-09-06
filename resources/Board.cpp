
#include "Board.h"
#include "Random.h"

namespace Constants
{
    int boardSize = 16;
}
Board::Board()
        : m_Board{std::vector< std::vector<Tile*> >(Constants::boardSize)}
    {
        for(int j = 0; j < Constants::boardSize; j++)
        {
            for(int i = 0; i < Constants::boardSize; i++)
            {
                m_Board[static_cast<std::size_t>(j)].push_back(new NumberedTile{j, i});
                m_Board[static_cast<std::size_t>(j)][static_cast<std::size_t>(i)]->display();
            }
        }
    };

void Board::randomizeBombs()
{
    for(int j = 0; j < Constants::boardSize; j++)
    {
        for(int i = 0; i < Constants::boardSize; i++)
        {
            m_Board[static_cast<std::size_t>(j)].push_back(new Bomb{j, i});
            m_Board[static_cast<std::size_t>(j)][static_cast<std::size_t>(i)]->display();
        }
    }
}
