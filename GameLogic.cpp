#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include "assets/Tile.h"
#include "assets/Board.h"
#include "assets/Texture.h"
#include "assets/Window.h"
#include "assets/Button.h"


// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// The window we'll be rendering to
LWindow gWindow;

// The window renderer
SDL_Renderer* gRenderer{nullptr};

// Tile sprites
SDL_Rect gSpriteClips[ TILE_SPRITE_TOTAL ];
Texture gTileSpriteSheetTexture;
Texture gWinTexture;
Texture gLoseTexture;

// Game Board
Board gBoard{};

// Rendering variables
int gTileSize;
int gOffset;

// Try again button 384x384
Button gTryAgainButton{Constants::SCREEN_WIDTH/2 - gTileSize, Constants::SCREEN_HEIGHT/2 + gTileSize * 2, gTileSize * 2, gTileSize* 2};

// Sound
Mix_Chunk* gFlagSound{nullptr};
Mix_Chunk* gClickSound{nullptr};
Mix_Chunk* gRevealSound{nullptr};
Mix_Chunk* gBombSound{nullptr};
Mix_Chunk* gVictorySound{nullptr};
Mix_Chunk* gDefeatSound{nullptr};

bool init()
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        // Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        // Create window
        if( !gWindow.init() )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            // Create renderer for window
            gRenderer = gWindow.createRenderer();
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                // Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }

                
                 //Initialize SDL_mixer
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }

                
            }
        }
    }

    return success;
}

bool loadMedia()
{
    // Loading success flag
    bool success = true;

    // Loading button texture
    if( !gTryAgainButton.setTexture( "resources/retrybutton.png" ) )
    {
        printf( "Failed to load retry button texture!\n" );
        success = false;
    }

    if( !gWinTexture.loadFromFile( "resources/crown.png" ) )
    {
        printf( "Failed to load win texture!\n" );
        success = false;
    }

    if( !gLoseTexture.loadFromFile( "resources/skull.png" ) )
    {
        printf( "Failed to load lose texture!\n" );
        success = false;
    }
    
    // Load spritesheet texture
    if( !gTileSpriteSheetTexture.loadFromFile( "resources/spritesheet.png" ) )
    {
        printf( "Failed to load tile sprite sheet texture!\n" );
        success = false;
    }
    else
    {
        // Set sprite clips
        // TILE_SPRITE_HIDDEN
        gSpriteClips[ TILE_SPRITE_HIDDEN ].x = 0;
        gSpriteClips[ TILE_SPRITE_HIDDEN ].y = 0;
        gSpriteClips[ TILE_SPRITE_HIDDEN ].w = Constants::TILE_WIDTH;
        gSpriteClips[ TILE_SPRITE_HIDDEN ].h = Constants::TILE_HEIGHT;

        // TILE_SPRITE_BOMB
        gSpriteClips[ TILE_SPRITE_BOMB ].x = 0;
        gSpriteClips[ TILE_SPRITE_BOMB ].y = Constants::TILE_HEIGHT * 2;
        gSpriteClips[ TILE_SPRITE_BOMB ].w = Constants::TILE_WIDTH;
        gSpriteClips[ TILE_SPRITE_BOMB ].h = Constants::TILE_HEIGHT;

        // TILE_SPRITE_BOMB_EXPLODED
        gSpriteClips[ TILE_SPRITE_BOMB_EXPLODED ].x = Constants::TILE_WIDTH;
        gSpriteClips[ TILE_SPRITE_BOMB_EXPLODED ].y = Constants::TILE_HEIGHT * 2;
        gSpriteClips[ TILE_SPRITE_BOMB_EXPLODED ].w = Constants::TILE_WIDTH;
        gSpriteClips[ TILE_SPRITE_BOMB_EXPLODED ].h = Constants::TILE_HEIGHT;            

        // TILE_SPRITE_FLAG
        gSpriteClips[ TILE_SPRITE_MARKED ].x = 0;
        gSpriteClips[ TILE_SPRITE_MARKED ].y = Constants::TILE_HEIGHT;
        gSpriteClips[ TILE_SPRITE_MARKED ].w = Constants::TILE_WIDTH;
        gSpriteClips[ TILE_SPRITE_MARKED ].h = Constants::TILE_HEIGHT;

        // TILE_SPRITE_REVEALED_0
        gSpriteClips[ TILE_SPRITE_REVEALED_0 ].x = Constants::TILE_WIDTH * 3;
        gSpriteClips[ TILE_SPRITE_REVEALED_0 ].y = Constants::TILE_HEIGHT * 2;
        gSpriteClips[ TILE_SPRITE_REVEALED_0 ].w = Constants::TILE_WIDTH;
        gSpriteClips[ TILE_SPRITE_REVEALED_0 ].h = Constants::TILE_HEIGHT;

        // TILE_SPRITE_REVEALED 1 TO 8
        for( int i = 2; i < 10; i++ )
        {
            if(i < 6)
            {
                gSpriteClips[i].x = Constants::TILE_WIDTH * (i-2);
                gSpriteClips[i].y = Constants::TILE_HEIGHT * 3;
            }
            else
            {
                gSpriteClips[i].x = Constants::TILE_WIDTH * (i-6);
                gSpriteClips[i].y = Constants::TILE_HEIGHT * 4;
            }
            gSpriteClips[ i ].w = Constants::TILE_WIDTH;
            gSpriteClips[ i ].h = Constants::TILE_HEIGHT;
        }
    }

    gFlagSound = Mix_LoadWAV( "soundeffects/flag.wav" );
    gClickSound = Mix_LoadWAV( "soundeffects/click.wav" );
    gRevealSound = Mix_LoadWAV( "soundeffects/reveal.wav" );
    gBombSound = Mix_LoadWAV( "soundeffects/bomb.mp3" );
    gVictorySound = Mix_LoadWAV( "soundeffects/victory.wav" );
    gDefeatSound = Mix_LoadWAV( "soundeffects/defeat.wav" );


	if(!gFlagSound || !gClickSound || !gRevealSound || !gBombSound)
	{
		printf( "Failed to load sound effects. SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
    else
    {
        Mix_VolumeChunk(gFlagSound, MIX_MAX_VOLUME * 0.4);
        Mix_VolumeChunk(gClickSound, MIX_MAX_VOLUME * 0.6);
        Mix_VolumeChunk(gBombSound, MIX_MAX_VOLUME * 0.6);
        Mix_VolumeChunk(gVictorySound, MIX_MAX_VOLUME * 0.2);
        Mix_VolumeChunk(gDefeatSound, MIX_MAX_VOLUME * 0.2);
    }

    return success;
}

void close()
{
    // Free loaded images
    gTileSpriteSheetTexture.free();

    // Free audio
	Mix_FreeChunk(gFlagSound);
    gFlagSound = nullptr;
    // Destroy window
    SDL_DestroyRenderer( gRenderer );
    gWindow.free();

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
    Mix_Quit();
}

void start()
{
    // Overlay variables
    bool startFadeOut = false;
    Uint8 overlayAlpha = 0;     // Start from fully transparent
    Uint8 maxOverlayAlpha = 180; // Max alpha value (adjust for desired darkness)
    int fadeOutSpeed = 4;        // Adjust the speed of the fade-out

    // Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        // Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {   
            // Main loop flag
            bool quit = false;

            // Win flag
            bool win = false;

            //Flag to check if the victory/defeat sounce has been played to avoid infinite replays
            bool soundPlayed = false;

            // Event handler
            SDL_Event e;

            // While application is running
            while( !quit )
            {
                // Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    // User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    gWindow.handleEvent( e );

                    // Handle tile events
                    if(!gBoard.getGameOver())
                    {
                        gBoard.handleEvents( &e );
                    }
                    else
                    {
                        if (!startFadeOut)
                        {
                            startFadeOut = true;
                            overlayAlpha = 0;
                        }
                        if(gTryAgainButton.handleEvent( &e ) == SDL_BUTTON_LEFT)
                        {  
                            Mix_PlayChannel(-1, gClickSound, 0);
                            gBoard = {};
                            overlayAlpha = 0;
                            win = false;
                            soundPlayed = false;
                        }
                    }
                }
                if( !gWindow.isMinimized() )
                {

                    // Clear screen
                    SDL_SetRenderDrawColor( gRenderer, 0x11, 0x26, 0x35, 255 );
                    SDL_RenderClear( gRenderer );

                    // Render tiles
                    gBoard.renderTiles();

                    // If game over, update the fade-out effect
                    if (gBoard.win())
                    {
                        win = true;
                    }
                    
                    if (gBoard.getGameOver())
                    {
                        gBoard.showBombs();
                        // Increase overlay alpha value
                        if (overlayAlpha < maxOverlayAlpha) {
                            overlayAlpha = (overlayAlpha + fadeOutSpeed > maxOverlayAlpha) ? maxOverlayAlpha : overlayAlpha + fadeOutSpeed;
                        }

                        // Render the semi-transparent overlay
                        SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
                        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, overlayAlpha);
                        
                        SDL_Rect overlayRect = {gOffset, 0, gTileSize*Constants::BOARD_SIZE, gTileSize*Constants::BOARD_SIZE};
                        SDL_RenderFillRect(gRenderer, &overlayRect);

                        

                        // Once fade-out is complete, render the retry button
                        if (overlayAlpha >= maxOverlayAlpha) {
                            gTryAgainButton.render(gTileSize * 7, gWindow.getHeight()/2 + gTileSize * 2, gTileSize * 2, gTileSize * 2);
                            if(win)
                            {
                                gWinTexture.render(gTileSize * 5.5 + gOffset, gWindow.getHeight()/2 - gTileSize * 3.5, gTileSize * 5, gTileSize * 5, NULL);
                            }
                            else
                            {
                                gLoseTexture.render(gTileSize * 6 + gOffset, gWindow.getHeight()/2 - gTileSize * 3, gTileSize * 4, gTileSize * 4, NULL);
                            }
                            if (!soundPlayed)
                            {
                                soundPlayed = true;
                                if (win)
                                {
                                    Mix_PlayChannel( -1, gVictorySound, 0 );
                                }
                                else
                                {
                                    Mix_PlayChannel( -1, gDefeatSound, 0 ); 
                                }                                
                            }
                        }
                    }

                    // Update screen
                    SDL_RenderPresent( gRenderer );
                }
            }
        }
    }

    // Free resources and close SDL
    close();
}
