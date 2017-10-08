#include <iostream>
#include <fstream>
#include <sstream>

#include "../include/cursor.h"
#include "../include/node.h"
#include "../include/game.h"
#include "../include/sdl_helpers.h"

/**
 * Game class default constructor. DON'T USE!
 */
game::game()
{
	std::cout << RED << "Game class default constructor called. A pointer to an SDL renderer is required!" << NC << std::endl;
}

/**
 * The non-default constructor of the game class.
 * @param r A pointer to the renderer.
 * @param g A struct of game options.
 */
game::game( SDL_Renderer* r, game_options g ) :
	renderer( r ),
	board( ::board( g.rows, g.cols, g.node_width, g.flags.grid_on ) ), timer( ::timer() ),
	current_ups( g.init_ups ),
	opts( g ),
	mouse_coords({{1,0},{0,0}})
{
	if ( g.flags.file_board ) board = g.load_board;
}

/**
 * Starts the game's main loop.
 */
void game::run ()
{
	// this hides the console cursor so it doesnt jump around while logging
	system( "setterm -cursor off" );

	SDL_Event e;
	bool quit = false;

	timer.pause();

	while ( !quit )
	{
		update_mouse_pos();
		
		// check for input events
		while ( SDL_PollEvent( &e ) ) handle_keybinds( e, quit );

		// clear the screen to the color based on if it is paused
		if ( timer.is_paused() ) SDL_SetRenderDrawColor( renderer, opts.paused_bg.r, opts.paused_bg.g, opts.paused_bg.b, opts.paused_bg.a );
		else SDL_SetRenderDrawColor( renderer, opts.default_bg.r, opts.default_bg.g, opts.default_bg.b, opts.default_bg.a );
		SDL_RenderClear( renderer );
		
		// add the live nodes
		board.render( renderer, opts.node_fg, opts.cursor_fg, opts.grid, opts.shadow );
		
		// update what the user can see
		SDL_RenderPresent( renderer );
		
		// update the board if unpaused and the time since last update is over the updates per second time
		if ( !timer.is_paused() && ( timer.get_ticks() >= ( 1000.f / current_ups ) ) )
		{
			board.update();
			// reset the time until next update
			timer.start();
		}
		
		// log some stuff to the console
		log();
	}

	// make sure to re-enable it
	system( "setterm -cursor on" );
}

/**
 * Compares two coordinate structs.
 * @param first  The first coordinate.
 * @param second The second coordinate.
 * @return       True if they are different, false if they are the same.
 */
bool operator!=( const coordinate& first, const coordinate& second )
{
	if ( first.x != second.x || first.y != second.y ) return true;
	return false;
}

/**
 * Checks an SDL_Event for mouse and kb input, then alters game parameters.
 * @param e The SDL_Event.
 */
void game::handle_keybinds ( SDL_Event& e, bool& quit )
{
	switch ( e.type )
	{
		// clicking can toggle nodes
		case SDL_MOUSEBUTTONDOWN:
			// only try to toggle nodes inside the board
			if ( mouse_coords[0].x < board.get_width() && mouse_coords[0].y < board.get_height() )
			{
				// hides the cursor
				board.get_cursor().set_alive( false );
				// moves it (why isnt this one function?)
				board.get_cursor().set_x_pos( mouse_coords[0].x * board.get_node_width() );
				board.get_cursor().set_y_pos( mouse_coords[0].y * board.get_node_width() );
				// toggles the node using the cursor
				board.use_cursor();
			}
			break;
		// click and drag functionality
		case SDL_MOUSEMOTION:
			if ( mouse_coords[0] != mouse_coords[1]    && e.motion.state & SDL_BUTTON( SDL_BUTTON_LEFT ) &&
			     mouse_coords[0].x < board.get_width() && mouse_coords[0].y < board.get_height() )
			{
				board.get_cursor().set_alive( false );
				board.get_cursor().set_x_pos( mouse_coords[0].x * board.get_node_width() );
				board.get_cursor().set_y_pos( mouse_coords[0].y * board.get_node_width() );
				board.use_cursor();
				mouse_coords[1] = mouse_coords[0];
			}
			break;
		case SDL_KEYDOWN:
			switch ( e.key.keysym.sym )
			{
				// q quits the game
				case SDLK_q:
					quit = true;
					break;
				// space pauses the game
				case SDLK_SPACE:
					if ( current_ups == 0 ) current_ups = opts.init_ups;
					if ( !timer.is_paused() ) timer.pause();
					else timer.unpause();
					break;
				// backspace clears the board
				case SDLK_BACKSPACE:
					board.clear_board();
					break;
				// ] increases updates per second
				case SDLK_RIGHTBRACKET:
					current_ups++;
					if ( current_ups > 0 ) timer.unpause();
					break;
				// [ decreases updates per second
				case SDLK_LEFTBRACKET:
					if ( current_ups != 0 ) current_ups--;
					if ( current_ups == 0 ) timer.pause();
					break;
				// \ resets the updates per second
				case SDLK_BACKSLASH:
					current_ups = opts.init_ups;
					timer.unpause();
					break;
				// . steps the simulation when paused
				case SDLK_PERIOD:
					if ( timer.is_paused() ) board.update();
					break;
				// the arrow keys move the cursor
				case SDLK_UP:
				case SDLK_DOWN:
				case SDLK_LEFT:
				case SDLK_RIGHT:
					board.move_cursor( e );
					break;
				// t and return toggle the life of a node
				case SDLK_t:
				case SDLK_RETURN:
					board.use_cursor();
					break;
				// p toggles paint mode (you dont have to press t/return each time you move)
				case SDLK_p:
					board.toggle_paint();
					break;
				// g toggles displaying a node grid on the board.
				case SDLK_g:
					board.toggle_grid();
					break;
				case SDLK_s:
					to_file();
					break;
				// r resets the board to the stored preset if one was loaded
				case SDLK_r:
					if ( opts.flags.file_board ) board = opts.load_board;
					break;
			}
			break;
		case SDL_QUIT:
			quit = true;
			break;
	}
}

Uint32 game::update_mouse_pos ()
{
	int x, y;
	Uint32 state = SDL_GetMouseState( &x, &y );
	mouse_coords[0].x = x / board.get_node_width();
	mouse_coords[0].y = y / board.get_node_width();

	return state;
}

/**
 * Prints useful information about the current game to the console.
 */
void game::log ()
{
	std::stringstream board_size, mouse_pos, cursor_pos, live_nodes, update_str;
	std::cout << std::endl;
	board_size << board.get_cols() << " x " << board.get_rows();
	mouse_pos << "[" << mouse_coords[0].x << ", " << mouse_coords[0].y << "]";
	cursor_pos << "[" << board.get_cursor().get_x_pos() / board.get_node_width() << ", " << board.get_cursor().get_y_pos() / board.get_node_width() << "]";
	live_nodes << board.count_live_nodes();
	update_str << ( timer.is_paused() ? "\033[0;31mpaused\033[0m" : std::to_string( (int) ( ( 1000.f / current_ups ) - timer.get_ticks() ) ) );

	std::cout << WHITE << "Board Size" << NC << " -------- " << GREEN << board_size.str()       << NC << "                       \n";
	std::cout << WHITE << "Mouse Position" << NC << " ---- " << GREEN << mouse_pos.str()        << NC << "                       \n";
	std::cout << WHITE << "Cursor Position" << NC << " --- " << GREEN << cursor_pos.str()       << NC << "                       \n";
	std::cout << WHITE << "Live Nodes" << NC << " -------- " << GREEN << live_nodes.str()       << NC << "                       \n";
	std::cout << WHITE << "Generation" << NC << " -------- " << GREEN << board.get_generation() << NC << "                       \n";
	std::cout << WHITE << "MS Until Update" << NC << " --- " << GREEN << update_str.str()       << NC << "                       \n";
	std::cout << WHITE << "Current UPS" << NC << " ------- " << GREEN << current_ups            << NC << "                       ";
	std::cout << std::endl;
	std::cout << "\r\e[A\e[A\e[A\e[A\e[A\e[A\e[A\e[A" << std::flush;
}

std::string Uint32_to_hex_string ( Uint32 i )
{
	std::stringstream temp;
	temp << std::hex << i;
	return temp.str();
}

/**
 * Saves the game variables and board state to a file.
 */
void game::to_file ()
{
	std::string filename = "./";
	filename += std::to_string( opts.rows ) + "x" + std::to_string( opts.cols ) + "-" + std::to_string( time( NULL ) ) + ".txt";
	std::ofstream out( filename, std::ofstream::out | std::ofstream::trunc );
	out << "[variables]\n"
	    << "rows = "       << opts.rows       << "\n"
	    << "cols = "       << opts.cols       << "\n"
	    << "node_width = " << opts.node_width << "\n"
	    << "init_ups = "   << opts.init_ups   << "\n"
	    << "grid_on = "    << opts.flags.grid_on << "\n"
	    << "verbose = "    << opts.flags.verbose << "\n"
	    << "\n"
	    << "[colors]\n"
	    << "node_fg    = 0x" << Uint32_to_hex_string( SDL_Color_to_Uint32( opts.node_fg ) ) << "\n"
	    << "cursor_fg  = 0x" << Uint32_to_hex_string( SDL_Color_to_Uint32( opts.cursor_fg ) ) << "\n"
	    << "grid       = 0x" << Uint32_to_hex_string( SDL_Color_to_Uint32( opts.grid ) ) << "\n"
	    << "shadow     = 0x" << Uint32_to_hex_string( SDL_Color_to_Uint32( opts.shadow ) ) << "\n"
	    << "default_bg = 0x" << Uint32_to_hex_string( SDL_Color_to_Uint32( opts.default_bg ) ) << "\n"
	    << "paused_bg  = 0x" << Uint32_to_hex_string( SDL_Color_to_Uint32( opts.paused_bg ) ) << "\n"
	    << "\n"
	    << "[board]"
	    << std::endl;
	for ( int i = 0; i < board.get_rows(); i++ )
	{
		for ( int j = 0; j < board.get_cols(); j++ )
		{
			if ( board.get_node( i, j ).is_alive() ) out << "1";
			else out << "0";
		}
		out << std::endl;
	}
	out.close();
	printf( "Saved board to: %s\n", filename.c_str() );
}
