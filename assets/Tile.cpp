#include "Tile.h"
#include "Texture.h"
#include "Constants.h"
#include <iostream>
#include <optional>
#include "../soundeffects/soundeffect.h"
#include "globals.h"

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
        if( x < m_coordinate.x * gTileSize + gOffset)
        {
            inside = false;
        }
        // Mouse is right of the tile
        else if( x > m_coordinate.x * gTileSize + gTileSize + gOffset)
        {
            inside = false;
        }
        // Mouse above the tile
        else if( y < m_coordinate.y * gTileSize)
        {
            inside = false;
        }
        // Mouse below the tile
        else if( y > m_coordinate.y * gTileSize + gTileSize)
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
                    return this;
                }
                if( e->button.button == SDL_BUTTON_RIGHT )
                {
                    m_CurrentSprite = TILE_SPRITE_MARKED;
                    Mix_PlayChannel( -1, gFlagSound, 0 );                    
                }
            }
            
            else if(m_CurrentSprite == TILE_SPRITE_MARKED)
            {
                if( e->button.button == SDL_BUTTON_RIGHT )
                {
                    m_CurrentSprite = TILE_SPRITE_HIDDEN;
                    Mix_PlayChannel( -1, gFlagSound, 0 );                    
                }
            }
            
        }

    }
    return std::nullopt;
}

void Tile::render()
{
    // Show current tile sprite
    gTileSpriteSheetTexture.render( m_coordinate.x * gTileSize + gOffset, m_coordinate.y * gTileSize, gTileSize, gTileSize,&gSpriteClips[ m_CurrentSprite ] );

}

void Tile::setSprite( TileSprite sprite )
{
    m_CurrentSprite = sprite;
}
