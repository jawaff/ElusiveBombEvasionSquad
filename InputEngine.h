#include <string>
#include <map>
#include "SDL.h"

class GameEngine
{
private:
	//Termination bool
	bool quit;
	SDL_Event event;
	static GameEngine* ginstance;
	std::map<std::string, bool> keys;
	std::map<std::string, bool>::iterator iter;

protected:
	GameEngine ();

public:
	static GameEngine* instance ();
	~GameEngine ();
	void handle_Input ();
	bool key_Down (std::string key);
	bool key_Up (std::string key);
	void set_Quit ();
	bool Quit ();
};