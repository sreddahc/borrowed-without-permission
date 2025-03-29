#ifndef GAME_H
#define GAME_H

// LTexture "class"
struct LTexture {
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};

extern bool LTexture_LoadImage( struct LTexture* self, SDL_Renderer* renderer, char* path );
extern bool LTexture_LoadText( struct LTexture* self, SDL_Renderer* renderer, char* textValue, TTF_Font* font, SDL_Color textColour );
extern void LTexture_SetBlendMode( struct LTexture* self, SDL_BlendMode blending );
extern void LTexture_SetColour( struct LTexture* self, Uint8 red, Uint8 green, Uint8 blue);
extern void LTexture_SetAlpha( struct LTexture* self, Uint8 alpha );
extern void LTexture_Render( struct LTexture* self, SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* centre, SDL_RendererFlip flip );
extern int LTexture_GetWidth( struct LTexture* self );
extern int LTexture_GetHeight( struct LTexture* self );
extern void LTexture_Free( struct LTexture* self );

#endif