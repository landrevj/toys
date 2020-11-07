#include <SDL2/SDL.h>

#include "../include/arg_helpers.h"
#include "../include/sdl_helpers.h"
#include "../include/game.h"

/**
 * Main function. Initializes the SDL window and game object.
 * @param  argc Number of input args.
 * @param  argv Array of args as c-style strings.
 * @return      Exit code (int).
 */
int main ( int argc, char* const* argv )
{
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	game_options go = {{false, false, false, false, false, false, false, false, false, false, false, false, false}};
	// default colors
	go.node_fg    = Uint32_to_SDL_Color( 0x787878ff );
	go.cursor_fg  = Uint32_to_SDL_Color( 0x939393ff );
	go.grid       = Uint32_to_SDL_Color( 0x404040ff );
	go.shadow     = Uint32_to_SDL_Color( 0x404040ff );
	go.default_bg = Uint32_to_SDL_Color( 0x505050ff );
	go.paused_bg  = Uint32_to_SDL_Color( 0x313131ff );

	// parse config file
	parse_file( "./config", go );

	// handle command line args
	handle_args( go, argc, argv );

	// initialize sdl window and renderer
	if ( !init( go.cols * go.node_width, go.rows * go.node_width, window, renderer ) )
	{
		std::exit( 1 );
	}

	// create game and run
	game g( renderer, go );
	g.run();

	// clean up
	close( window, renderer );

	return 0;
}
