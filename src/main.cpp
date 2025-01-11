#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

const int SCREEN_HEIGHT = 960;
const int SCREEN_WIDTH = 1280;

// Global Functions
bool init();
bool loadMedia();
void close();

enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

// Global variables
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gCurrentSurface = NULL;
SDL_Surface* gLoadSurface = NULL;
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];
SDL_Surface* loadSurface( std::string path );

bool init()
{
    bool success = true;

    // Initialise SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialise. SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        // Create window
        gWindow = SDL_CreateWindow( "Window Title Goes Here!", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

        if( gWindow == NULL )
        {
            printf( "Window could not be created. SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            // Get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }

    return success;
}

SDL_Surface* loadSurface( std::string path ){

    SDL_Surface* optimizedSurface = NULL;

    // Load image onto surface
    SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );

    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s, SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }
    else
    {
        // Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
        if( optimizedSurface == NULL )
        {
            printf( "Unable to optimise image %s, SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        SDL_FreeSurface( loadedSurface );

    }

    return optimizedSurface;

}

bool loadMedia()
{
    bool success = true;

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = gLoadSurface = SDL_LoadBMP( "images/default.bmp" );
    if( gLoadSurface == NULL )
    {
        printf( "Unable to load default image SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = gLoadSurface = SDL_LoadBMP( "images/up.bmp" );
    if( gLoadSurface == NULL )
    {
        printf( "Unable to load default image SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = gLoadSurface = SDL_LoadBMP( "images/down.bmp" );
    if( gLoadSurface == NULL )
    {
        printf( "Unable to load default image SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = gLoadSurface = SDL_LoadBMP( "images/left.bmp" );
    if( gLoadSurface == NULL )
    {
        printf( "Unable to load default image SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = gLoadSurface = SDL_LoadBMP( "images/right.bmp" );
    if( gLoadSurface == NULL )
    {
        printf( "Unable to load default image SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }

    return success;
}

void close()
{

    SDL_FreeSurface( gLoadSurface );
    gLoadSurface = NULL;

    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    SDL_Quit();
}

int main( int argc, char* args[] )
{

    if( !init() )
    {
        printf( "Failed to initialise.\n" );
    }
    else
    {
        if ( !loadMedia() )
        {
            printf( "Failed to load media.\n" );
        }
        else
        {
            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
            SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, NULL );
            SDL_UpdateWindowSurface( gWindow );
        }

        bool quit = false;
        SDL_Event e;

        while( !quit ){

            while( SDL_PollEvent( &e ) != 0 )
            {

                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
                if( e.type == SDL_KEYDOWN )
                {

                    switch( e.key.keysym.sym )
                    {
                        case SDLK_ESCAPE:
                        quit = true;
                        break;

                        case SDLK_UP:
                        gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
                        break;

                        case SDLK_DOWN:
                        gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
                        break;

                        case SDLK_LEFT:
                        gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
                        break;

                        case SDLK_RIGHT:
                        gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
                        break;

                        default:
                        break;
                    }
                }

            }

            SDL_Rect stretchRect;
            stretchRect.x = 0;
            stretchRect.y = 0;
            stretchRect.w = SCREEN_WIDTH;
            stretchRect.h = SCREEN_HEIGHT;

            SDL_BlitScaled( gCurrentSurface, NULL, gScreenSurface, &stretchRect );
            SDL_UpdateWindowSurface( gWindow );

        }

    }

    close();

    return 0;
}
