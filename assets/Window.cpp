#include "Window.h"
#include "globals.h"

LWindow::LWindow()
{
    //Initialize non-existant window
    mWindow = NULL;
    mMouseFocus = false;
    mKeyboardFocus = false;
    mFullScreen = false;
    mMinimized = false;
    mWidth = 0;
    mHeight = 0;
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
        gTileSize = mHeight/Constants::BOARD_SIZE;
        gOffset = mWidth/2 - gTileSize * Constants::BOARD_SIZE/2;
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
            gTileSize = mHeight > mWidth ? mWidth/Constants::BOARD_SIZE : mHeight/Constants::BOARD_SIZE;
            gOffset = mWidth/2 - gTileSize * Constants::BOARD_SIZE/2;
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
        // Window caption
            std::stringstream caption;
            caption << "Minesweeper";
            SDL_SetWindowTitle( mWindow, caption.str().c_str() );
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

void LWindow::free()
{
    SDL_DestroyWindow(mWindow);
}

int LWindow::getWidth()
{
    return mWidth;
}

int LWindow::getHeight()
{
    return mHeight;
}

bool LWindow::hasMouseFocus()
{
    return mMouseFocus;
}

bool LWindow::hasKeyboardFocus()
{
    return mKeyboardFocus;
}

bool LWindow::isMinimized()
{
    return mMinimized;
}
