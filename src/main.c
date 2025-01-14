#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Screen dimensions
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

// Key press surface constants
enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

// Functions
bool init();
bool loadMedia();
void closeAll();
SDL_Surface* loadSurface( char* path );
SDL_Texture* loadTexture( char* path );

// Global variables
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gCurrentSurface = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;

SDL_Surface* loadSurface(char* path)
{
    // The final optimised image
    SDL_Surface* optimisedSurface = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path );
    
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", path, IMG_GetError() );
    }
    else
    {
        // Convert surface to screen format
        optimisedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
        if( optimisedSurface == NULL )
        {
            printf( "Unable to optimise image %s! SDL Error: %s\n", path, SDL_GetError() );
        }

        // Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    
    return optimisedSurface;
}

SDL_Texture* loadTexture( char* path )
{
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load( path );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
        }

        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}

bool init()
{

    // Initialisation flag
    bool success = true;

    // Initialise SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not be initialised! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        // Create window
        gWindow = SDL_CreateWindow( "Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL)
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                // Initialise renderer colour
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                
                // Initialise PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialise! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
                else
                {
                    // Get window surface
                    gScreenSurface = SDL_GetWindowSurface( gWindow );
                }
            }
        }
    }

    return success;
}

bool loadMedia( SDL_Surface* *mediaSurface )
{
    bool success = true;
    
    mediaSurface[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface( "src/images/default.png" );
    if( mediaSurface[ KEY_PRESS_SURFACE_DEFAULT ] == NULL )
    {
        printf( "Failed to load default image!\n" );
        success = false;
    }

    mediaSurface[ KEY_PRESS_SURFACE_UP ] = loadSurface( "src/images/up.png" );
    if( mediaSurface[ KEY_PRESS_SURFACE_UP ] == NULL )
    {
        printf( "Failed to load up image!\n" );
        success = false;
    }

    mediaSurface[ KEY_PRESS_SURFACE_DOWN ] = loadSurface( "src/images/down.png" );
    if( mediaSurface[ KEY_PRESS_SURFACE_DOWN ] == NULL )
    {
        printf( "Failed to load down image!\n" );
        success = false;
    }

    mediaSurface[ KEY_PRESS_SURFACE_LEFT ] = loadSurface( "src/images/left.png" );
    if( mediaSurface[ KEY_PRESS_SURFACE_LEFT ] == NULL )
    {
        printf( "Failed to load left image!\n" );
        success = false;
    }

    mediaSurface[ KEY_PRESS_SURFACE_RIGHT ] = loadSurface( "src/images/right.png" );
    if( mediaSurface[ KEY_PRESS_SURFACE_RIGHT ] == NULL )
    {
        printf( "Failed to load right image!\n" );
        success = false;
    }

    return success;
}

void closeAll()
{
    // Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    // Start up SDL and create window
    if( !init() ){
        printf( "Failed to initialise!\n" );
    }
    else
    {
        //Main loop flag
        bool quit = false;

        //Event handler
        SDL_Event e;

        SDL_Surface* gStretchedSurface;

        // Image of corresponding keypress
        SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];
        
        if( !loadMedia( (SDL_Surface**) &gKeyPressSurfaces ) )
        {
            return 1;
        }

        gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];

        while( !quit )
        {
            while( SDL_PollEvent( &e ) != 0 )
            {
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
                else if( e.type == SDL_KEYDOWN )
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
                        gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
                        break;
                    }
                }
            }

            // Apply the image stretched
            SDL_Rect stretchRect;
            stretchRect.x = 0;
            stretchRect.y = 0;
            stretchRect.w = SCREEN_WIDTH;
            stretchRect.h = SCREEN_HEIGHT;
            SDL_BlitScaled( gCurrentSurface, NULL, gScreenSurface, &stretchRect);

            // // // Apply the image
            // SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, NULL );

            // Update the surface
            SDL_UpdateWindowSurface( gWindow );
        }

        SDL_FreeSurface( gCurrentSurface );
}

    // Free resources and close SDL
    closeAll();

    return 0;
}
