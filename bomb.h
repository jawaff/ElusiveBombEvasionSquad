#include "SDL.h"
#include <string>
#include <map>


class bomb
{
private:
	bool spawn;
	//Nooooo clue what this was for...
	//bool wave;
	int move;
	int numberEnemies;
	bool waveUp;
	//I have no clue what this was for... I never used it. So yeah...
	//SDL_Rect area [4]
	//The placement variable for the points
	int m [5];
	//Placement variable for the basic ai bombs
	int n[5];
	//More advanced ai bombs
	int v[5];
	int g;
	int f;
	int d;
	int rowDecider[5];
	//raytrace vars
	int traceSpd;
	int ipreviousMove[5];
	int icurrentMove[5];
	int ipreviousChosenMove[5];
	int icurrentChosenMove[5];
	bool startCurve[5];
	float xCurve[5];
	float yCurve[5];
	//checkMovement() check types
	enum CHECK {fourWayCheck, diagsCheck, curveRightCheck, curveUpCheck, curveLeftCheck, curveDownCheck, standby, right, up, left, down, curve1, curve2, curve3, curve4, curve5, curve6, curve7, curve8, diag1, diag2, diag3, diag4};
	
	int ipreviousCheck[5];
	int icurrentCheck[5];
	static bomb* binstance;
	std::map<int, bool> bombCollision;
	std::map<int, bool> pointCollision;
	std::map<int, bool> sBombCollision;
	//The first five of mBomb are to the actual bombs, 5-9 are for the tests.
	std::map<int, bool> mBombCollision;
	std::map<int, bool>::iterator iterion;
	std::map<int, int> rayDistance[5];
	std::map<int, int>::iterator iter;
public:
	static bomb* instance();
	bomb ();
	~bomb ();
	int h;
	//These variables tell me where each row's y coordinate is. Crucial for the rowBombPlacement() function.
	int rowy [5];
	int rowHeight;
	bool redrawRow [5];
	//Counts how many times the 5 rows have gone past the screen.
	int wave;
	//For every so many waves, this goes up.
	int waveCount;
	int speed; 
	int bombDamage;
	//This is how much the different objects will affect the score
	int bombCost;
	int pointCost;
	int splosionTime;
	bool bSplosion[20];
	bool pSplosion[5];
	//Functions for the bombs.
	void spawnBombs ();
	void bombPlacement ();
	void rowBombPlacement (int row);
	int enemyGen (int var);
	int random (int min, int max);
	int distanceFormula(SDL_Rect A, SDL_Rect B);
	int raytrace (int move, int bomb, int bombSpd);
	int collisionDistance (SDL_Rect bomb, SDL_Rect player);
	bool testCollision(int meatybomb);
	bool insertRayDistance (int move, int distance, int mbomb);
	int getRayDistance (int move, int mbomb);
	//function and method for the bombCollision map.
	bool isThereBombCollision (int bombNum);
	bool insertBomb (int bomb, bool collision);
	bool isTherePointCollision (int pointNum);
	bool insertPoint (int point, bool collision);
	bool isThereSBombCollision (int bombNum);
	bool insertSBomb (int bomb, bool collision);
	bool isThereMBombCollision (int bombNum);
	bool insertMBomb (int bomb, bool collision);
	void simpleIA (int row, SDL_Rect player);
	//For the meatierBomb, returns move.
	int checkMovement (int bomb, int check, int playerRow[2]);
	//returns move
	int implementMovement ();
	void meatierIA (int row, SDL_Rect player);
	void applyBombs (SDL_Surface* dest);
	void moveBombs (SDL_Rect player);

	//These are the variable for the bombs and points.
	//bombs may not be needed due to the fact that they will all be the same surface if I don't make multiple bombs. Which I may do since they
	//will have different movement types. But even so I would only need one surface per image.
	//SDL_Surface *bombs;
	SDL_Rect bdim [15];
	SDL_Rect point [5];
	SDL_Rect simpleBomb [5];
	SDL_Rect meatierBomb [5];
	SDL_Rect testBomb[5];
	SDL_Rect start[5];
	//These are for the meatyBomb's checkMovement();
	SDL_Rect player;
	int playerRow[2];
	int meatyRow[2];
};
