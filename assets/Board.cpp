#include "Board.h"
#include "Random.h"
#include "Constants.h"
#include "Tile.h"
#include <cassert>
#include "../soundeffects/soundeffect.h"

Board::Board()
        : m_Board{std::vector< std::vector<Tile*> >(Constants::BOARD_SIZE)}
{
    for(std::size_t j = 0; j < Constants::BOARD_SIZE; j++)
    {
        for(std::size_t i = 0; i < Constants::BOARD_SIZE; i++)
        {
            m_Board[j].push_back(new NumberedTile{static_cast<int>(j), static_cast<int>(i)});
            //m_Board[j].push_back(new NumberedTile{static_cast<int>(j) * Constants::TILE_RENDERED_SIZE, static_cast<int>(i) * Constants::TILE_RENDERED_SIZE});
            //m_Board[j][i]->display();
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

    if(m_firstClick)
    {
        m_firstClick = false;
        randomizeBombs(Point2D {point.x, point.y});
        //Initialize all tile sprites
    }

    if (m_Board[point.x][point.y]->getRevealed())
    {
        return; //If the tile is already revealed, return early.
    }

    if( m_Board[point.x][point.y]->isBomb() )
    {
        m_Board[point.x][point.y]->setSprite(TILE_SPRITE_BOMB_EXPLODED);
        Mix_PlayChannel(-1, gBombSound, 0);
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
            tile->setRevealed(true);
            ++m_tilesRevealed;
            ++m_currentTilesRevealed;

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
            tile->setRevealed(true);
            tile->setSprite(TILE_SPRITE_REVEALED_1);
            m_tilesRevealed += 1;
            ++m_currentTilesRevealed;
            break;
        case 2:
            tile->setRevealed(true);
            tile->setSprite(TILE_SPRITE_REVEALED_2);
            m_tilesRevealed += 1;
            ++m_currentTilesRevealed;
            break;
        case 3:
            tile->setRevealed(true);
            tile->setSprite(TILE_SPRITE_REVEALED_3);
            m_tilesRevealed += 1;
            ++m_currentTilesRevealed;
            break;
        case 4:
            tile->setRevealed(true);
            tile->setSprite(TILE_SPRITE_REVEALED_4);
            m_tilesRevealed += 1;
            ++m_currentTilesRevealed;
            break;
        case 5:
            tile->setRevealed(true);
            tile->setSprite(TILE_SPRITE_REVEALED_5);
            m_tilesRevealed += 1;
            ++m_currentTilesRevealed;
            break;
        case 6:
            tile->setRevealed(true);
            tile->setSprite(TILE_SPRITE_REVEALED_6);
            m_tilesRevealed += 1;
            ++m_currentTilesRevealed;
            break;
        case 7:
            tile->setRevealed(true);
            tile->setSprite(TILE_SPRITE_REVEALED_7);
            m_tilesRevealed += 1;
            ++m_currentTilesRevealed;
            break;
        case 8:
            tile->setRevealed(true);
            tile->setSprite(TILE_SPRITE_REVEALED_8);
            m_tilesRevealed += 1;
            ++m_currentTilesRevealed;
            break;
        default:
            assert("reveal() failed");
        }
    }
}

void Board::showBombs()
{
    for(std::size_t j = 0; j < Constants::BOARD_SIZE; j++)
    {
        for(std::size_t i = 0; i < Constants::BOARD_SIZE; i++)
        {
            if(m_Board[j][i]->isBomb() && m_Board[j][i]->getSprite() != TILE_SPRITE_BOMB_EXPLODED)
            {
                m_Board[j][i]->setSprite(TILE_SPRITE_BOMB);
            }
        }
    }
}


void Board::handleEvents(SDL_Event* e, int offset, bool fullscreen)
{
    for(std::size_t j = 0; j < Constants::BOARD_SIZE; j++)
    {
        for(std::size_t i = 0; i < Constants::BOARD_SIZE; i++)
        {
            std::optional<Tile*> tile = m_Board[j][i]->handleEvent(e, offset, fullscreen);
            if( tile )
            {
                if(tile.value()->isBomb())
                {
                    m_gameOver = true;
                }
                Point2D point = tile.value()->getCoordinate();

                // Reset the counter before revealing tiles
                m_currentTilesRevealed = 0;

                reveal(point);
                std::cout << m_currentTilesRevealed << std::endl;
                // Decide which sound to play
                if (m_currentTilesRevealed == 1)
                {
                    Mix_PlayChannel(-1, gClickSound, 0);
                    std::cout << "Played Click\n";
                }
                else if (m_currentTilesRevealed > 1)
                {
                    Mix_PlayChannel(-1, gRevealSound, 0);
                    std::cout << "Played Revealed\n";
                }
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

bool Board::win()
{
    if(m_tilesRevealed == Constants::TOTAL_TILES - Constants::BOMBS_MAX)
    {
        return true;
    }
    return false;
}
