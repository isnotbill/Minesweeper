#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

// Extern to use it in different files without violating ODR

extern Mix_Chunk* gFlagSound;
extern Mix_Chunk* gClickSound;
extern Mix_Chunk* gRevealSound;
extern Mix_Chunk* gBombSound;
