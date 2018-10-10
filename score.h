#include "SDL.h"

class ScoreKeeper
{
private:
	//Either make separate variables for each 10^i or figure out some way of parsing
	//a variable with several places. It almost seems like a stream may be the best way to parse it.
	//width of
	int numberInterval;
	SDL_Rect numberRect;
	SDL_Surface *fontMap;
	//The number chosen to blit;
	int number;
	int score[10];
public:
	ScoreKeeper ();
	//~ScoreKeeper (); 
	void updateRect();
	void applyScore ();
	void incrementScore (int howMuch);
	bool roundUp (int score, int increase);
	void decrementScore (int howMuch);
	bool roundDown (int score, int decrease);
};