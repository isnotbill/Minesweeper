#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include "Tile.h"
#include "Board.h"
#include "Texture.h"
#include "Window.h"
#include "Button.h"

// The window we'll be rendering to
extern LWindow gWindow;

// The window renderer
extern SDL_Renderer* gRenderer;

// Sprites
extern SDL_Rect gSpriteClips[ TILE_SPRITE_TOTAL ];
extern Texture gTileSpriteSheetTexture;


// Game Board
extern Board gBoard;

// Try again button 384x384
extern Button gTryAgainButton;