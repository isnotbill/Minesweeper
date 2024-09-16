#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "classes/Tile.h"
#include "classes/Board.h"

#define SDL_MAIN_HANDLED


// Screen dimension constants
constexpr int SCREEN_WIDTH = 1024;
constexpr int SCREEN_HEIGHT = 1024;

// Tile constants
constexpr int TILE_WIDTH = 512;   // Width of each tile
constexpr int TILE_HEIGHT = 512;  // Height of each tile
constexpr int TOTAL_TILES_X = 16; // Number of tiles horizontally
constexpr int TOTAL_TILES_Y = 16; // Number of tiles vertically
constexpr int TOTAL_TILES = TOTAL_TILES_X * TOTAL_TILES_Y;
constexpr int TILE_RENDERED_SIZE = 64;

// Enum for different tile sprites

// Texture wrapper class
class LTexture
{
    public:
        // Initializes variables
        LTexture();

        // Deallocates memory
        ~LTexture();

        // Loads image at specified path
        bool loadFromFile( std::string path );
        
        // Deallocates texture
        void free();

        // Renders texture at given point
        void render( int x, int y, SDL_Rect* clip = NULL );

        // Gets image dimensions
        int getWidth();
        int getHeight();

    private:
        // The actual hardware texture
        SDL_Texture* mTexture;

        // Image dimensions
        int mWidth;
        int mHeight;
};

// The tile on the grid
class LTile
{
    public:
        // Initializes internal variables
        LTile();

        // Sets tile position
        void setPosition( int x, int y );

        // Handles mouse event
        void handleEvent( SDL_Event* e );

        // Shows tile sprite
        void render();

        // Sets the current sprite
        void setSprite( TileSprite sprite );

    private:
        // Position
        SDL_Point mPosition;

        // Current sprite
        TileSprite mCurrentSprite;
};

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
LTexture gTileSpriteSheetTexture;

// Tile objects
LTile gTiles[ TOTAL_TILES ];

LTexture::LTexture()
{
    // Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    // Deallocate
    free();
}

bool LTexture::loadFromFile( std::string path )
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

void LTexture::free()
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

void LTexture::render( int x, int y, SDL_Rect* clip )
{
    // Set rendering space and render to screen
    //SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    SDL_Rect renderTile;
    renderTile.x = x;
    renderTile.y = y;
    renderTile.w = TILE_RENDERED_SIZE;
    renderTile.h = TILE_RENDERED_SIZE;

    // Render to screen
    SDL_RenderCopy( gRenderer, mTexture, clip, &renderTile );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

LTile::LTile()
{
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = TILE_SPRITE_HIDDEN;
}

void LTile::setPosition( int x, int y )
{
    mPosition.x = x;
    mPosition.y = y;
}

void LTile::handleEvent( SDL_Event* e )
{
    // If mouse event happened
    if( e->type == SDL_MOUSEBUTTONDOWN )
    {
        // Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );

        // Check if mouse is in tile
        bool inside = true;

        // Mouse is left of the tile
        if( x < mPosition.x )
        {
            inside = false;
        }
        // Mouse is right of the tile
        else if( x > mPosition.x + TILE_RENDERED_SIZE )
        {
            inside = false;
        }
        // Mouse above the tile
        else if( y < mPosition.y )
        {
            inside = false;
        }
        // Mouse below the tile
        else if( y > mPosition.y + TILE_RENDERED_SIZE )
        {
            inside = false;
        }

        // Mouse is inside tile
        if( inside )
        {
            // For simplicity, toggle between hidden and revealed
            if( mCurrentSprite == TILE_SPRITE_HIDDEN )
            {
                // You can set the revealed sprite based on your game logic
                mCurrentSprite = TILE_SPRITE_REVEALED_0; // Example: revealed with 0 adjacent mines
            }
        }
    }
}

void LTile::render()
{
    // Show current tile sprite
    gTileSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
}

void LTile::setSprite( TileSprite sprite )
{
    mCurrentSprite = sprite;
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
    if( !gTileSpriteSheetTexture.loadFromFile( "SDL2/spritesheet.png" ) )
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
        gSpriteClips[ TILE_SPRITE_HIDDEN ].w = TILE_WIDTH;
        gSpriteClips[ TILE_SPRITE_HIDDEN ].h = TILE_HEIGHT;

        // TILE_SPRITE_BOMB
        gSpriteClips[ TILE_SPRITE_BOMB ].x = 0;
        gSpriteClips[ TILE_SPRITE_BOMB ].y = TILE_HEIGHT;
        gSpriteClips[ TILE_SPRITE_BOMB ].w = TILE_WIDTH;
        gSpriteClips[ TILE_SPRITE_BOMB ].h = TILE_HEIGHT;

        // TILE_SPRITE_FLAG
        gSpriteClips[ TILE_SPRITE_MARKED ].x = 0;
        gSpriteClips[ TILE_SPRITE_MARKED ].y = TILE_HEIGHT * 2;
        gSpriteClips[ TILE_SPRITE_MARKED ].w = TILE_WIDTH;
        gSpriteClips[ TILE_SPRITE_MARKED ].h = TILE_HEIGHT;

        // TILE_SPRITE_REVEALED_0
        gSpriteClips[ TILE_SPRITE_REVEALED_0 ].x = TILE_WIDTH * 3;
        gSpriteClips[ TILE_SPRITE_REVEALED_0 ].y = TILE_HEIGHT * 2;
        gSpriteClips[ TILE_SPRITE_REVEALED_0 ].w = TILE_WIDTH;
        gSpriteClips[ TILE_SPRITE_REVEALED_0 ].h = TILE_HEIGHT;

        // TILE_SPRITE_REVEALED 1 TO 8
        for( int i = 2; i < 10; i++ )
        {
            if(i < 4)
            {
                gSpriteClips[i].x = TILE_WIDTH * (i-2);
                gSpriteClips[i].y = TILE_HEIGHT * 3;
            }
            else
            {
                gSpriteClips[i].x = TILE_WIDTH * (i-6);
                gSpriteClips[i].y = TILE_HEIGHT * 4;
            }
            gSpriteClips[ i ].w = TILE_WIDTH;
            gSpriteClips[ i ].h = TILE_HEIGHT;
        }


        // Initialize tiles
        int tileIndex = 0;
        for( int y = 0; y < TOTAL_TILES_Y; ++y )
        {
            for( int x = 0; x < TOTAL_TILES_X; ++x )
            {
                gTiles[ tileIndex ].setPosition( x * TILE_RENDERED_SIZE, y * TILE_RENDERED_SIZE );
                ++tileIndex;
            }
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

int main( int argc, char* args[] )
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
                    for( int i = 0; i < TOTAL_TILES; ++i )
                    {
                        gTiles[ i ].handleEvent( &e );
                    }
                }

                // Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );

                // Render tiles
                for( int i = 0; i < TOTAL_TILES; ++i )
                {
                    gTiles[ i ].render();
                }

                // Update screen
                SDL_RenderPresent( gRenderer );
            }
        }
    }

    // Free resources and close SDL
    close();

    return 0;
}