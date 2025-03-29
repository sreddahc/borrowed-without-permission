#pragma once

// LTexture "class"
typedef struct LTexture {
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
} LTexture;

extern bool LTexture_LoadImage( LTexture* self, SDL_Renderer* renderer, char* path );
extern bool LTexture_LoadText( LTexture* self, SDL_Renderer* renderer, char* textValue, TTF_Font* font, SDL_Color textColour );
extern void LTexture_SetBlendMode( LTexture* self, SDL_BlendMode blending );
extern void LTexture_SetColour( LTexture* self, Uint8 red, Uint8 green, Uint8 blue);
extern void LTexture_SetAlpha( LTexture* self, Uint8 alpha );
extern void LTexture_Render( LTexture* self, SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* centre, SDL_RendererFlip flip );
extern int LTexture_GetWidth( LTexture* self );
extern int LTexture_GetHeight( LTexture* self );
extern void LTexture_Free( LTexture* self );
