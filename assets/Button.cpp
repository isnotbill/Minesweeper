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


Button::Button(int x, int y, int w, int h)
    : m_coordinate{x,y}
    , m_width {w}
    , m_height {h}
{}

bool Button::setTexture( std::string path )
{

    return ( m_texture.loadFromFile( path.c_str() ) );
}

void Button::render()
{
    
    SDL_Rect renderTile;
    
    renderTile.w = m_width;
    renderTile.h = m_height;
    
    if(gWindow.isFullScreen() )
    {
        renderTile.x = m_coordinate.x + Constants::BOARD_OFFSET;
        renderTile.y = m_coordinate.y;
    }
    else
    {
        renderTile.x = m_coordinate.x;
        renderTile.y = m_coordinate.y;
    }

    // Render to screen
    SDL_RenderCopy( gRenderer, m_texture.getTexture(), NULL, &renderTile );
    
}


int Button::handleEvent(SDL_Event* e)
{
    int offset = Constants::BOARD_OFFSET;
    if(!gWindow.isFullScreen())
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
        if( x < m_coordinate.x + offset)
        {
            inside = false;
        }
        // Mouse is right of the tile
        else if( x > m_coordinate.x + m_width + offset)
        {
            inside = false;
        }
        // Mouse above the tile
        else if( y < m_coordinate.y)
        {
            inside = false;
        }
        // Mouse below the tile
        else if( y > m_coordinate.y + m_width)
        {
            inside = false;
        }
        // Mouse is inside tile
        if( inside )
        {
            // For simplicity, toggle between hidden and revealed
            if(e->button.button == SDL_BUTTON_LEFT)
            {
                std::cout << "Left clicked" << '\n';
                return SDL_BUTTON_LEFT;
            }
            
        }
    }
    return -1;
    
}
