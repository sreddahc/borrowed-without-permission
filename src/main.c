#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

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

// Functions
bool init();
void closeAll();

// LTexture "class"
struct LTexture {
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};
int LTexture_LoadImage( struct LTexture* self, char* path );
void LTexture_SetBlendMode( struct LTexture* self, SDL_BlendMode blending );
void LTexture_SetColour( struct LTexture* self, Uint8 red, Uint8 green, Uint8 blue);
void LTexture_SetAlpha( struct LTexture* self, Uint8 alpha );
void LTexture_Render( struct LTexture* self, int x, int y, SDL_Rect* clip, double angle, SDL_Point* centre, SDL_RendererFlip flip );
int LTexture_GetWidth( struct LTexture* self );
int LTexture_GetHeight( struct LTexture* self );
void LTexture_Free( struct LTexture* self );


int LTexture_LoadImage( struct LTexture* self, char* path ){
    
    bool success = true;
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load( path );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
        success = false;
    }
    else
    {
        self->mWidth = loadedSurface->w;
        self->mHeight = loadedSurface->h;

        // Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0x00, 0xFF ) );
        
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
            success = false;
        }

        SDL_FreeSurface( loadedSurface );
    }

    self->mTexture = newTexture;

    return success;
}


void LTexture_SetBlendMode( struct LTexture* self, SDL_BlendMode blending )
{
    SDL_SetTextureBlendMode( self->mTexture, blending );
}


void LTexture_SetColour( struct LTexture* self, Uint8 red, Uint8 green, Uint8 blue)
{
    SDL_SetTextureColorMod( self->mTexture, red, green, blue );
}


void LTexture_SetAlpha( struct LTexture* self, Uint8 alpha )
{
    SDL_SetTextureAlphaMod( self->mTexture, alpha );
}


void LTexture_Render( struct LTexture* self, int x, int y, SDL_Rect* clip, double angle, SDL_Point* centre, SDL_RendererFlip flip )
{
    // Set rendering space and render to screen
    SDL_Rect renderZone = { x, y, self->mWidth, self->mHeight };
    
    // Set clip dimensions
    if( clip != NULL )
    {
        renderZone.w = clip->w;
        renderZone.h = clip->h;
    }

    // Render to screen
    SDL_RenderCopyEx( gRenderer, self->mTexture, clip, &renderZone, angle, centre, flip );
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
        SDL_DestroyTexture(self->mTexture);
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

        struct LTexture gBackground;
        LTexture_LoadImage( &gBackground, "src/images/backgrounds/day.png" );

        // Sprite
        const int WALKING_ANIMATION_FRAMES = 4;
        const int SPRITE_HEIGHT = 32;
        const int SPRITE_WIDTH = 32;
        SDL_Rect gSprite[ WALKING_ANIMATION_FRAMES ];
        
        struct LTexture gSpriteSheet;
        if( !( LTexture_LoadImage( &gSpriteSheet, "src/images/sprites/player.png" ) ) )
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

        int frame = 0;
        SDL_RendererFlip flipType = SDL_FLIP_NONE;
        double degrees = 0;
        double angle_increment = 30;

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
            LTexture_Render( &gBackground, 0, 0, NULL, 0.0, NULL, SDL_FLIP_NONE );

            // Render Sprite
            SDL_Rect* gSpriteFrame = &gSprite[ frame ];
            LTexture_Render( &gSpriteSheet, 235, 235, gSpriteFrame, degrees, NULL, flipType );
            frame = ( frame + 1 ) % WALKING_ANIMATION_FRAMES;

            // Update screen
            SDL_RenderPresent( gRenderer );
        }

        LTexture_Free( &gSpriteSheet );
        LTexture_Free( &gBackground );

    }

    // Free resources and close SDL
    closeAll();

    return 0;
}
