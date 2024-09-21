#include "Board.h"
#include "Random.h"
#include "Constants.h"
#include <cassert>

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
            //m_Board[j].push_back(new NumberedTile{static_cast<int>(j) * Constants::TILE_RENDERED_SIZE, static_cast<int>(i) * Constants::TILE_RENDERED_SIZE});
            m_Board[j][i]->display();
        }
    }
};

void Board::randomizeBombs(Point2D point)
{
    int bombCount { 0 };
    while (bombCount < Constants::BOMBS_MAX)
    {
        int x{Random::get(0, Constants::BOARD_SIZE - 1)};
        int y{Random::get(0, Constants::BOARD_SIZE - 1)};    /*  
        std::cout << static_cast<std::size_t>(x) << " " << static_cast<std::size_t>(y) << '\n';
        bombCount++;*/
        if( (x >= point.x - 2 && x <= point.x + 2) && (y >= point.y - 2 && y <= point.y + 2)) // No bombs in a 3x3 area at the first click
            continue;
        
        if( !m_Board[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)]->isBomb())
        {
            m_Board[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)] = new Bomb{ x, y };
            incrementSurrounding( x, y );
            bombCount++;
        }
    }

    m_firstClick = false;
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
                //std::cout<<"called Illegalincrement\n";
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

void Board::reveal(Point2D point)
{
    std::cout << point.x << ", " << point.y << '\n';
    if(m_firstClick)
    {
        m_firstClick = false;
        randomizeBombs(Point2D {point.x, point.y});
        //Initialize all tile sprites
    }
    
    if( m_Board[point.x][point.y]->isBomb() )
    {
        m_Board[point.x][point.y]->setSprite(TILE_SPRITE_BOMB);
    }
    else
    {
        NumberedTile* tile{dynamic_cast<NumberedTile*>(m_Board[point.x][point.y])};
        int surrounding{ tile->getSurrounding() };
        switch(surrounding)
        {
        case 0:
            if(tile->getSprite() == TILE_SPRITE_REVEALED_0 || tile->getSprite() == TILE_SPRITE_MARKED)
                break;
            tile->setSprite(TILE_SPRITE_REVEALED_0);
            
            for ( int i = point.x - 1; i <= point.x + 1; ++i )
            {
                for (int j = point.y - 1; j <= point.y + 1; ++j )
                {
                    if ((i >= 0 && i < Constants::BOARD_SIZE && j >= 0 && j < Constants::BOARD_SIZE)) 
                    {
                        reveal(Point2D {i, j});
                    }
                }
            }
            break;
        case 1:
            tile->setSprite(TILE_SPRITE_REVEALED_1);
            break;
        case 2:
            tile->setSprite(TILE_SPRITE_REVEALED_2);
            break;
        case 3:
            tile->setSprite(TILE_SPRITE_REVEALED_3);
            break;
        case 4:
            tile->setSprite(TILE_SPRITE_REVEALED_4);
            break;
        case 5:
            tile->setSprite(TILE_SPRITE_REVEALED_5);
            break;
        case 6:
            tile->setSprite(TILE_SPRITE_REVEALED_6);
            break;
        case 7:
            tile->setSprite(TILE_SPRITE_REVEALED_7);
            break;
        case 8:
            tile->setSprite(TILE_SPRITE_REVEALED_8);
            break;
        default:
            assert("reveal() failed");
        }
    }
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

void Board::handleEvents(SDL_Event* e)
{
    for(std::size_t j = 0; j < Constants::BOARD_SIZE; j++)
    {
        for(std::size_t i = 0; i < Constants::BOARD_SIZE; i++)
        {
            std::optional<Tile*> tile = m_Board[j][i]->handleEvent(e);
            if( tile )
            {
                Point2D point = tile.value()->getCoordinate();
                reveal(point);
            }
        }
    }
}

void Board::renderTiles()
{
    for(std::size_t j = 0; j < Constants::BOARD_SIZE; j++)
    {
        for(std::size_t i = 0; i < Constants::BOARD_SIZE; i++)
        {
            m_Board[j][i]->render();
        }
    }
}
