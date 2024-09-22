#include <cassert>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "Constants.h"
#include "Tile.h"
#include "Texture.h"
#include "Button.h"
#include "globals.h"

Button::Button(int x, int y)
    : m_coordinate{x,y}
{}

bool Button::setTexture( std::string path )
{

    return ( m_texture.loadFromFile( path.c_str() ) );
}

void Button::render()
{
    
    SDL_Rect renderTile;
    
    renderTile.w = Constants::TILE_RENDERED_SIZE * 2;
    renderTile.h = Constants::TILE_RENDERED_SIZE * 2;
    
    if(gWindow.isFullScreen() )
    {
        renderTile.x = m_coordinate.x + Constants::BOARD_OFFSET - Constants::TILE_RENDERED_SIZE;
        renderTile.y = m_coordinate.y + Constants::TILE_RENDERED_SIZE;
    }
    else
    {
        renderTile.x = m_coordinate.x - Constants::TILE_RENDERED_SIZE;
        renderTile.y = m_coordinate.y + Constants::TILE_RENDERED_SIZE;
    }

    // Render to screen
    SDL_RenderCopy( gRenderer, m_texture.getTexture(), NULL, &renderTile );

    
}

/*
void Button::handleEvent(SDL_Event* e)
{
    if(!fullscreen)
    {
        offset = 0;
    }

    // If mouse event happened
    if( e->type == SDL_MOUSEBUTTONDOWN )
    {
        // Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );

        // Check if mouse is in tile
        bool inside = true;

        // Mouse is left of the tile
        if( x < m_coordinate.x * Constants::TILE_RENDERED_SIZE + offset)
        {
            inside = false;
        }
        // Mouse is right of the tile
        else if( x > m_coordinate.x * Constants::TILE_RENDERED_SIZE + Constants::TILE_RENDERED_SIZE + offset)
        {
            inside = false;
        }
        // Mouse above the tile
        else if( y < m_coordinate.y * Constants::TILE_RENDERED_SIZE)
        {
            inside = false;
        }
        // Mouse below the tile
        else if( y > m_coordinate.y * Constants::TILE_RENDERED_SIZE + Constants::TILE_RENDERED_SIZE)
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
    
}
*/