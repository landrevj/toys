#ifndef TIMER
#define TIMER

#include <SDL2/SDL.h>

class timer
{
private:
	Uint32 start_ticks;
	Uint32 paused_ticks;
	bool started;
	bool paused;

public:
	timer();

	void start ();
	void stop ();
	void pause ();
	void unpause ();

	Uint32 get_ticks () const;

	bool is_started () const;
	bool is_paused () const;
};

#endif
