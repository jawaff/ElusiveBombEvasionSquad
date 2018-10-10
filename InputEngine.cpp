#include "InputEngine.h"

using namespace std;

typedef pair<string, bool> keyPair;
GameEngine* GameEngine::ginstance = NULL;

GameEngine::GameEngine ()
{
	quit = false;
}

GameEngine* GameEngine::instance ()
{
	if (ginstance == NULL)
	{
		ginstance = new GameEngine ();
	}
	return ginstance;
}

GameEngine::~GameEngine ()
{
	delete ginstance;
	//ginstance = NULL;
	keys.clear ();
}

void GameEngine::handle_Input ()
{
	while (SDL_PollEvent (&event))
	{
		//I have gotten an unhandled exception from this part of my code.
		if (event.type == SDL_QUIT)
		{
			set_Quit ();
		}

		switch (event.type)
		{
		case SDL_KEYDOWN:
			iter = keys.find("KEYDOWN");
			if (iter != keys.end ()){iter->second = true;}
			else{keys.insert (keyPair("KEYDOWN", true));}
			switch (event.key.keysym.sym)
			{
			case SDLK_y:
				iter = keys.find("y");
				if (iter != keys.end ()){iter->second = true;}
				else{keys.insert (keyPair("y", true));}
				break;
			case SDLK_n:
				iter = keys.find("n");
				if (iter != keys.end ()){iter->second = true;}
				else{keys.insert (keyPair("n", true));}
				break;
			case SDLK_d:
				iter = keys.find("d");
				if (iter != keys.end ()){iter->second = true;}
				else{keys.insert (keyPair("d", true));}
				break;
			case SDLK_w:
				iter = keys.find("w");
				if (iter != keys.end ()){iter->second = true;}
				else{keys.insert (keyPair("w", true));}
				break;
			case SDLK_a:
				iter = keys.find("a");
				if (iter != keys.end ()){iter->second = true;}
				else{keys.insert (keyPair("a", true));}
				break;
			case SDLK_s:
				iter = keys.find("s");
				if (iter != keys.end ()){iter->second = true;}
				else{keys.insert (keyPair("s", true));}
				break;
			case SDLK_UP:
				iter = keys.find("up");
				if (iter != keys.end ()){iter->second = true;}
				else{keys.insert (keyPair("up", true));}
				break;
			case SDLK_DOWN:
				iter = keys.find("down");
				if (iter != keys.end ()){iter->second = true;}
				else{keys.insert (keyPair("down", true));}
				break;
			case SDLK_LEFT:
				iter = keys.find("left");
				if (iter != keys.end ()){iter->second = true;}
				else{keys.insert (keyPair("left", true));}
				break;
			case SDLK_RIGHT:
				iter = keys.find("right");
				if (iter != keys.end ()){iter->second = true;}
				else{keys.insert (keyPair("right", true));}
				break;
			case SDLK_RETURN:
				iter = keys.find("enter");
				if (iter != keys.end ()){iter->second = true;}
				else{keys.insert (keyPair("enter", true));}
				break;
			case SDLK_LSHIFT:
				iter = keys.find("Lshift");
				if (iter != keys.end ()){iter->second = true;}
				else{keys.insert (keyPair("Lshift", true));}
				break;
			case SDLK_ESCAPE:
				iter = keys.find("esc");
				if (iter != keys.end ()){iter->second = true;}
				else{keys.insert (keyPair("esc", true));}
				break;
				
			}
			break;
		}

		switch (event.type)
		{
		case SDL_KEYUP:
			iter = keys.find("KEYDOWN");
			if (iter != keys.end ()){iter->second = false;}
			else{keys.insert (keyPair("KEYDOWN", false));}
			switch (event.key.keysym.sym)
			{
			case SDLK_y:
				iter = keys.find("y");
				if (iter != keys.end ()){iter->second = false;}
				else {keys.insert (keyPair("y", false));}
				break;
			case SDLK_n:
				iter = keys.find("n");
				if (iter != keys.end ()){iter->second = false;}
				else {keys.insert (keyPair("n", false));}
				break;
			case SDLK_d:
				iter = keys.find("d");
				if (iter != keys.end ()){iter->second = false;}
				else {keys.insert (keyPair("d", false));}
				break;
			case SDLK_w:
				iter = keys.find("w");
				if (iter != keys.end ()){iter->second = false;}
				else {keys.insert (keyPair("w", false));}
				break;
			case SDLK_a:
				iter = keys.find("a");
				if (iter != keys.end ()){iter->second = false;}
				else {keys.insert (keyPair("a", false));}
				break;
			case SDLK_s:
				iter = keys.find("s");
				if (iter != keys.end ()){iter->second = false;}
				else {keys.insert (keyPair("s", false));}
				break;
			case SDLK_UP:
				iter = keys.find("up");
				if (iter != keys.end ()){iter->second = false;}
				else{keys.insert (keyPair("up", false));}
				break;
			case SDLK_DOWN:
				iter = keys.find("down");
				if (iter != keys.end ()){iter->second = false;}
				else{keys.insert (keyPair("down", false));}
				break;
			case SDLK_LEFT:
				iter = keys.find("left");
				if (iter != keys.end ()){iter->second = false;}
				else{keys.insert (keyPair("left", false));}
				break;
			case SDLK_RIGHT:
				iter = keys.find("right");
				if (iter != keys.end ()){iter->second = false;}
				else{keys.insert (keyPair("right", false));}
				break;
			case SDLK_RETURN:
				iter = keys.find("enter");
				if (iter != keys.end ()){iter->second = false;}
				else {keys.insert (keyPair("enter", false));}
				break;
			case SDLK_LSHIFT:
				iter = keys.find("Lshift");
				if (iter != keys.end ()){iter->second = false;}
				else{keys.insert (keyPair("Lshift", false));}
				break;
			case SDLK_ESCAPE:
				iter = keys.find("esc");
				if (iter != keys.end ()){iter->second = false;}
				else {keys.insert (keyPair("esc", false));}
				break;
			}
			break;
		}
	}
}

bool GameEngine::key_Down (std::string key)
{
	//keys.find = see if this key is being pressed, being key. And iter is the variable that says if that key is being pressed or not.
	iter = keys.find (key);
	//if a key hasn't been pressed.
	if (iter != keys.end())
	{
		//if there is a key pressed then iter returns the type. Which in this case is bool true or false;
		return iter->second;
	}
	else
	{
		//There is no input
		return false;
	}
}

bool GameEngine::key_Up (std::string key)
{
	//read everything in key_Down...
	iter = keys.find (key);
	if (iter != keys.end())
	{
		return iter->second;
	}
	else 
	{
		return false;
	}
}

void GameEngine::set_Quit ()
{
	quit = true;
}
//If the player hits the x on the window and wants to quit this will be used to see if that has been hit inside the main loop inside game.cpp
bool GameEngine::Quit ()
{
	return quit;
}