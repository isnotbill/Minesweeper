#pragma once

namespace Constants
{
    constexpr int TILE_WIDTH = 512;   // Width of each tile
    constexpr int TILE_HEIGHT = 512;  // Height of each tile
    constexpr int TOTAL_TILES_X = 16; // Number of tiles horizontally
    constexpr int TOTAL_TILES_Y = 16; // Number of tiles vertically
    constexpr int TOTAL_TILES = TOTAL_TILES_X * TOTAL_TILES_Y; // Total number of tiles
    constexpr int TILE_RENDERED_SIZE = 64; // Pixel size of each tile
    constexpr int BOARD_SIZE = 16; // dimensions nxn
    constexpr int BOMBS_MAX = 40; // Number of bombs
    //constexpr int BOARD_OFFSET = 448;

    // Screen dimension constants
    constexpr int SCREEN_WIDTH = 900;
    constexpr int SCREEN_HEIGHT = 900;
}
