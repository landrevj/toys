#ifndef SDL_HELPERS
#define SDL_HELPERS

#include <SDL2/SDL.h>

bool init ( unsigned int width, unsigned int height, SDL_Window*& window, SDL_Renderer*& renderer );
void close ( SDL_Window*& window, SDL_Renderer*& renderer );
SDL_Color Uint32_to_SDL_Color ( Uint32 i );
Uint32 SDL_Color_to_Uint32 ( const SDL_Color& c );

#endif
