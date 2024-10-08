#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>


// Texture wrapper class
class Texture
{
    public:
        // Initializes variables
        Texture();

        // Deallocates memory
        ~Texture();

        // Loads image at specified path
        bool loadFromFile( std::string path );
        
        // Deallocates texture
        void free();

        // Renders texture at given point
        void render( int x, int y, int w, int h, SDL_Rect* clip = NULL );

        // Gets image dimensions
        int getWidth();
        int getHeight();
        SDL_Texture* getTexture() const { return mTexture; }

        void setAlpha(Uint8 alpha); // Set alpha value

    private:
        // The actual hardware texture
        SDL_Texture* mTexture;

        // Image dimensions
        int mWidth;
        int mHeight;
};
#endif