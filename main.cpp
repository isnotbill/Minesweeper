#include <iostream>
#include <vector>
#include "classes/Board.h"
#include "classes/Tile.h"
#include <iostream>

int main()
{
    Board b {};
    b.randomizeBombs();
    b.display();
    
    return 0;
}
