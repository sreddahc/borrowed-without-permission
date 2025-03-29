#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "renderer/renderer.h"

// Global variables
// Screen dimensions
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 768;

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
TTF_Font* gFont = NULL;

// Functions
bool init();
void closeAll();


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
        gWindow = SDL_CreateWindow( "Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            // Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                // Initialise Renderer Colour
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                // Initialise PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialise! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }

                // Initialise TTF loading
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialise! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}


void closeAll()
{
    // Free Loaded Images
    SDL_DestroyTexture( gTexture );
    gTexture = NULL;
    
    // Destroy Window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    TTF_Quit();
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
        // Main loop flag
        bool quit = false;

        // Event handler
        SDL_Event e;

        // Background
        struct LTexture gBackground;
        LTexture_LoadImage( &gBackground, gRenderer, "src/images/backgrounds/day.png" );

        // Text
        gFont = TTF_OpenFont( "src/assets/fonts/dejavu/DejaVuSerif.ttf", 28 );
        if( gFont == NULL )
        {
            printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
            quit = true;
        }
        SDL_Color textColour = { 0, 0, 0 };
        struct LTexture gText;
        if( !( LTexture_LoadText( &gText, gRenderer, "New game!", gFont, textColour ) ) )
        {
            printf( "Failed to render texture!\n" );
            quit = true;
        }

        // Sprite
        const int WALKING_ANIMATION_FRAMES = 4;
        const int SPRITE_HEIGHT = 32;
        const int SPRITE_WIDTH = 32;
        SDL_Rect gSprite[ WALKING_ANIMATION_FRAMES ];
        // Controls
        int frame = 0;
        SDL_RendererFlip flipType = SDL_FLIP_NONE;
        double degrees = 0;
        double angle_increment = 30;

        struct LTexture gSpriteSheet;
        if( !( LTexture_LoadImage( &gSpriteSheet, gRenderer, "src/images/sprites/player.png" ) ) )
        {
            printf( "Failed to load walking animation texture.\n" );
        }
        else
        {
            // Set sprite animation frames
            // An assumption is being made that the frames go from left to right only.
            for( int f = 0; f < WALKING_ANIMATION_FRAMES; f++ )
            {
                gSprite[ f ].x = f * SPRITE_WIDTH;
                gSprite[ f ].y = 0;
                gSprite[ f ].w = SPRITE_WIDTH;
                gSprite[ f ].h = SPRITE_HEIGHT;
            }
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
                    
                    // No flip
                    case SDLK_s:
                        flipType = SDL_FLIP_NONE;
                        break;

                    // Flip horizontally
                    case SDLK_a:
                        flipType = SDL_FLIP_HORIZONTAL;
                        break;
                    
                    // Flip vertically
                    case SDLK_w:
                        flipType = SDL_FLIP_VERTICAL;
                        break;

                    // Rotate clockwise
                    case SDLK_RIGHT:
                        degrees += angle_increment;
                        if( degrees >= 360 )
                        {
                            degrees -= 360;
                        }
                        break;
                    
                    // Rotate anti-clockwise
                    case SDLK_LEFT:
                        degrees -= angle_increment;
                        if( degrees < 0 )
                        {
                            degrees += 360;
                        }
                        break;
                    
                    // Reset angle
                    case SDLK_DOWN:
                        degrees = 0;
                        break;

                    default:
                        break;
                    }
                }
            }

            // Update the surface
            SDL_RenderClear( gRenderer );
            
            // Render Background
            LTexture_Render( &gBackground, gRenderer, 0, 0, NULL, 0.0, NULL, SDL_FLIP_NONE );

            // Render Text
            LTexture_Render( &gText, gRenderer, 0, 0, NULL, 0.0, NULL, SDL_FLIP_NONE );

            // Render Sprite
            SDL_Rect* gSpriteFrame = &gSprite[ frame ];
            LTexture_Render( &gSpriteSheet, gRenderer, 235, 235, gSpriteFrame, degrees, NULL, flipType );
            frame = ( frame + 1 ) % WALKING_ANIMATION_FRAMES;

            // Update screen
            SDL_RenderPresent( gRenderer );
        }

        LTexture_Free( &gText );
        LTexture_Free( &gSpriteSheet );
        LTexture_Free( &gBackground );

    }

    // Free resources and close SDL
    closeAll();

    return 0;
}
