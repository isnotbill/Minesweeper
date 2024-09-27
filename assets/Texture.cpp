#include "Texture.h"
#include "Constants.h"
#include "globals.h"

// Initialize texture
Texture::Texture()
{
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

// Deallocates texture
Texture::~Texture()
{
    // Deallocate
    free();
}

// Load texture from resource
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

// Free texture
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

// Render the texture
void Texture::render( int x, int y, int w, int h, SDL_Rect* clip )
{
    // Set rendering space and render to screen
    //SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    SDL_Rect renderQuad;
    renderQuad.x = x;
    renderQuad.y = y;
    renderQuad.w = w;
    renderQuad.h = h;

    // Render to screen
    SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}

// Accessors
int Texture::getWidth() { return mWidth; }
int Texture::getHeight(){ return mHeight; }

// Set opacity
void Texture::setAlpha(Uint8 alpha)
{
    // Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}
