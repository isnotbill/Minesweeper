#ifndef LWINDOW_H
#define LWINDOW_H

#include <cassert>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "Constants.h"

class LWindow
{
    public:
        //Intializes internals
        LWindow();

        //Creates LWindow
        bool init();

        //Creates renderer from internal LWindow
        SDL_Renderer* createRenderer();

        //Handles LWindow events
        void handleEvent( SDL_Event& e );

        //Deallocates internals
        void free();

        //LWindow dimensions
        int getWidth();
        int getHeight();

        //LWindow focii
        bool hasMouseFocus();
        bool hasKeyboardFocus();
        bool isMinimized();
        bool isFullScreen();

    private:
        //LWindow data
        SDL_Window* mWindow;

        //LWindow dimensions
        int mWidth;
        int mHeight;

        //LWindow focus
        bool mMouseFocus;
        bool mKeyboardFocus;
        bool mFullScreen;
        bool mMinimized;
};

#endif