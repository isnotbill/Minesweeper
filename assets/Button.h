#ifndef BUTTON_H
#define BUTTON_H

#include <cassert>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "Constants.h"
#include "Tile.h"
#include "Texture.h"

class Button
{
public:
    Button() = default;
    Button(int x, int y);

    Texture getTexture() const { return m_texture; }
    Point2D getPoint() const { return m_coordinate; }

    bool setTexture( std::string path );

    void render();

    void handleEvent(SDL_Event*);

private:
    Texture m_texture;
    Point2D m_coordinate;
};

#endif