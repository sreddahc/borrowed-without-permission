#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "renderer.h"


bool LTexture_LoadImage( struct LTexture* self, SDL_Renderer* renderer, char* path ){
    
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
        
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
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


#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture_LoadText( struct LTexture* self, SDL_Renderer* renderer, char* textValue, TTF_Font* font, SDL_Color textColour )
{
    bool success = true;

    SDL_Surface* textSurface = TTF_RenderText_Solid( font, textValue, textColour );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        // Create texture from surface pixels
        self->mTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
        if( self->mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            // Get image dimensions
            self->mWidth = textSurface->w;
            self->mHeight = textSurface->h;
        }
        // Clean up old surface
        SDL_FreeSurface( textSurface );
    }
    
    return success;
}
#endif


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


void LTexture_Render( struct LTexture* self, SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* centre, SDL_RendererFlip flip )
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
    SDL_RenderCopyEx( renderer, self->mTexture, clip, &renderZone, angle, centre, flip );
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