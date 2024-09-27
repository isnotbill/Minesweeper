#ifndef BUTTON_H
#define BUTTON_H

#include <cassert>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <optional>

#include "Constants.h"
#include "Tile.h"
#include "Texture.h"

class Button
{
public:
    // Default constructor
    Button() = default;
    // Overloaded constructor
    Button(int x, int y, int w, int h);

    // Assessors
    Texture getTexture() const { return m_texture; }
    Point2D getPoint() const { return m_coordinate; }

    // Mutators
    bool setTexture( std::string path );

    // Render the button
    void render(int x, int y, int w, int h);

    // Free texture
    void free();

    // Handle mouse events
    std::optional<int> handleEvent(SDL_Event*);

private:
    int m_width; // Width in pixels of the button
    int m_height; // Height in pixels of the button
    Texture m_texture; // Texture of the button
    Point2D m_coordinate; // Position of the button
};

#endif
