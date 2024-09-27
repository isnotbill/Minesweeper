# Minesweeper Game Made in C++ and Integrated With SDL2

Dependencies: SDL2, SDL2-image, and SDL2-mixer libraries.
Authors: Bill Huynh-Lu and Rei Shibatani

## How To Run The Game

**To play the game, simply follow the steps:**

1. Download the ZIP from [Github](https://github.com/isnotbill/Minesweeper)
2. Run main.exe

## Gameplay

Based entirely on the classic [Minesweeper](https://minesweepergame.com/strategy/how-to-play-minesweeper.php) game
- RMB to reveal a tile
- LMB to flag a tile
- MMB to reveal surrounding tiles

## Credits

- Minesweeper Icons by RockPaperKatana - Licensed under CC-BY-SA 3.0
- Tile font "Mine-Sweeper" by Gangetsha Lyx - Licensed under CC-BY-SA 3.0
- freesound.org for all the uncopyrighted sound effects in the project
- Skull pixel art by [RamSketches](https://www.pinterest.com/ramsketches/)
- Crown pixel art by [pngtree](https://pngtree.com/)
- Retry button pixel art by [rrcaseyr](https://opengameart.org/users/rrcaseyr)

Special thanks to these creators for their amazing work, which helps bring this project to life!

## Documentation
### Root directory
- *GameLogic.cpp*: Handles the core logic of the gameplay.
- *main.cpp*: Calls the start() function to initiate the gameplay.

### Assets directory
- *Board*: Class that represents the Minesweeper board.
- *Button*: Class to create an interactable texture.
- *Constants.h*: Constants for the game and rendering.
- *globals.h*: Global variables for rendering.
- *Random.h*: Random value algorithm.
- *Texture*: Class to hold and render an image.
- *Tile*: Classes to represent the tiles in the board.
- *Window*: Class to handle core rendering and the window the game is held.
