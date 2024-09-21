#include "Tile.h"
#include "Texture.h"
#include "Constants.h"
#include <iostream>
#include <optional>

// Bomb class definitions
bool Bomb::isBomb()
{
    return true;
};

void Bomb::display()
{
    std::cout << m_coordinate << " is a Bomb.\n";
}

// NumberedTile class definitions
int NumberedTile::getSurrounding() const // gets m_bombSurrounding
{
    return m_bombSurrounding;
}

bool NumberedTile::isBomb()
{
    return false;
};

void NumberedTile::display()
{
    std::cout << m_coordinate << "with " << m_bombSurrounding << " bombs surrounding" << " is a NumberedTile.\n";
};

void NumberedTile::increment()
{
    m_bombSurrounding = m_bombSurrounding + 1;
}

void Tile::setPosition( int x, int y )
{
    m_coordinate.x = x;
    m_coordinate.y = y;
}

std::optional<Tile*> Tile::handleEvent( SDL_Event* e )
{
    // If mouse event happened
    if( e->type == SDL_MOUSEBUTTONDOWN )
    {
        // Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );

        // Check if mouse is in tile
        bool inside = true;

        // Mouse is left of the tile
        if( x < m_coordinate.x * Constants::TILE_RENDERED_SIZE )
        {
            inside = false;
        }
        // Mouse is right of the tile
        else if( x > m_coordinate.x * Constants::TILE_RENDERED_SIZE + Constants::TILE_RENDERED_SIZE )
        {
            inside = false;
        }
        // Mouse above the tile
        else if( y < m_coordinate.y * Constants::TILE_RENDERED_SIZE)
        {
            inside = false;
        }
        // Mouse below the tile
        else if( y > m_coordinate.y * Constants::TILE_RENDERED_SIZE + Constants::TILE_RENDERED_SIZE )
        {
            inside = false;
        }

        
        // Mouse is inside tile
        if( inside )
        {
            // For simplicity, toggle between hidden and revealed
            if( m_CurrentSprite == TILE_SPRITE_HIDDEN )
            {
                if( e->button.button == SDL_BUTTON_LEFT )
                {
                    // You can set the revealed sprite based on your game logic
                    //m_CurrentSprite = TILE_SPRITE_REVEALED_0;
                    //Board::reveal(location )
                    //-> TILE_SPRITE_REVEALED_0; // Example: revealed with 0 adjacent mines
                    // return true;
                    // return *this;
                    std::cout<< "Returning this";
                    return this;
                }
                if( e->button.button == SDL_BUTTON_RIGHT )
                {
                    m_CurrentSprite = TILE_SPRITE_MARKED;                    
                }
            }
            
            else if(m_CurrentSprite == TILE_SPRITE_MARKED)
            {
                if( e->button.button == SDL_BUTTON_RIGHT )
                {
                    m_CurrentSprite = TILE_SPRITE_HIDDEN;                    
                }
            }
            
        }

    }
    return std::nullopt;
}

void Tile::setSprite( TileSprite sprite )
{
    m_CurrentSprite = sprite;
}
