//============================================================================
// Name        : Knights2.cpp
// Author      : Daniel "Monty" Monteiro
// Version     :
// Copyright   : 2014 - Daniel Monteiro
// Description : Knights2
//===========================================================================
#include <iostream>
#include <SDL2/SDL.h>
#include "SDL2renderer.h"

SDL_Window *window;
SDL_Renderer *render;
SDL_Texture *bricks;
SDL_Texture *grass;

SDL_Texture *entry;
SDL_Texture *exitGate;
SDL_Texture *bull;
SDL_Texture *falcon;
SDL_Texture *turtle;
SDL_Texture *demon;


SDL_Texture * loadTexture( const char* filename ) {

    SDL_Texture *texture = nullptr;
    SDL_Surface *bmp = SDL_LoadBMP( filename );

    if ( bmp != nullptr ) {
        texture = SDL_CreateTextureFromSurface( render, bmp );
        SDL_FreeSurface( bmp );
    }

    return texture;
}

void renderMap( char map[20][20]) {

    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = 640;
    src.h = 480;

    SDL_SetRenderDrawColor( render, 255, 255, 255, 255);
    SDL_RenderFillRect( render, &src );
    SDL_Texture *current;

    for ( int x = 0; x < 20; ++x ) {
        for ( int y = 0; y < 20; ++y ) {
            src.x = 16 * x;
            src.y = 16 * y;
            src.w = 16;
            src.h = 16;

            if ( map [ x ][ y ] == '#' ) {
                current = bricks;
            } else if ( map [ x ][ y ] == '.' ) {
                current = grass;
            } else if ( map [ x ][ y ] == '9' ) {
                current = exitGate;
            } else if ( map [ x ][ y ] == '*' ) {
                current = entry;
            }

            SDL_RenderCopy( render, current, nullptr,  &src );

        }
    }
}

void renderView( char map[20][20]) {
    renderMap( map );
}

int initSDL() {

    if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 ) {
        std::cout << "Something went wrong: " << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow( "Knights2",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               640,
                               480,
                               SDL_WINDOW_SHOWN );

    if ( window == nullptr ) {
        std::cout << "Could not create a Window. Check your doors next: ";
        std::cout << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    render = SDL_CreateRenderer( window, -1, 0 );

    if ( render == nullptr ) {
        std::cout << "Surrender! Could not create renderer: ";
        std::cout << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }


    grass = loadTexture( "res/grass.bmp" );
    bricks = loadTexture( "res/bricks.bmp" );

    entry = loadTexture( "res/begin.bmp" );
    exitGate = loadTexture( "res/exit.bmp" );
    falcon = loadTexture( "res/falcon.bmp" );
    bull = loadTexture( "res/bull.bmp" );
    turtle = loadTexture( "res/turtle.bmp" );
    demon = loadTexture( "res/demon.bmp" );





    if ( grass == nullptr || bricks == nullptr ) {
        SDL_DestroyRenderer( render );
        SDL_DestroyWindow( window );
        std::cout << "You got a very smooth surface: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    return 0;
}

void mainSDLLoop() {
    SDL_Event e;
    bool quit = false;

    while ( !quit ) {
        while( SDL_PollEvent( &e ) ) {
            switch( e.type ) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:

            {
                if ( e.key.keysym.sym == SDLK_q ) {
                    quit = true;
                }
            }
            break;
            }
        }

        SDL_RenderClear( render );

        renderView( map );
        SDL_RenderPresent( render );

    }
}

void destroy() {

    SDL_DestroyTexture( grass );
    SDL_DestroyTexture( bricks );

    SDL_DestroyRenderer( render );
    SDL_DestroyWindow( window );

    SDL_Quit();
}
