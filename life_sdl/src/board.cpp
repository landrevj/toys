#include <fstream>
#include <string>
#include "../include/board.h"

/**
 * The default constructor for the board class. Defaults to a 10x10 board of 10px wide nodes.
 */
board::board() : rows( 10 ), cols( 10 ), node_width( 10 ), generation( 0 ), paint( false ), draw_grid( false )
{
	c = cursor( 0, 0, node_width, false );
	// this CANNOT be the right way of doing this
	nodes.resize( rows );
	discrete_state.resize( rows );
	for ( int i = 0; i < rows; i++ )
	{
		nodes[i].resize( cols );
		discrete_state[i].resize( cols );
	}
	for ( int i = 0; i < rows; i++ )
	{
		for ( int j = 0; j < cols; j++ )
		{
			nodes[i][j] = node( j * node_width, i * node_width, node_width, false );
			discrete_state[i][j] = node( j * node_width, i * node_width, node_width, false );
		}
	}
}

/**
 * The non-default constructor for the board class.
 * @param r  The number of rows to make.
 * @param c  The number of columns to make.
 * @param nw The number of pixels wide to make each node.
 */
board::board( unsigned int r, unsigned int c, unsigned int nw, bool g ) : rows( r ), cols( c ), node_width( nw ), generation( 0 ), paint( false ), draw_grid( g )
{
	this->c = cursor( 0, 0, node_width, true );
	nodes.resize( rows );
	discrete_state.resize( rows );
	for ( int i = 0; i < rows; i++ )
	{
		nodes[i].resize( cols );
		discrete_state[i].resize( cols );
	}
	for ( int i = 0; i < rows; i++ )
	{
		for ( int j = 0; j < cols; j++ )
		{
			nodes[i][j] = node( j * node_width, i * node_width, node_width, false );
			discrete_state[i][j] = node( j * node_width, i * node_width, node_width, false );
		}
	}
}

/**
 * Get the number of colums.
 * @return The number of columns.
 */
unsigned int board::get_cols () const { return cols; }

/**
 * Get the number of rows.
 * @return The number of rows.
 */
unsigned int board::get_rows () const { return rows; }

/**
 * Get the pixel width of the board.
 * @return The pixel width of the board.
 */
unsigned int board::get_width () const { return cols * node_width; }

/**
 * Get the pixel height of the board.
 * @return The pixel height of the board.
 */
unsigned int board::get_height () const { return rows * node_width; }

/**
 * Get the pixel width used to create the nodes.
 * @return The pixel width use to create the nodes.
 */
unsigned int board::get_node_width () const { return node_width; }

/**
 * Get the pixel width used to create the nodes.
 * @return The pixel width use to create the nodes.
 */
unsigned int long board::get_generation () const { return generation; }

/**
 * Get a node object from the board.
 * @param  row The row index of the desired node. (The y index.)
 * @param  col The column index of the desired node. (The x index.)
 * @return     A reference to the desired node object.
 */
node & board::get_node ( unsigned int row, unsigned int col ) { return nodes[row][col]; }

/**
 * Get how many living nodes surround the specified node.
 *
 * The function looks at the 8 nodes surrounding the specified one and returns
 * how many of them are alive. Currently this function doesn't count nodes outside
 * of the board as dead, so a node up against a wall will only have 5 neighbors
 * and one in a corner will have only three.
 *
 * @param  row The row index of the specified node. (The y index.)
 * @param  col The column index of the specified node. (The x index.)
 * @return     The number of living neighbor nodes the specified node has.
 */
unsigned int board::get_living_neighbors ( unsigned int row, unsigned int col )
{
	bool along_top    = ( row == 0 ),
	     along_bottom = ( row == rows - 1 ),
	     along_left   = ( col == 0 ),
	     along_right  = ( col == cols - 1 );

	unsigned int count = 0;

	// check right
	if ( !along_right && nodes[row][col + 1].is_alive() ) count++;
	// check top right
	if ( !along_right && !along_top && nodes[row - 1][col + 1].is_alive() ) count++;
	// check top
	if ( !along_top && nodes[row - 1][col].is_alive() ) count++;
	// check top left
	if ( !along_left && !along_top && nodes[row - 1][col - 1].is_alive() ) count++;
	// check left
	if ( !along_left && nodes[row][col - 1].is_alive() ) count++;
	// check bottom left
	if ( !along_left && !along_bottom && nodes[row + 1][col - 1].is_alive() ) count++;
	// check bottom
	if ( !along_bottom && nodes[row + 1][col].is_alive() ) count++;
	// check bottom right
	if ( !along_right && !along_bottom && nodes[row + 1][col + 1].is_alive() ) count++;

	return count;
}

/**
 * Get the number of live nodes there are in the board.
 * (This not how I'd like to do this but I can't be bothered to refactor the class right now.)
 * @return The number of live nodes on the board.
 */
unsigned int board::count_live_nodes ()
{
	unsigned int count = 0;
	for ( int i = 0; i < rows; i++ )
	{
		for ( int j = 0; j < cols; j++ )
		{
			if ( nodes[i][j].is_alive() ) count++;
		}
	}
	return count;
}

/**
 * Gets the cursor object from the board.
 * @return A reference to the board's cursor object.
 */
cursor & board::get_cursor () { return c; }

/**
 * Moves the cursor based on the arrow key pressed.
 * Checks bounds to keep cursor in board.
 * If paint mode is enabled it draws on movement.
 *
 * @param e The SDL_Event which contains the key pressed.
 */
void board::move_cursor ( SDL_Event& e )
{
	c.set_alive( true );
	int x = c.get_x_pos();
	int y = c.get_y_pos();
	switch ( e.key.keysym.sym )
	{
		case SDLK_UP:
			if ( y != 0 ) { c.set_y_pos( y - node_width ); if ( paint ) use_cursor(); }
			break;
		case SDLK_DOWN:
			if ( y != get_height() - node_width ) { c.set_y_pos( y + node_width ); if ( paint ) use_cursor(); }
			break;
		case SDLK_LEFT:
			if ( x != 0 ) { c.set_x_pos( x - node_width ); if ( paint ) use_cursor(); }
			break;
		case SDLK_RIGHT:
			if ( x != get_width() - node_width ) { c.set_x_pos( x + node_width ); if ( paint ) use_cursor(); }
			break;
	}
}

/**
 * Toggles the life of the node under the cursor.
 */
void board::use_cursor ()
{
	int row, col;
	row = c.get_y_pos() / node_width;
	col = c.get_x_pos() / node_width;
	nodes[row][col].toggle_life();
}

/**
 * Toggles paint mode.
 * Toggles the life of the node under the cursor when being turned on.
 */
void board::toggle_paint () { if ( !paint ) use_cursor(); paint = !paint; }

/**
 * Toggles grid display.
 */
void board::toggle_grid () { draw_grid = !draw_grid; }

/**
 * Kills all nodes.
 */
void board::clear_board ()
{
	for ( int i = 0; i < rows; i++ )
		for ( int j = 0; j < cols; j++ )
			nodes[i][j].set_alive( false );
	discrete_state = nodes;
	generation = 0;
}

/**
 * Updates live nodes using the standard rules for Conway's Game of Life:
 *
 * Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
 * Any live cell with two or three live neighbours lives on to the next generation.
 * Any live cell with more than three live neighbours dies, as if by overpopulation.
 * Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
 * (From Wikipedia, https://en.wikipedia.org/wiki/Conway's_Game_of_Life#Rules)
 */
void board::update ()
{
	for ( int i = 0; i < rows; i++ )
	{
		for ( int j = 0; j < cols; j++ )
		{
			bool living = nodes[i][j].is_alive();
			int neighbors = get_living_neighbors( i, j );

			if ( living )
			{
				if ( neighbors < 2 || neighbors > 3 )
				{
					discrete_state[i][j].set_alive( false );
				}
				else
					discrete_state[i][j].set_alive( true );
			}
			else if ( neighbors == 3 )
			{
				discrete_state[i][j].set_alive( true );
			}
		}
	}

	generation++;

	// copy final state into nodes vector
	nodes = discrete_state;
}

/**
 * Adds nodes and the cursor to the renderer.
 * @param renderer A pointer to the SDL_Renderer.
 */
void board::render ( SDL_Renderer* renderer, SDL_Color node_fg, SDL_Color cursor_fg, SDL_Color grid, SDL_Color shadow ) const
{
	// outer boundaries and grid when enabled
	for ( int i = node_width; i <= get_width(); i += node_width )
	{
		if ( draw_grid || i == get_width() )
		{
			SDL_SetRenderDrawColor( renderer, grid.r, grid.g, grid.b, grid.a );
			SDL_RenderDrawLine( renderer, i, 0, i, get_height() );
		}
	}
	for ( int i = node_width; i <= get_height(); i += node_width )
	{
		if ( draw_grid || i == get_height() )
		{
			SDL_SetRenderDrawColor( renderer, grid.r, grid.g, grid.b, grid.a );
			SDL_RenderDrawLine( renderer, 0, i, get_width(), i );
		}
	}

	// draw alive nodes and shadows if grid is disabled
	if ( !draw_grid ) c.render_bg( renderer, shadow );
	for ( int i = 0; i < rows; i++ )
	{
		for ( int j = 0; j < cols; j++ )
		{
			if ( !draw_grid ) nodes[i][j].render_bg( renderer, shadow );
			nodes[i][j].render_fg( renderer, node_fg );
		}
	}
	// the foreground on top
	c.render_fg( renderer, cursor_fg );
}
