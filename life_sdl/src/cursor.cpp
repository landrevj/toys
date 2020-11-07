#include "../include/cursor.h"

/**
 * The default constructor for the cursor object.
 */
cursor::cursor() : node() {}

/**
 * The non-default constructor for the cursor object.
 * @param x The x pixel position.
 * @param y The y pixel position.
 * @param w The width of the node in pixels.
 * @param a The life state of the node. (Bool)
 */
cursor::cursor( unsigned int x, unsigned int y, unsigned int w, bool a ) : node( x, y, w, a ) {  }

/**
 * Adds the cursor foreground to the renderer if the cursor is "alive". (In use.)
 * @param renderer Pointer to the SDL_Renderer.
 */
void cursor::render_fg ( SDL_Renderer* renderer, SDL_Color cursor_fg ) const
{
	if ( is_alive() )
	{
		SDL_Rect r = { (int) get_x_pos(), (int) get_y_pos(), (int) get_width(), (int) get_width() };
		SDL_SetRenderDrawColor( renderer, cursor_fg.r, cursor_fg.g, cursor_fg.b, cursor_fg.a );
		SDL_RenderDrawRect( renderer, &r );
	}
}

/**
 * Adds the cursor background to the renderer if the cursor is "alive". (In use.)
 * @param renderer Pointer to the SDL_Renderer.
 */
void cursor::render_bg ( SDL_Renderer* renderer, SDL_Color sh ) const
{
	if ( is_alive() )
	{
		SDL_Rect shadow = { (int) ( get_x_pos() + ( (float) get_width() / 3 ) ), (int) ( get_y_pos() + ( (float) get_width() / 3 ) ), (int) get_width(), (int) get_width() };
		SDL_SetRenderDrawColor( renderer, sh.r, sh.g, sh.b, sh.a );
		SDL_RenderDrawRect( renderer, &shadow );
	}
}
