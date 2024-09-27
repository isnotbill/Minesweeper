#include "Board.h"
#include "Random.h"
#include "Constants.h"
#include "Tile.h"
#include <cassert>
#include "../soundeffects/soundeffect.h"

// Default constructor, create a 16x16 board
Board::Board()
        : m_Board{std::vector< std::vector<Tile*> >(Constants::BOARD_SIZE)}
{
    //Fill vector with dynamically allocated NumberedTile objects
    for(std::size_t j = 0; j < Constants::BOARD_SIZE; j++)
    {
        for(std::size_t i = 0; i < Constants::BOARD_SIZE; i++)
        {
            m_Board[j].push_back(new NumberedTile{static_cast<int>(j), static_cast<int>(i)});
        }
    }
};

// Randomize bomb placement, guarantees a 3x3 area without bombs on first clicked spot
void Board::randomizeBombs(Point2D point)
{
    int bombCount { 0 };

    // Place 40 bombs randomly using random algorithm in "Random.h"
    while (bombCount < Constants::BOMBS_MAX)
    {
        int x{Random::get(0, Constants::BOARD_SIZE - 1)};
        int y{Random::get(0, Constants::BOARD_SIZE - 1)};

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

//Assign the amount of bombs surrounding a NumberedTile
void Board::incrementSurrounding(int x, int y)
{
    //Check all bombs and increment the tiles surrounding them
    for (int i = x - 1; i <= x + 1; ++i )
    {
        for (int j = y - 1; j <= y + 1; ++j )
        {
            if( i == x && j == y )
                continue;

            if( !outOfBounds( Point2D{i, j} ) )
            {
                // If within bounds, check if the tile is a bomb
                if (m_Board[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)]->isBomb()) {
                    continue;
                }
        
                NumberedTile* tile{dynamic_cast<NumberedTile*>(m_Board[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)])};
                ++(*tile);
            }
        }
    }

    
}

// Check if a point is out of bounds
bool Board::outOfBounds(Point2D point) 
{
    if (point.x < 0 || point.x >= Constants::BOARD_SIZE || point.y < 0 || point.y >= Constants::BOARD_SIZE) {
        return true;
    }

    return false;
}

// Number of flags that surround a tile
int Board::countFlags(Point2D point)
{
    int count = 0;

    for (int i = point.x - 1; i <= point.x + 1; ++i )
    {
        for (int j = point.y - 1; j <= point.y + 1; ++j )
        {
            if ( outOfBounds(Point2D{i, j}) )
                continue;

            if( i == point.x && j == point.y )
                continue;
            
            if( m_Board[i][j]->getSprite() == TILE_SPRITE_MARKED )
            {
                ++count;
            }
        }
    }

    return count;
}

// Reveal a tile and its surrounding tiles
void Board::reveal(Point2D point)
{

    if( outOfBounds(point) )
        return;

    // Initialize all bombs
    if(m_firstClick) 
    {
        m_firstClick = false;
        randomizeBombs(Point2D {point.x, point.y});
    }

    // If the tile is already revealed, return early.
    if (m_Board[point.x][point.y]->getRevealed() || m_Board[point.x][point.y]->getSprite() == TILE_SPRITE_MARKED )
    {
        return; 
    }

    // If the revealed tile is a bomb, game is over
    if( m_Board[point.x][point.y]->isBomb() )
    {
        m_Board[point.x][point.y]->setSprite(TILE_SPRITE_BOMB_EXPLODED);
        Mix_PlayChannel(-1, gBombSound, 0);
        m_gameOver = true;
    }

    else
    {
        NumberedTile* tile{dynamic_cast<NumberedTile*>(m_Board[point.x][point.y])};
        int surrounding{ tile->getSurrounding() };

        //If the tile revealed has no bombs around it, reveal the surrounding tiles
        switch(surrounding)
        {
        case 0:
            if(tile->getSprite() == TILE_SPRITE_REVEALED_0)
                break;
                
            tile->setSprite(TILE_SPRITE_REVEALED_0);
            tile->setRevealed(true);
            ++m_tilesRevealed;
            ++m_currentTilesRevealed;

            for ( int i = point.x - 1; i <= point.x + 1; ++i )
            {
                for (int j = point.y - 1; j <= point.y + 1; ++j )
                {
                    reveal(Point2D {i, j}); //Recurse
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

// Reveal all the bombs on loss
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

// Handle events of all tiles
void Board::handleEvents(SDL_Event* e)
{
    for(std::size_t j = 0; j < Constants::BOARD_SIZE; j++)
    {
        for(std::size_t i = 0; i < Constants::BOARD_SIZE; i++)
        {
            std::optional<Tile*> tile = m_Board[j][i]->handleEvent(e);
            if( tile )
            {
                if( e->button.button == SDL_BUTTON_LEFT )  // Handle left click event, reveals a tile
                {
                    Point2D point = tile.value()->getCoordinate();

                    // Reset the counter before revealing tiles
                    m_currentTilesRevealed = 0;

                    reveal(point);

                    // Decide which sound to play
                    if (m_currentTilesRevealed == 1)
                    {
                        Mix_PlayChannel(-1, gClickSound, 0);
                    }
                    else if (m_currentTilesRevealed > 1)
                    {
                        Mix_PlayChannel(-1, gRevealSound, 0);
                    }
                }
                else if( e->button.button == SDL_BUTTON_MIDDLE ) // Handle middle click event, reveals all surrounding tiles that aren't flags
                {
                    NumberedTile* numbered = dynamic_cast<NumberedTile*>(m_Board[j][i]);
                    if ( numbered )
                    {
                        Point2D coord = numbered->getCoordinate();
                        if( numbered->getSurrounding() == countFlags( numbered->getCoordinate() ) )
                        {
                            for (int i = coord.x - 1; i <= coord.x + 1; ++i )
                            {
                                for (int j = coord.y - 1; j <= coord.y + 1; ++j )
                                {
                                    if( i == coord.x && j == coord.y )
                                        continue;
                                    
                                    reveal(Point2D{i, j});
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// Render tiles to window
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

// Check for win
bool Board::win()
{
    if(m_tilesRevealed == Constants::TOTAL_TILES - Constants::BOMBS_MAX)
    {
        m_gameOver = true;
        return true;
    }
    return false;
}
