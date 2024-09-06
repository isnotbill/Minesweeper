#ifndef TILE_H
#define TILE_H

#include <cassert>
#include <iostream>

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

class Tile // Abstract class
{
public:
    Tile(int x, int y)
        : m_coordinate { x, y }  
    {}
    virtual ~Tile() = default;
    virtual bool isBomb() = 0;
    virtual void display() = 0;
    Point2D getCoordinate() const {return m_coordinate;}

protected:
    Point2D m_coordinate;

};

class Bomb : public Tile 
{
public:
    Bomb(int x, int y)
        : Tile{x,y}
    {
        
    }
    virtual ~Bomb() = default;
    bool isBomb() override
    {
        return true;
    };
    void display() override
    {
        std::cout << m_coordinate << " is a Bomb.\n";
    }
    
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

    int setBombSurrounding(int bomb) // Sets the surrounding the tile
    {
        assert(bomb < 9); // Number of bombs surrounding the tile must be 8 or below
        m_bombSurrounding = bomb;
    }

    int getSurrounding() const // gets m_bombSurrounding
    {
        return m_bombSurrounding;
    }


    bool isBomb() override
    {
        return false;
    };

    void display() override
    {
        std::cout << m_coordinate << "with " << m_bombSurrounding << " bombs surrounding" << " is a NumberedTile.\n";
    };

    void increment()
    {
        m_bombSurrounding = m_bombSurrounding + 1;
    }

    NumberedTile operator++(int)
    {
        ++m_bombSurrounding;
        return *this;
    }

private:
    int m_bombSurrounding{}; //number of bombs surrounding the tile.
};

#endif