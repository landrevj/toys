#ifndef BOARD
#define BOARD

#include <vector>
#include "node.h"
#include "cursor.h"

/// Manages game nodes and adds them to the renderer.
class board
{
private:
	std::vector<std::vector<node> > nodes;
	std::vector<std::vector<node> > discrete_state;
	unsigned int rows;
	unsigned int cols;
	unsigned int node_width;
	unsigned int long generation;
	cursor c;
	bool paint;
	bool draw_grid;

public:
	board();
	board( unsigned int, unsigned int, unsigned int, bool );

	unsigned int get_rows () const;
	unsigned int get_cols () const;
	unsigned int get_width () const;
	unsigned int get_height () const;
	unsigned int get_node_width () const;
	unsigned int long get_generation () const;

	node & get_node ( unsigned int, unsigned int );
	unsigned int get_living_neighbors ( unsigned int, unsigned int );
	unsigned int count_live_nodes ();

	cursor & get_cursor ();
	void move_cursor ( SDL_Event& );
	void use_cursor ();
	void toggle_paint ();
	void toggle_grid ();

	void clear_board ();

	void update ();
	void render( SDL_Renderer*, SDL_Color, SDL_Color, SDL_Color, SDL_Color ) const;
};

#endif
