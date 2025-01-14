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
SDL_Texture* loadTexture( char* path );

// Global variables
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;


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
            }
        }
    }

    return success;
}

bool loadMedia( SDL_Texture* *mediaTexture )
{
    bool success = true;
    
    mediaTexture[ KEY_PRESS_SURFACE_DEFAULT ] = loadTexture( "src/images/default.png" );
    if( mediaTexture[ KEY_PRESS_SURFACE_DEFAULT ] == NULL )
    {
        printf( "Failed to load default image!\n" );
        success = false;
    }

    mediaTexture[ KEY_PRESS_SURFACE_UP ] = loadTexture( "src/images/up.png" );
    if( mediaTexture[ KEY_PRESS_SURFACE_UP ] == NULL )
    {
        printf( "Failed to load up image!\n" );
        success = false;
    }

    mediaTexture[ KEY_PRESS_SURFACE_DOWN ] = loadTexture( "src/images/down.png" );
    if( mediaTexture[ KEY_PRESS_SURFACE_DOWN ] == NULL )
    {
        printf( "Failed to load down image!\n" );
        success = false;
    }

    mediaTexture[ KEY_PRESS_SURFACE_LEFT ] = loadTexture( "src/images/left.png" );
    if( mediaTexture[ KEY_PRESS_SURFACE_LEFT ] == NULL )
    {
        printf( "Failed to load left image!\n" );
        success = false;
    }

    mediaTexture[ KEY_PRESS_SURFACE_RIGHT ] = loadTexture( "src/images/right.png" );
    if( mediaTexture[ KEY_PRESS_SURFACE_RIGHT ] == NULL )
    {
        printf( "Failed to load right image!\n" );
        success = false;
    }

    return success;
}

void closeAll()
{
    //Free Loaded Images
    SDL_DestroyTexture( gTexture );
    gTexture = NULL;
    
    // Destroy Window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
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
        SDL_Texture* gKeyPressTexture[ KEY_PRESS_SURFACE_TOTAL ];
        
        if( !loadMedia( (SDL_Texture**) &gKeyPressTexture ) )
        {
            return 1;
        }

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

                    default:
                        break;
                    }
                }
            }

            // Update the surface
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );

            // Top Left Viewport
            SDL_Rect topLeftViewport;
            topLeftViewport.x = 0;
            topLeftViewport.y = 0;
            topLeftViewport.w = SCREEN_WIDTH / 2;
            topLeftViewport.h = SCREEN_HEIGHT / 2;
            SDL_RenderSetViewport( gRenderer, &topLeftViewport );
            SDL_RenderCopy( gRenderer, gKeyPressTexture[ KEY_PRESS_SURFACE_LEFT ], NULL, NULL );

            // Top Right Viewport
            SDL_Rect topRightViewport;
            topRightViewport.x = SCREEN_WIDTH / 2;
            topRightViewport.y = 0;
            topRightViewport.w = SCREEN_WIDTH / 2;
            topRightViewport.h = SCREEN_HEIGHT / 2;
            SDL_RenderSetViewport( gRenderer, &topRightViewport );
            SDL_RenderCopy( gRenderer, gKeyPressTexture[ KEY_PRESS_SURFACE_RIGHT ], NULL, NULL );

            // Bottom
            SDL_Rect bottomViewport;
            bottomViewport.x = 0;
            bottomViewport.y = SCREEN_HEIGHT / 2;
            bottomViewport.w = SCREEN_WIDTH;
            bottomViewport.h = SCREEN_HEIGHT / 2;
            SDL_RenderSetViewport( gRenderer, &bottomViewport );
            SDL_RenderCopy( gRenderer, gKeyPressTexture[ KEY_PRESS_SURFACE_DOWN ], NULL, NULL );

            //Update screen
            SDL_RenderPresent( gRenderer );
        }

        // Free texture resources
        for( int textureId = 0; textureId < KEY_PRESS_SURFACE_TOTAL; textureId++  )
        {
            SDL_DestroyTexture( gKeyPressTexture[ textureId ] );
            gKeyPressTexture[ textureId ] = NULL;
        }

    }

    // Free resources and close SDL
    closeAll();

    return 0;
}
