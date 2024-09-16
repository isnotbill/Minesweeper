 #include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "assets/Tile.h"
#include "assets/Board.h"
#include "assets/Texture.h"


// Screen dimension constants
constexpr int SCREEN_WIDTH = 1024;
constexpr int SCREEN_HEIGHT = 1024;

// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// The window we'll be rendering to
SDL_Window* gWindow = NULL;

// The window renderer
SDL_Renderer* gRenderer = NULL;

// Tile sprites
SDL_Rect gSpriteClips[ TILE_SPRITE_TOTAL ];
Texture gTileSpriteSheetTexture;

// Tile objects
//Tile gTiles[ Constants::TOTAL_TILES ];

Board gBoard{};

// Booleans

bool gInitializedBombs{false};


void Tile::render()
{
    // Show current tile sprite
    gTileSpriteSheetTexture.render( m_coordinate.x, m_coordinate.y, &gSpriteClips[ m_CurrentSprite ] );
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

bool init()
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
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
        gWindow = SDL_CreateWindow( "Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            // Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
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
        // Set sprite clips
        // TILE_SPRITE_HIDDEN
        gSpriteClips[ TILE_SPRITE_HIDDEN ].x = 0;
        gSpriteClips[ TILE_SPRITE_HIDDEN ].y = 0;
        gSpriteClips[ TILE_SPRITE_HIDDEN ].w = Constants::TILE_WIDTH;
        gSpriteClips[ TILE_SPRITE_HIDDEN ].h = Constants::TILE_HEIGHT;

        // TILE_SPRITE_BOMB
        gSpriteClips[ TILE_SPRITE_BOMB ].x = 0;
        gSpriteClips[ TILE_SPRITE_BOMB ].y = Constants::TILE_HEIGHT;
        gSpriteClips[ TILE_SPRITE_BOMB ].w = Constants::TILE_WIDTH;
        gSpriteClips[ TILE_SPRITE_BOMB ].h = Constants::TILE_HEIGHT;

        // TILE_SPRITE_FLAG
        gSpriteClips[ TILE_SPRITE_MARKED ].x = 0;
        gSpriteClips[ TILE_SPRITE_MARKED ].y = Constants::TILE_HEIGHT * 2;
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
            if(i < 4)
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

    return success;
}

void close()
{
    // Free loaded images
    gTileSpriteSheetTexture.free();

    // Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
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
                    
                    // Handle tile events
                    gBoard.handleEvents( &e );
                }

                // Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );

                // Render tiles
                gBoard.renderTiles();

                // Update screen
                SDL_RenderPresent( gRenderer );
            }
        }
    }

    // Free resources and close SDL
    close();
}