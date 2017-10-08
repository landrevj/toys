#ifndef CURSOR
#define CURSOR

#include "node.h"

class cursor : public node
{
public:
	cursor();
	cursor( unsigned int, unsigned int, unsigned int, bool );

	void render_fg( SDL_Renderer * renderer, SDL_Color ) const;
	void render_bg( SDL_Renderer * renderer, SDL_Color ) const;
};

#endif
