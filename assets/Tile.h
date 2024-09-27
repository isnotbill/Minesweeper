#ifndef TILE_H
#define TILE_H

#include <cassert>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "Constants.h"
#include "../soundeffects/soundeffect.h"
#include <optional>

struct Point2D
{
    int x;
    int y;

    // Overloading output operator for debugging purposes
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
    TILE_SPRITE_REVEALED_0, //1
    TILE_SPRITE_REVEALED_1, //2
    TILE_SPRITE_REVEALED_2, //3
    TILE_SPRITE_REVEALED_3, //4
    TILE_SPRITE_REVEALED_4, //5
    TILE_SPRITE_REVEALED_5, //6
    TILE_SPRITE_REVEALED_6, //7
    TILE_SPRITE_REVEALED_7, //8
    TILE_SPRITE_REVEALED_8, //9
    TILE_SPRITE_MARKED, // 10
    TILE_SPRITE_BOMB, // 11
    TILE_SPRITE_BOMB_EXPLODED, //12
    TILE_SPRITE_HIGHLIGHT, // 13
    TILE_SPRITE_TOTAL // 14
};


class Tile // Abstract class
{
public:
    // Constructor initializes position and the current sprite (hidden by default)
    Tile(int x, int y)
        : m_coordinate { x, y }
        , m_CurrentSprite {TILE_SPRITE_HIDDEN}
    {
        
    }
    // Default destructor 
    virtual ~Tile() = default;

    // Virtual Member functions
    virtual bool isBomb() = 0;

    // Handles mouse event
    std::optional<Tile*> handleEvent( SDL_Event* e);

    // Shows tile sprite
    void render();

    // Mutators
    void setSprite( TileSprite sprite );
    void setRevealed(bool boolean) {m_Revealed = boolean;}
    void setPosition(int x, int y);

    // Accessors
    TileSprite getSprite() const {return m_CurrentSprite;}
    bool getRevealed() const {return m_Revealed;}
    Point2D getCoordinate() const {return m_coordinate;}

protected:
    Point2D m_coordinate; // Coordinate of the tile

    TileSprite m_CurrentSprite; // Current tile sprite

    bool m_Revealed{false}; // Check if the tile is revealed as a NumberTile

};

class Bomb : public Tile 
{
public:
    // Constructor
    Bomb(int x, int y) 
        : Tile{x,y}
    {}
    // Destructor 
    virtual ~Bomb() = default;

    // Member functions
    bool isBomb() override;
};

class NumberedTile : public Tile
{
public:
    // Constructor
    NumberedTile(int x, int y)
        : Tile{x,y}
    {
        
    };

    //Destructor
    virtual ~NumberedTile() = default;

    // Member functions
    int getSurrounding() const;
    bool isBomb() override;
    
    // Overloading ++ operator
    NumberedTile& operator++(); 

private:
    int m_bombSurrounding{}; //number of bombs surrounding the tile
};

#endif
