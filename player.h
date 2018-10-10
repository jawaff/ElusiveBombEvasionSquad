#include "SDL.h"
#include <string>
using namespace std;
class PlayerEngine 
{
private:
	SDL_Surface *playerSprite;
	SDL_Surface *toast;
	SDL_Surface *bacon;
	SDL_Surface *waffle;
	//The effect for the corresponding char
	SDL_Surface *effect;
	SDL_Surface *attack1;
	SDL_Surface *attack2;
	SDL_Surface *attack3;
	SDL_Surface *attack4;
	SDL_Rect effectOffset;
	string fileGreat;
	string fileGood;
	string fileOk;
	string fileMeh;
	string fileZzz;
	string fileDead;
	string fileEffect;
	string atk1;
	string atk2;
	string atk3;
	string atk4;
	string player;
	bool startCurve;
	//the lengths of the triagle within the radian circle thing. This will determine the players coordinates during the curve movement.
	int x;
	int y;
	//I'm using these as variables for speed during the curves.
	float xCurve;
	float yCurve;
	//Variable for degrees that is also a crucial part of the curve movement.
	double theta;
public:
	PlayerEngine (int x, int y);
	~PlayerEngine ();

	//This is for the selection screen
	void showChars();
	void freeChars(int i);
	void show (SDL_Surface* source, SDL_Surface* destination, SDL_Surface* effect);
	//returns playerSprite
	SDL_Surface* playerGet ();
	SDL_Surface* effectGet ();
	void setSpeed (int spd_x, int spd_y);
	int beginDirectionCalc ();
	int endDirectionCalc ();
	void move (int direction);
	bool chooseChar(int character);
	bool loadPlayer();
	void takeDamage (int damage);
	//player variables
	int xspeed;
	int yspeed;
	int boostSpd;
	int atkSpd;
	int score;
	bool death;
	bool attack;
	bool attackRight;
	bool attackUp;
	bool attackLeft;
	bool attackDown;
	//The health of the player
	int hitPoints;
	//Is toast man moving?
	bool moving;
	//Number of keys flag
	bool oneKey;
	bool standby;
	bool twoKeys;
	bool special;
	int prevMKeysPressed;
	int prevCKeysPressed;
	int mKeysPressed;
	int cKeysPressed;
	int ipreviousMove;
	int icurrentMove;
	//Variables holding current keyboard input for the movement method to handle
	bool w, a, s, d;
	bool up, down, left, right;
	//The dimension of which the player will contain.
	SDL_Rect pdim;
	//This rect will be in the center of the player and will be the area that will be checked for collisions with the
	//bombs and point thingys.
	SDL_Rect colRect;
};