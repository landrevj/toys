#ifndef GAME
#define GAME

#include <SDL2/SDL.h>
#include "board.h"
#include "timer.h"

struct coordinate
{
	int x;
	int y;
};

struct opt_flags
{
	bool rows;
	bool cols;
	bool node_width;
	bool init_ups;

	bool node_fg;
	bool cursor_fg;
	bool grid;
	bool shadow;
	bool default_bg;
	bool paused_bg;

	bool file;
	bool file_board;
	bool grid_on;
	bool verbose;
};

struct game_options
{
	opt_flags flags;
	unsigned int rows;
	unsigned int cols;
	unsigned int node_width;
	unsigned int init_ups;
	board load_board;
	SDL_Color node_fg;
	SDL_Color cursor_fg;
	SDL_Color grid;
	SDL_Color shadow;
	SDL_Color default_bg;
	SDL_Color paused_bg;
};

#define NC      "\033[0m"
#define BLACK   "\033[0;30m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN    "\033[0;36m"
#define WHITE   "\033[0;37m"

/// Class which manages displaying things to the user and interpreting user input.
class game
{
private:
	SDL_Renderer* renderer;
	::board board;
	::timer timer;
	unsigned int current_ups;
	game_options opts;
	coordinate mouse_coords[2];

public:
	game();
	game( SDL_Renderer*, game_options );

	void run ();
	void handle_keybinds ( SDL_Event&, bool& );
	Uint32 update_mouse_pos ();
	void log ();
	void to_file ();
};

#endif
