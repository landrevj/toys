#include <stdio.h>
#include "../include/sdl_helpers.h"
/**
 * Initializes SDL along with a window and renderer.
 * @param  width    The pixel width of the SDL_Window.
 * @param  height   The pixel height of the SDL_Window.
 * @param  window   A reference to a pointer to an SDL_Window.
 * @param  renderer A reference to a pointer to an SDL_Renderer.
 * @return          True/false based on the success of initialization.
 */
bool init ( unsigned int width, unsigned int height, SDL_Window*& window, SDL_Renderer*& renderer )
{
	// initialize SDL
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize.\nError: %s\n", SDL_GetError() );
		return false;
	}

	// set texture filtering to linear
	if ( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
	{
		printf( "Failed to enable linear texture filtering." );
		return false;
	}

	// create window
	window = SDL_CreateWindow( "life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN );
	if ( window == NULL )
	{
		printf( "Window could not be created.\nError: %s\n", SDL_GetError() );
		return false;
	}

	// create renderer for window
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if ( renderer == NULL )
	{
		printf( "Renderer could not be created.\nError: %s\n", SDL_GetError() );
		return false;
	}

	// initialize renderer color
	SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

	return true;
}

/**
 * Cleans up a bit.
 * @param  window   A reference to a pointer to an SDL_Window.
 * @param  renderer A reference to a pointer to an SDL_Renderer.
 */
void close ( SDL_Window*& window, SDL_Renderer*& renderer )
{
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;

	SDL_Quit();
}

/**
 * Converts a Uint32 to an SDL_Color.
 * @param  i The Uint32 to convert.
 * @return   The SDL_Color.
 */
SDL_Color Uint32_to_SDL_Color ( Uint32 i )
{
	SDL_Color result;
	result.r = i >> 24;
	result.g = i >> 16;
	result.b = i >> 8;
	result.a = i;
	return result;
}

/**
 * Converts an SDL_Color to a Uint32.
 * @param  c The SDL_Color to convert.
 * @return   The Uint32.
 */
Uint32 SDL_Color_to_Uint32 ( const SDL_Color& c )
{
	return ( c.r << 24 ) | ( c.g << 16 ) | ( c.b << 8 ) | c.a;
}
