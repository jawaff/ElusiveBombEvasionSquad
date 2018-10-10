#include "timer.h"
#include "SDL.h"

timer::timer ()
{
	timers = 26;
	for (int i = 0; i < timers + 1; i++)
	{
		//set the status of the timer
		started[i] = false;
		paused[i] = false;
	}
}

void timer::start (int i)
{
	//set the status of the timer
	started[i] = true;
	paused[i] = false;

	//Get how many ticks has gone by since the program started.
	startTicks[i] = SDL_GetTicks ();
}

void timer::stop (int i)
{
	//set the status of the timer to stop by stating that the timer isn't started or paused.
	started[i] = false;
	paused[i] = false;
}

void timer::pause (int i)
{
	if (started[i] == true && paused[i] == false)
	{
		//set the status of the timer to paused.
		paused[i] = true;

		//calculate the paused ticks by subtracting the overall time by the time when the timer was initialized.
		pausedTicks[i] = SDL_GetTicks () - startTicks[i];
	}
}

void timer::unpause (int i)
{
	if (started[i] == true && paused[i] == true)
	{
		//Change the status of the timer
		paused[i] = false;

		//Reset the startTicks back to what it was before.
		startTicks[i] = SDL_GetTicks () - pausedTicks[i];

		//reset the paused ticks.
		pausedTicks[i] = 0;
	}
}

int timer::get_Ticks (int i)
{
	if (started[i] == true)
	{
		if (paused[i] == true)
		{
			//returns the ticks that has paused while the game is paused
			return pausedTicks[i];
		}
		else 
		{
			//returns the ticks since startTicks was set or resetted. 
			return SDL_GetTicks () - startTicks[i];
		}
	}
	//if the timer isn't being ran, return nothing
	return 0;
}

bool timer::is_Started (int i)
{
	//return the status of the timer
	return started[i];
}

bool timer::is_Paused (int i)
{
	//return the status of the timer
	return paused[i];
}

