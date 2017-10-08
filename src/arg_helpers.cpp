#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <getopt.h>

#include "../include/arg_helpers.h"
#include "../include/sdl_helpers.h"

/**
 * Determines if a c-style string is a positive int.
 * @param  a The c-style string to check.
 * @return   Either the unsigned int or 0 if it isn't.
 */
unsigned int is_positive_int ( const char a[] )
{
	for ( int i = 0; i < std::strlen( a ); i++ )
	{
		if ( a[i] < '0' || a[i] > '9' )
			return 0;
	}
	return atoi( a );
}

/**
 * Returns a copy of a string but without spaces.
 * @param  line The string to filter.
 * @return      The copy without spaces.
 */
std::string strip_spaces ( const std::string& line )
{
	std::string result;
	for ( int i = 0; i < line.length(); i++ )
		if ( line[i] != ' ' )
			result += line[i];
	return result;
}

/**
 * Extracts game options from argv.
 * @param go   Struct of game options.
 * @param  argc Number of input args.
 * @param  argv Array of args as c-style strings.
 */
void handle_args ( game_options& go, int argc, char* const* argv )
{
	int opt;
	while ( ( opt = getopt( argc, argv, "f:r:c:w:u:hgv" ) ) != -1 )
	{
		switch ( opt )
		{
			case 'f':
				parse_file( optarg, go );
				go.flags.file = true;
				break;
			case 'r':
				go.rows = is_positive_int( optarg );
				if ( go.rows == 0 )
				{
					std::cerr << WHITE << "\nRows (-r) option must be a positive non-zero int" << NC << ": " << RED << optarg << NC << "\n" << std::endl;
					std::exit( 2 );
				}
				go.flags.rows = true;
				break;
			// define column number
			case 'c':
				go.cols = is_positive_int( optarg );
				if ( go.cols == 0 )
				{
					std::cerr << WHITE << "\nColumns (-c) option must be a positive non-zero int" << NC << ": " << RED << optarg << NC << "\n" << std::endl;
					std::exit( 3 );
				}
				go.flags.cols = true;
				break;
			// define node width
			case 'w':
				go.node_width = is_positive_int( optarg );
				if ( go.node_width == 0 )
				{
					std::cerr << WHITE << "\nNode width (-w) option must be a positive non-zero int" << NC << ": " << RED << optarg << NC << "\n" << std::endl;
					std::exit( 4 );
				}
				go.flags.node_width = true;
				break;
			// define updates per second
			case 'u':
				go.init_ups = is_positive_int( optarg );
				if ( go.init_ups == 0 )
				{
					std::cerr << WHITE << "\nInitial Updates Per Second (-u) option must be a positive non-zero int" << NC << ": " << RED << optarg << NC << "\n" << std::endl;
					std::exit( 5 );
				}
				go.flags.init_ups = true;
				break;
			// display help text
			case 'h':
				show_help();
				std::exit( 0 );
				break;
			// enable grid mode by default
			case 'g':
				go.flags.grid_on = true;
				break;
			case 'v':
				go.flags.verbose = true;
				break;
		}
	}

	// defaults
	if ( !go.flags.rows )
	{
		go.rows = 50;
		if ( go.flags.verbose ) std::cout << YELLOW << "Rows (-r) defaulted to" << NC << ": " << WHITE << go.rows << NC << std::endl;
	}
	if ( !go.flags.cols )
	{
		go.cols = 50;
		if ( go.flags.verbose ) std::cout << YELLOW << "Columns (-c) defaulted to" << NC << ": " << WHITE << go.cols << NC << std::endl;
	}
	if ( !go.flags.node_width )
	{
		go.node_width = 10;
		if ( go.flags.verbose ) std::cout << YELLOW << "Node Width (-w) defaulted to" << NC << ": " << WHITE << go.node_width << NC << std::endl;
	}
	if ( !go.flags.init_ups )
	{
		go.init_ups = 5;
		if ( go.flags.verbose ) std::cout << YELLOW << "Initial Updates Per Second (-u) defaulted to" << NC << ": " << WHITE << go.init_ups << NC << std::endl;
	}
}

/**
 * Display program help text to console.
 */
void show_help ()
{
	std::cout << "\nAn implementation of Conway's Game of life, made using SDL2." << "\n\n"
	          << "Usage: life [-hvg] [-r integer > 0] [-c integer > 0] [-w integer > 0] [-u integer > 0] [-f filepath]" << "\n\n"
	          << "Options:" << "\n"
	          << "\t-h Show this text." << "\n"
	          << "\n"
	          << "\t-f Load a save/options file with a relative file path." << "\n"
	          << "\t   Options passed after this one will override what is set in the file," << "\n"
	          << "\t   excluding (for now) -r -c and -w." << "\n"
	          << "\n"
	          << "\t-v Enable verbose logging." << "\n"
	          << "\t-g Start the program with grid mode enabled." << "\n"
	          << "\n"
	          << "\t-r Sets the number of rows." << "\n"
	          << "\t-c Sets the number of columns." << "\n"
	          << "\t-w Sets the pixel width of the nodes." << "\n"
	          << "\t-u Sets the updates per second." << "\n"
	          << std::endl;
}


/**
 * Copies game options and board layout from a save file.
 * @param filename The name of the file to parse.
 * @param go       The game options struct.
 */
void parse_file ( std::string filename, game_options& go )
{
	std::unordered_map<std::string, std::string> options;
	std::ifstream in( filename );
	std::string line;

	while ( getline( in, line ) )
	{
		if ( strip_spaces( line ) == "[board]" ) break;

		if ( line[0] != '#' && ( line.find_first_not_of( ' ' ) != std::string::npos ) )
		{
			std::string results[2];
			parse_line( line, results );
			options.insert( {results[0], results[1]} );
		}
	}
	set_options_from_file( go, options );

	if ( strip_spaces( line ) == "[board]" )
	{
		go.flags.file_board = true;
		go.load_board = board( go.rows, go.cols, go.node_width, go.flags.grid_on );
		for ( int i = 0; i < go.rows; i++ )
		{
			getline( in, line );
			for ( int j = 0; j < go.cols; j++ )
			{
				go.load_board.get_node( i, j ).set_alive( j >= line.length() ? 0 : std::stoi( line.substr( j, 1 ) ) );
			}
		}
	}

	in.close();
}

/**
 * Exctracts lvalue and rvalue from a string.
 * @param line    The line to parse.
 * @param results The array in which to store the results.
 */
void parse_line ( const std::string& line, std::string results[2] )
{
	bool hit_equals = false;
	for ( int i = 0; i < line.length(); i++ )
	{
		if ( line[i] == '=' ) hit_equals = true;
		else
		{
			if ( !hit_equals && line[i] != ' ' ) results[0] += line[i];
			if (  hit_equals && line[i] != ' ' ) results[1] += line[i];
		}
	}
}

/**
 * Sets the options and flags from the options map.
 * @param go      The game_options struct.
 * @param options The options map. Typically created while parsing a file.
 */
void set_options_from_file ( game_options& go, std::unordered_map<std::string, std::string>& options )
{
	// holy fuck this thing is ugly
	for ( std::unordered_map<std::string, std::string>::iterator it = options.begin(); it != options.end(); it++ )
	{
		if ( it->first == "rows" )       { go.rows       = std::stoi( options["rows"] );       go.flags.rows = true; }
		if ( it->first == "cols" )       { go.cols       = std::stoi( options["cols"] );       go.flags.cols = true; }
		if ( it->first == "node_width" ) { go.node_width = std::stoi( options["node_width"] ); go.flags.node_width = true; }
		if ( it->first == "init_ups" )   { go.init_ups   = std::stoi( options["init_ups"] );   go.flags.init_ups = true; }

		if ( it->first == "node_fg" )    { go.node_fg    = Uint32_to_SDL_Color( std::stoul( options["node_fg"], NULL, 16 ) );    go.flags.node_fg = true; }
		if ( it->first == "cursor_fg" )  { go.cursor_fg  = Uint32_to_SDL_Color( std::stoul( options["cursor_fg"], NULL, 16 ) );  go.flags.cursor_fg = true; }
		if ( it->first == "grid" )       { go.grid       = Uint32_to_SDL_Color( std::stoul( options["grid"], NULL, 16 ) );       go.flags.grid = true; }
		if ( it->first == "shadow" )     { go.shadow     = Uint32_to_SDL_Color( std::stoul( options["shadow"], NULL, 16 ) );     go.flags.shadow = true; }
		if ( it->first == "default_bg" ) { go.default_bg = Uint32_to_SDL_Color( std::stoul( options["default_bg"], NULL, 16 ) ); go.flags.default_bg = true; }
		if ( it->first == "paused_bg" )  { go.paused_bg  = Uint32_to_SDL_Color( std::stoul( options["paused_bg"], NULL, 16 ) );  go.flags.paused_bg = true; }

		if ( it->first == "file" )    { go.flags.file    = std::stoi( options["file"] );    go.flags.file = true; }
		if ( it->first == "grid_on" ) { go.flags.grid_on = std::stoi( options["grid_on"] ); }
		if ( it->first == "verbose" ) { go.flags.verbose = std::stoi( options["verbose"] ); }
	}

}
