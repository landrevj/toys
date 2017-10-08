#include "../include/node.h"

/**
 * The default constructor for the node object. Default to the node being at (0,0), 10px wide, and dead.
 */
node::node() : x_pos( 0 ), y_pos( 0 ), width( 10 ), alive( false ) {}

/**
 * The non-default constructor for the node object.
 * @param x The x pixel position.
 * @param y The y pixel position.
 * @param w The width of the node in pixels.
 * @param a The life state of the node. (Bool)
 */
node::node( unsigned int x, unsigned int y, unsigned int w, bool a ) : x_pos( x ), y_pos( y ), width( w ), alive( a ) {}

/**
 * Get the x pixel position of the node.
 * @return The x pixel position of the node.
 */
unsigned int node::get_x_pos () const { return x_pos; }

/**
 * Get the y pixel position of the node.
 * @return The y pixel position of the node.
 */
unsigned int node::get_y_pos () const { return y_pos; }

/**
 * Set the x pixel position of the node.
 * @param x The x pixel position to set to.
 */
void node::set_x_pos ( unsigned int x ) { x_pos = x;}

/**
 * Set the y pixel position of the node.
 * @param y The y pixel position to set to.
 */
void node::set_y_pos ( unsigned int y ) { y_pos = y;}

/**
 * Get the pixel width of the node.
 * @return The pixel width of the node.
 */
unsigned int node::get_width () const { return width; }

/**
 * Get whether or not the node is alive.
 * @return Bool life var.
 */
bool node::is_alive () const {return alive;}

/**
 * Set the life state of the node.
 * @param b Bool life var.
 */
void node::set_alive ( bool b ) {alive = b;}

/**
 * Toggles the node between being alive and dead.
 * @return Bool life var after toggle.
 */
bool node::toggle_life ()
{
	alive = !alive;
	return alive;
}

/**
 * Adds the node to the renderer if it is alive.
 * @param renderer Pointer to the SDL_Renderer.
 */
void node::render_fg ( SDL_Renderer* renderer, SDL_Color node_fg ) const
{
	if ( alive )
	{
		SDL_Rect r = { (int) x_pos, (int) y_pos, (int) width, (int) width };
		SDL_SetRenderDrawColor( renderer, node_fg.r, node_fg.g, node_fg.b, node_fg.a );
		SDL_RenderFillRect( renderer, &r );
	}
}

/**
 * Adds the node's shadow to the renderer if it is alive.
 * @param renderer Pointer to the SDL_Renderer.
 */
void node::render_bg ( SDL_Renderer* renderer, SDL_Color sh ) const
{
	if ( alive )
	{
		SDL_Rect shadow = { (int) ( x_pos + ( (float) width / 2.2 ) ), (int) ( y_pos + ( (float) width / 2.2 ) ), (int) width, (int) width };
		SDL_SetRenderDrawColor( renderer, sh.r, sh.g, sh.b, sh.a );
		SDL_RenderFillRect( renderer, &shadow );

	}
}
