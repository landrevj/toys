#include "../include/timer.h"

/**
 * The default constructor for the timer class.
 */
timer::timer() : start_ticks( 0 ), paused_ticks( 0 ), started(false), paused( false ) {}

/**
 * Either starts the timer or resets it if it has already been started.
 */
void timer::start ()
{
	start_ticks = SDL_GetTicks();
	paused_ticks = 0;
	started = true;
	paused = false;
}

/**
 * Stops the timer, i.e. resets everything back to zero.
 */
void timer::stop ()
{
	start_ticks = 0;
	paused_ticks = 0;
	started = false;
	paused = false;
}

/**
 * Pauses the timer.
 */
void timer::pause ()
{
	if ( started && !paused )
	{
		paused = true;
		paused_ticks = SDL_GetTicks() - start_ticks;
		start_ticks = 0;
	}
}

/**
 * Unpauses the timer.
 */
void timer::unpause ()
{
	if ( started && paused )
	{
		paused = false;
		start_ticks = SDL_GetTicks() - paused_ticks;
		paused_ticks = 0;
	}
}

/**
 * Gets the elapsed ticks.
 * @return The elapsed ticks.
 */
Uint32 timer::get_ticks () const
{
	Uint32 time = 0;
	if ( started )
	{
		if ( paused )
			time = paused_ticks;
		else
			time = SDL_GetTicks() - start_ticks;
	}
	return time;
}

/**
 * Tells whether the timer is started.
 * @return Whether or not the timer is started.
 */
bool timer::is_started () const { return started; }


/**
 * Tells whether the timer is paused.
 * @return Whether or not the timer is paused.
 */
bool timer::is_paused () const { return paused;}
