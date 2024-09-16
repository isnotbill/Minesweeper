#include "Texture.h"
#include "Constants.h"

Texture::Texture()
{
    // Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

Texture::~Texture()
{
    // Deallocate
    free();
}



int Texture::getWidth()
{
    return mWidth;
}

int Texture::getHeight()
{
    return mHeight;
}