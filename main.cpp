#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "main.h"
//#include "stdlib.h"
#include <iostream>

using namespace std;

bool quit;

int main (int argc, char* args[])
{
	//instance for the timer for regulating the frames per second.
	timer fps;

	int init;

	init = init_Game ();
	if (init == 0)
	{
		cout << "init_Game() failed to initialized\n";
	}
	//This line of code coupled with the first three lines on this page represent a system that
	//dumps information about memory leaks upon exiting the program.
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	quit = false;
	while (quit == false)
	{
		//The way I'm using this timer, it is not capping the frame rate at all.
		//Starts the timer that regulates the fps
		fps.start (25);
		//updates the state of the game.
		update_State();
		//Check if the frame rate is going faster than what I want it to.
		if (fps.get_Ticks(25) < 1000 / FRAMES_PER_SECOND)
		{
			//slow down the frame rate if it is going to fast
			SDL_Delay ((1000 / FRAMES_PER_SECOND) - fps.get_Ticks(25));
		}
	}
	return 0;
}

void update_State ()
{
	enum GAME_STATE {menu, setup, game, gquit};
	GAME_STATE Game_State = menu;
	
	int myState;
	switch (Game_State)
	{
	case menu:
		myState = showMenu ();
		if (myState == 0)
		{
			quit = true;
			exit(1);
		}
		if (myState == 1)
		{
			Game_State = setup;
		}

	case setup:

	case game:
		myState = 0;
		myState = playGame ();
		if (myState == 1)
		{
			Game_State = gquit;
		}
	case gquit:
		quit = true;
	}
}