#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Global variables
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

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;

// Functions
bool init();
void closeAll();
SDL_Texture* loadFromFile( char* path );

// LTexture "class"
struct LTexture {
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};
void LTexture_LoadImage( struct LTexture* self, char* path );
void LTexture_Render( struct LTexture* self, int x, int y );
int LTexture_GetWidth( struct LTexture* self );
int LTexture_GetHeight( struct LTexture* self );
void LTexture_Free( struct LTexture* self );


void LTexture_LoadImage( struct LTexture* self, char* path ){
    
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
        else
        {
            //Color key image
            SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0x00, 0xFF ) );

            self->mWidth = loadedSurface->w;
            self->mHeight = loadedSurface->h;
        }

        SDL_FreeSurface( loadedSurface );
    }

    self->mTexture = newTexture;
}

void LTexture_Render( struct LTexture* self, int x, int y )
{
    SDL_Rect renderZone = { x, y, self->mWidth, self->mHeight };
    SDL_RenderCopy( gRenderer, self->mTexture, NULL, &renderZone );
}

int LTexture_GetWidth( struct LTexture* self )
{
    return self->mWidth;
}

int LTexture_GetHeight( struct LTexture* self )
{
    return self->mHeight;
}

void LTexture_Free( struct LTexture* self )
{
    if( self->mTexture != NULL )
    {
        self->mTexture = NULL;
        self->mWidth = 0;
        self->mHeight = 0;
    }
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

        struct LTexture gBackground;
        LTexture_LoadImage( &gBackground, "src/images/landscape.png" );

        struct LTexture gHuman;
        LTexture_LoadImage( &gHuman, "src/images/human.png" );

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

            LTexture_Render( &gBackground, 0, 0 );

            LTexture_Render( &gHuman, 400, 200 );

            //Update screen
            SDL_RenderPresent( gRenderer );
        }

        LTexture_Free( &gBackground );
        LTexture_Free( &gHuman );

    }

    // Free resources and close SDL
    closeAll();

    return 0;
}
