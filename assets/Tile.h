#ifndef TILE_H
#define TILE_H

#include <cassert>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "Constants.h"

// Initialize bombs first and then numbered tiles.

struct Point2D
{
    int x;
    int y;

    friend std::ostream& operator<< (std::ostream& os, const Point2D& point)
    {
        os << "(" << point.x << ", " << point.y << ")";
        return os;
    }
};

// Tile constants


enum TileSprite
{
    TILE_SPRITE_HIDDEN = 0,
    TILE_SPRITE_REVEALED_0,
    TILE_SPRITE_REVEALED_1,
    TILE_SPRITE_REVEALED_2,
    TILE_SPRITE_REVEALED_3,
    TILE_SPRITE_REVEALED_4,
    TILE_SPRITE_REVEALED_5,
    TILE_SPRITE_REVEALED_6,
    TILE_SPRITE_REVEALED_7,
    TILE_SPRITE_REVEALED_8,
    TILE_SPRITE_MARKED,
    TILE_SPRITE_BOMB,
    TILE_SPRITE_TOTAL
};


class Tile // Abstract class
{
public:
    Tile(int x, int y)
        : m_coordinate { x, y }
        , m_CurrentSprite {TILE_SPRITE_HIDDEN}
    {
        
    }
    virtual ~Tile() = default;
    virtual bool isBomb() = 0;
    virtual void display() = 0;
    Point2D getCoordinate() const {return m_coordinate;}

    //SDL2 Functions
    void setPosition(int x, int y);

    // Handles mouse event
    void handleEvent( SDL_Event* e );

    // Shows tile sprite
    void render();

    // Sets the current sprite
    void setSprite( TileSprite sprite );


protected:
    Point2D m_coordinate;

    TileSprite m_CurrentSprite;
    SDL_Point mPosition;

};

class Bomb : public Tile 
{
public:
    Bomb(int x, int y)
        : Tile{x,y}
    {}

    virtual ~Bomb() = default;
    bool isBomb() override;
    void display() override;
    
private:
};

class NumberedTile : public Tile
{
public:
    NumberedTile(int x, int y)
        : Tile{x,y}
    {
        
    };
    virtual ~NumberedTile() = default;

    int getSurrounding() const;

    bool isBomb() override;

    void display() override;

    void increment();

private:
    int m_bombSurrounding{}; //number of bombs surrounding the tile.
};

#endif