#ifndef NODE
#define NODE

#include <SDL2/SDL.h>

class node
{
private:
	unsigned int x_pos;
	unsigned int y_pos;
	unsigned int width;
	bool alive;

public:
	node();
	node( unsigned int, unsigned int, unsigned int, bool );

	unsigned int get_x_pos () const;
	unsigned int get_y_pos () const;
	void set_x_pos ( unsigned int );
	void set_y_pos ( unsigned int );

	unsigned int get_width () const;

	bool is_alive () const;
	void set_alive ( bool );
	bool toggle_life ();

	void render_fg( SDL_Renderer*, SDL_Color ) const;
	void render_bg( SDL_Renderer*, SDL_Color ) const;
};

#endif
