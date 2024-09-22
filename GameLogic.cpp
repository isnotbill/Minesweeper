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

// Game Board
Board gBoard{};

// Try again button 384x384
Button gTryAgainButton{Constants::SCREEN_WIDTH/2, Constants::SCREEN_HEIGHT/2};

// Sound
Mix_Chunk* gFlagSound{nullptr};
Mix_Chunk* gClickSound{nullptr};
Mix_Chunk* gRevealSound{nullptr};
Mix_Chunk* gBombSound{nullptr};

void Tile::render()
{
    // Show current tile sprite
    if(gWindow.isFullScreen() )
    {
        gTileSpriteSheetTexture.render( m_coordinate.x * Constants::TILE_RENDERED_SIZE + Constants::BOARD_OFFSET, m_coordinate.y * Constants::TILE_RENDERED_SIZE, &gSpriteClips[ m_CurrentSprite ] );
    }
    else
    {
        
        gTileSpriteSheetTexture.render( m_coordinate.x * Constants::TILE_RENDERED_SIZE, m_coordinate.y * Constants::TILE_RENDERED_SIZE, &gSpriteClips[ m_CurrentSprite ] );
    }

}

bool Texture::loadFromFile( std::string path )
{
    // Get rid of preexisting texture
    free();

    // The final texture
    SDL_Texture* newTexture = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            // Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    // Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

void Texture::free()
{
    // Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void Texture::render( int x, int y, SDL_Rect* clip )
{
    // Set rendering space and render to screen
    //SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    SDL_Rect renderTile;
    renderTile.x = x;
    renderTile.y = y;
    renderTile.w = Constants::TILE_RENDERED_SIZE;
    renderTile.h = Constants::TILE_RENDERED_SIZE;

    // Render to screen
    SDL_RenderCopy( gRenderer, mTexture, clip, &renderTile );
}

bool LWindow::init()
{
    //Create window
    mWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
    if( mWindow != NULL )
    {
        mMouseFocus = true;
        mKeyboardFocus = true;
        mWidth = Constants::SCREEN_WIDTH;
        mHeight = Constants::SCREEN_HEIGHT;
    }

    return mWindow != NULL;
}

SDL_Renderer* LWindow::createRenderer()
{
    return SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
}

void LWindow::handleEvent( SDL_Event& e )
{
    //Window event occured
    if( e.type == SDL_WINDOWEVENT )
    {
        //Caption update flag
        bool updateCaption = false;
        switch( e.window.event )
        {
            //Get new dimensions and repaint on window size change
            case SDL_WINDOWEVENT_SIZE_CHANGED:
            mWidth = e.window.data1;
            mHeight = e.window.data2;
            std::cout << mWidth << " " << mHeight << '\n';
            SDL_RenderPresent( gRenderer );
            break;

            //Repaint on exposure
            case SDL_WINDOWEVENT_EXPOSED:
            SDL_RenderPresent( gRenderer );
            break;

            //Mouse entered window
            case SDL_WINDOWEVENT_ENTER:
            mMouseFocus = true;
            updateCaption = true;
            break;
            
            //Mouse left window
            case SDL_WINDOWEVENT_LEAVE:
            mMouseFocus = false;
            updateCaption = true;
            break;

            //Window has keyboard focus
            case SDL_WINDOWEVENT_FOCUS_GAINED:
            mKeyboardFocus = true;
            updateCaption = true;
            break;

            //Window lost keyboard focus
            case SDL_WINDOWEVENT_FOCUS_LOST:
            mKeyboardFocus = false;
            updateCaption = true;
            break;

            //Window minimized
            case SDL_WINDOWEVENT_MINIMIZED:
            mMinimized = true;
            break;

            //Window maximized
            case SDL_WINDOWEVENT_MAXIMIZED:
            mMinimized = false;
            break;
            
            //Window restored
            case SDL_WINDOWEVENT_RESTORED:
            mMinimized = false;
            break;
        }
        //Update window caption with new data
        if( updateCaption )
        {
            std::stringstream caption;
            caption << "SDL Tutorial - MouseFocus:" << ( ( mMouseFocus ) ? "On" : "Off" ) << " KeyboardFocus:" << ( ( mKeyboardFocus ) ? "On" : "Off" );
            SDL_SetWindowTitle( mWindow, caption.str().c_str() );
        }
    }
    //Enter exit full screen on return key
    else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN )
    {
        if( mFullScreen )
        {
            SDL_SetWindowFullscreen( mWindow, 0 );
            mFullScreen = false;
        }
        else
        {
            SDL_SetWindowFullscreen( mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP );
            mFullScreen = true;
            mMinimized = false;
        }
    }
}

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

    // Load spritesheet texture
    if( !gTileSpriteSheetTexture.loadFromFile( "resources/spritesheet.png" ) )
    {
        printf( "Failed to load tile sprite sheet texture!\n" );
        success = false;
    }
    else
    {
        if( !gTryAgainButton.setTexture( "resources/retrybutton.png" ) )
        {
            printf( "Failed to load retry button texture!\n" );
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
    }

    gFlagSound = Mix_LoadWAV( "soundeffects/flag.wav" );
    gClickSound = Mix_LoadWAV( "soundeffects/click.wav" );
    gRevealSound = Mix_LoadWAV( "soundeffects/reveal.wav" );
    gBombSound = Mix_LoadWAV( "soundeffects/bomb.mp3" );


	if(!gFlagSound || !gClickSound || !gRevealSound || !gBombSound)
	{
		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
    else
    {
        Mix_VolumeChunk(gFlagSound, MIX_MAX_VOLUME * 0.4);
        Mix_VolumeChunk(gClickSound, MIX_MAX_VOLUME * 0.6);
        Mix_VolumeChunk(gBombSound, MIX_MAX_VOLUME * 0.6);
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
                        gBoard.handleEvents( &e, Constants::BOARD_OFFSET, gWindow.isFullScreen());
                    }
                }
                if( !gWindow.isMinimized() )
                {
                    // Clear screen
                    SDL_SetRenderDrawColor( gRenderer, 0x11, 0x26, 0x35, 255 );
                    SDL_RenderClear( gRenderer );

                    // Render tiles
                    gBoard.renderTiles();
                    if(gBoard.win())
                    {
                        gBoard = {};
                    }
                    if (gBoard.getGameOver())
                    {
                        gBoard.showBombs();
                        gTryAgainButton.render();
                        //gBoard = {};
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
