#include "bomb.h"
#include "init.h"
#include <ctime>
#include <math.h>

typedef std::pair<int, bool> Col;
typedef std::pair<int, int> rayTuple;

bomb* bomb::binstance = NULL;
//Note to self: Donuts are the most delicious thing in the world.
bomb* bomb::instance()
{
	if (binstance == NULL)
	{
		binstance = new bomb ();
	}
	return binstance;
}
bomb::bomb ()
{
	//Due to the fact that I only have to define this once. I pulled it out of the random function
	srand (time(NULL));

	//Reset all of the variables for the bomb class
	spawn = false;
	//I have no clue what this was for... 
	//wave = false;
	wave = 0;
	waveCount = 0;
	move = 0;
	waveUp = false;
	//Change the speed default in spawnBombs() if changing speed.
	speed = 4;//Not sure if it's going to stay at 5 though...
	bombDamage = 5;
	bombCost = 6;
	pointCost = 8;
	splosionTime = 500;
	d = -1;
	f = -1;
	g = -1;
	//raytrace var
	traceSpd = 20;
	//bombs = IMG_load ("bomb.bmp");
	
	//Assigning the width and height of each bomb... If I make different bomb types I may want to alter this accordingly.
	for (int i = 0; i < 20; i++)
	{
		bdim[i].w = 50;
		bdim[i].h = 50;
		bSplosion[i] = false;
	}
	for (int i = 0; i < 5; i++)
	{
		point[i].w = 50;
		point[i].h = 50;
		simpleBomb[i].w = 50;
		simpleBomb[i].h = 50;
		meatierBomb[i].w = 50;
		meatierBomb[i].h = 50;
		//raytrace vars
		ipreviousMove[i] = -1;
		icurrentMove[i] = -1;
		startCurve[i] = false;
		ipreviousCheck[i] = -1;
		icurrentCheck[i] = -1;
		xCurve[i] = 0;
		yCurve[i] = 0;
		pSplosion[i] = false;
		redrawRow[i] = false;
	}

	rowHeight = SCREEN_HEIGHT / 4;
	//Setting the initial y value for the bottom of each row that separates the bombs and such.
	//this has been updated to fit screen_height 768
	rowy[0] = -rowHeight;
	rowy[1] = -rowHeight*2;
	rowy[2] = -rowHeight*3;
	rowy[3] = -rowHeight*4;
	rowy[4] = -rowHeight*5;	
}

bomb::~bomb ()
{
	delete binstance;
}

int bomb::random (int min, int max)
{
	//variable that holds the random number
	int blah;
	//Only define this once or the number won't be random.
	//I put this in the constructor of the bomb
	//srand (time(NULL));
	blah = rand() % (max - min) + min;

	return blah;
}
int bomb::distanceFormula (SDL_Rect A, SDL_Rect B)
{
	double distance = (B.x - A.x)^2 + (B.y - A.y)^2;
	if (distance < 0)
	{
		distance *= -1;
	}
	distance = sqrt(distance);
	return ceil(distance);
}
int bomb::raytrace (int move, int bomb, int bombSpd)
{
	int distance;
	move += 6;
	double distance;
	ipreviousMove[bomb] = icurrentMove[bomb];
	switch (move)
	{
	case standby:
		break;
	case right:
		if (ipreviousMove[bomb] != 1)
		{
			icurrentMove[bomb] = 1;
			testBomb[bomb].x = meatierBomb[bomb].x;
			start[bomb].x = testBomb[bomb].x;
		}
		testBomb[bomb].x += traceSpd;
		break;
	case up:
		if (ipreviousMove[bomb] != 2)
		{
			icurrentMove[bomb] = 2;
			testBomb[bomb].y = meatierBomb[bomb].y;
			start[bomb].y = testBomb[bomb].y;
		}
		testBomb[bomb].y -= traceSpd;
		break;
	case left:
		if (ipreviousMove[bomb] != 3)
		{
			icurrentMove[bomb] = 3;
			testBomb[bomb].x = meatierBomb[bomb].x;
			start[bomb].x = testBomb[bomb].x;
		}
		testBomb[bomb].x -= traceSpd;
		break;
	case down:
		if (ipreviousMove[bomb] != 4)
		{
			icurrentMove[bomb] = 4;
			testBomb[bomb].y = meatierBomb[bomb].y;
			start[bomb].y = testBomb[bomb].y;
		}
		testBomb[bomb].y += traceSpd;
		break;
	case curve1:
		if (ipreviousMove[bomb] != 5)
		{
			startCurve[bomb] = false;
			testBomb[bomb].x = meatierBomb[bomb].x;
			testBomb[bomb].y = meatierBomb[bomb].y;
			start[bomb].x = testBomb[bomb].x;
			start[bomb].y = testBomb[bomb].y;
		}
		if (startCurve[bomb] == false)
		{
			xCurve[bomb] = 0;
			yCurve[bomb] = bombSpd * 1.25 * 2;
			startCurve[bomb] = true;
		}
		if (xCurve[bomb] > bombSpd * 1.25 * 2 && yCurve[bomb] < 0)
		{
			testBomb[bomb].x += traceSpd;
		}
		else 
		{
			xCurve[bomb] += 0.8;//25
			yCurve[bomb] -= 0.8;//25
			testBomb[bomb].x += xCurve[bomb];
			testBomb[bomb].y -= yCurve[bomb];
		}
		break;
	case curve2:
		if (ipreviousMove[bomb] != 6)
		{
			startCurve[bomb] = false;
			testBomb[bomb].x = meatierBomb[bomb].x;
			testBomb[bomb].y = meatierBomb[bomb].y;
			start[bomb].x = testBomb[bomb].x;
			start[bomb].y = testBomb[bomb].y;
		}
		if (startCurve[bomb] == false)
		{
			xCurve[bomb] = bombSpd * 1.25 * 2;
			yCurve[bomb] = 0;
			startCurve[bomb] = true;
		}
		if (xCurve[bomb] < 0 && yCurve[bomb] > bombSpd * 1.25 * 2)
		{
			testBomb[bomb].y -= traceSpd;
		}
		else 
		{
			xCurve[bomb] -= 0.8;//xspeed / 25;
			yCurve[bomb] += 0.8;//yspeed / 25;
			testBomb[bomb].x += xCurve[bomb];// += X;
			testBomb[bomb].y -= yCurve[bomb];
		}
		break;
	case curve3:
		if (ipreviousMove[bomb] != 7)
		{
			startCurve[bomb] = false;
			testBomb[bomb].x = meatierBomb[bomb].x;
			testBomb[bomb].y = meatierBomb[bomb].y;
			start[bomb].x = testBomb[bomb].x;
			start[bomb].y = testBomb[bomb].y;
		}
		if (startCurve[bomb] == false)
		{
			xCurve[bomb] = bombSpd * 1.25 * 2;
			yCurve[bomb] = 0;
			startCurve[bomb] = true;
		}
		if (xCurve[bomb] < 0 && yCurve[bomb] > bombSpd * 1.25 * 2)
		{
			testBomb[bomb].y -= bombSpd;
		}
		else 
		{
			xCurve[bomb] -= 0.8;//xspeed / 25;
			yCurve[bomb] += 0.8;//yspeed / 25;
			testBomb[bomb].x -= xCurve[bomb];// += X;
			testBomb[bomb].y -= yCurve[bomb];
		}
		break;
	case curve4:
		if (ipreviousMove[bomb] != 8)
		{
			startCurve[bomb] = false;
			testBomb[bomb].x = meatierBomb[bomb].x;
			testBomb[bomb].y = meatierBomb[bomb].y;
			start[bomb].x = testBomb[bomb].x;
			start[bomb].y = testBomb[bomb].y;
		}
		if (startCurve[bomb] == false)
		{
			xCurve[bomb] = 0;
			yCurve[bomb] = bombSpd * 1.25 * 2;
			startCurve[bomb] = true;
		}
		if (xCurve[bomb] > bombSpd * 1.25 * 2 && yCurve < 0)
		{
			testBomb[bomb].x -= bombSpd;
		}
		else 
		{
			xCurve[bomb] += 0.8;//xspeed / 25;
			yCurve[bomb] -= 0.8;//yspeed / 25;
			testBomb[bomb].x -= xCurve[bomb];// += X;
			testBomb[bomb].y -= yCurve[bomb];
		}
		break;
	case curve5:
		if (ipreviousMove[bomb] != 9)
		{
			startCurve[bomb] = false;
			testBomb[bomb].x = meatierBomb[bomb].x;
			testBomb[bomb].y = meatierBomb[bomb].y;
			start[bomb].x = testBomb[bomb].x;
			start[bomb].y = testBomb[bomb].y;
		}
		if (startCurve[bomb] == false)
		{
			xCurve[bomb] = 0;
			yCurve[bomb] = bombSpd * 1.25 * 2;
			startCurve[bomb] = true;
		}
		if (xCurve[bomb] > bombSpd * 1.25 * 2 && yCurve[bomb] < 0)
		{
			testBomb[bomb].x -= bombSpd;
		}
		else 
		{
			xCurve[bomb] += 0.8;//xspeed / 25;
			yCurve[bomb] -= 0.8;//yspeed / 25;
			testBomb[bomb].x -= xCurve[bomb];// += X;
			testBomb[bomb].y += yCurve[bomb];
		}
		break;
	case curve6:
		if (ipreviousMove[bomb] != 10)
		{
			startCurve[bomb] = false;
			testBomb[bomb].x = meatierBomb[bomb].x;
			testBomb[bomb].y = meatierBomb[bomb].y;
			start[bomb].x = testBomb[bomb].x;
			start[bomb].y = testBomb[bomb].y;
		}
		if (startCurve[bomb] == false)
		{
			xCurve[bomb] = bombSpd * 1.25 * 2;
			yCurve[bomb] = 0;
			startCurve[bomb] = true;
		}
		if (xCurve[bomb] < 0 && yCurve[bomb] > bombSpd * 1.25 * 2)
		{
			testBomb[bomb].y += bombSpd;
		}
		else 
		{
			xCurve[bomb] -= 0.8;//xspeed / 25;
			yCurve[bomb] += 0.8;//yspeed / 25;
			testBomb[bomb].x -= xCurve[bomb];// += X;
			testBomb[bomb].y += yCurve[bomb];
		}
		break;
	case curve7:
		if (ipreviousMove[bomb] != 11)
		{
			startCurve[bomb] = false;
			testBomb[bomb].x = meatierBomb[bomb].x;
			testBomb[bomb].y = meatierBomb[bomb].y;
			start[bomb].x = testBomb[bomb].x;
			start[bomb].y = testBomb[bomb].y;
		}
		if (startCurve[bomb] == false)
		{
			xCurve[bomb] = bombSpd * 1.25 * 2;
			yCurve[bomb] = 0;
			startCurve[bomb] = true;
		}
		if (xCurve[bomb] < 0 && yCurve[bomb] > bombSpd * 1.25 * 2)
		{
			testBomb[bomb].y += bombSpd;
		}
		else 
		{
			xCurve[bomb] -= 0.8;//xspeed / 25;
			yCurve[bomb] += 0.8;//yspeed / 25;
			testBomb[bomb].x += xCurve[bomb];// += X;
			testBomb[bomb].y += yCurve[bomb];
		}
		break;
	case curve8:
		if (ipreviousMove[bomb] != 12)
		{
			startCurve[bomb] = false;
			testBomb[bomb].x = meatierBomb[bomb].x;
			testBomb[bomb].y = meatierBomb[bomb].y;
			start[bomb].x = testBomb[bomb].x;
			start[bomb].y = testBomb[bomb].y;
		}
		if (startCurve[bomb] == false)
		{
			xCurve[bomb] = 0;
			yCurve[bomb] = bombSpd * 1.25 * 2;
			startCurve[bomb] = true;
		}
		if (xCurve[bomb] > bombSpd * 1.25 && yCurve[bomb] < 0)
		{
			testBomb[bomb].x += bombSpd;
		}
		else 
		{
			xCurve[bomb] += 0.8;//xspeed / 25;
			yCurve[bomb] -= 0.8;//yspeed / 25;
			testBomb[bomb].x += xCurve[bomb];// += X;
			testBomb[bomb].y += yCurve[bomb];
		}
		break;
	case diag1:
		if (ipreviousMove[bomb] != 13)
		{
			icurrentMove[bomb] = 13;
			testBomb[bomb].x = meatierBomb[bomb].x;
			testBomb[bomb].y = meatierBomb[bomb].y;
			start[bomb].x = testBomb[bomb].x;
			start[bomb].y = testBomb[bomb].y;
		}
		testBomb[bomb].x += bombSpd/1.5 * 2;
		testBomb[bomb].y -= bombSpd/1.5 * 2;
		break;
	case diag2:
		if (ipreviousMove[bomb] != 14)
		{
			icurrentMove[bomb] = 14;
			testBomb[bomb].x = meatierBomb[bomb].x;
			testBomb[bomb].y = meatierBomb[bomb].y;
			start[bomb].x = testBomb[bomb].x;
			start[bomb].y = testBomb[bomb].y;
		}
		testBomb[bomb].x -= bombSpd/1.5 * 2;
		testBomb[bomb].y -= bombSpd/1.5 * 2;
		break;
	case diag3:
		if (ipreviousMove[bomb] != 15)
		{
			icurrentMove[bomb] = 15;
			testBomb[bomb].x = meatierBomb[bomb].x;
			testBomb[bomb].y = meatierBomb[bomb].y;
			start[bomb].x = testBomb[bomb].x;
			start[bomb].y = testBomb[bomb].y;
		}
		testBomb[bomb].x -= bombSpd/1.5 * 2;
		testBomb[bomb].y += bombSpd/1.5 * 2;
		break;
	case diag4:
		if (ipreviousMove[bomb] != 16)
		{
			icurrentMove[bomb] = 16;
			testBomb[bomb].x = meatierBomb[bomb].x;
			testBomb[bomb].y = meatierBomb[bomb].y;
			start[bomb].x = testBomb[bomb].x;
			start[bomb].y = testBomb[bomb].y;
		}
		testBomb[bomb].x += bombSpd/1.5 * 2;
		testBomb[bomb].y += bombSpd/1.5 * 2;
		break;
	}
	//This is the distance the raytrace has gone.
	distance = distanceFormula(start[bomb], testBomb[bomb]);
	return distance;
}
int bomb::collisionDistance (SDL_Rect A, SDL_Rect B)
{
	int distance;
	int Ax, Ay, Bx, By;
	Ax = A.x + (A.w / 2);
	Ay = Ay + (A.h / 2);
	Bx = B.x + (B.w / 2);
	By = By + (B.h / 2);
	distance = (Ax - Bx)^2 + (Ay - By)^2;
	return distance;
}
bool bomb::testCollision(int meatybomb)
{
	int distance;
	int maxBomb;
	if (isThereMBombCollision(meatybomb + 5) != true)
	{
		if (meatyRow[0] == meatyRow[1])
		{
			maxBomb = (meatyRow[0] + 1) * 3;
			for (int i = maxBomb - 3; i < maxBomb; i++)
			{
				distance = collisionDistance(meatierBomb[meatybomb], bdim[i]);
				if (distance < 2916)
				{
					if (insertMBomb(meatybomb + 5, true) == true)
					{	
						return true;
					}
				}
			}
			//If collision with any object, return true
			//else return false;
		}
	}
}
bool bomb::insertRayDistance (int move, int distance, int mbomb)
{
	iter = rayDistance[mbomb].find(move);
	if (iter != rayDistance[mbomb].end())
	{
		if (iter->second != distance)
		{
			iter->second = distance;
			return true;
		}
		else 
		{
			return false;
		}
	}
	else
	{
		rayDistance[mbomb].insert(rayTuple(move, distance));
		return true;
	}
}
int bomb::getRayDistance (int move, int mbomb)
{
	iter = rayDistance[mbomb].find(move);
	if (iter != rayDistance[mbomb].end())
	{
		return iter->second;
	}
	else
	{
		return -1;
	}
}
void bomb::spawnBombs ()
{	
	//To increase the speed depending on the wave. % 5 is so it resets to the original speed after 5 waves then goes
	//back up again.
	if (waveCount >= 4)
	{
		waveCount = 0;
	}
	speed = waveCount + 4;
}
//This never got used, but maybe will in the future?
int bomb::enemyGen (int var)
{
	////easiest wave
	//if (var == 1){/*enemies = ?*/;}
	//else if (var == 2){/*enemies = ?*/;}
	//else if (var == 3){/*enemies = ?*/;}
	//else if (var == 4){/*enemies = ?*/;}
	//
	int enemies = 12;
	int rola = 0;
	rola = var * 2;
	enemies += rola;
	return enemies;
}
void bomb::bombPlacement ()
{
	//Everything dealing with x was meant for the rowPlacement function. But since these are the starting m values. It needs to start off with something.
	//So the if statements are completely a bad decision.
	//Some things dealing with x are ok... But not the things commented out below...
	////Variable for the row before the one that is needed.
	//int x;

	//Since this function is for the setup of the bombs and such. The row won't be anything other than 0, so I just declared this to make it easier.
	int row;
	row = 0;

	//m[0] = random (0, 4);

	////This is because there is no row before 0...
	//x = m[row + 4];

	//if ((x - 1) >= 0 && (x + 1) << 4)
	//{
	//	m[row] = random (x - 1, x + 1);
	//}

	//else if ((x + 1) >> 4)
	//{
	//	m[row] = random ( x-1, x);
	//}

	//else if ((x - 1) << 0)
	//{
	//	m[row] = random (x, x + 1);
	//}
	bool start = true;
	if (start == true)
	{
		//Assigning all of the point places in relation to the bombs and such.
		for (int i = 0; i < 5; i++)
		{
			//Decides what type of bomb will be placed in this row.
			//Only two special types atm.
			rowDecider[i] = random (0,2);
			//Points
			m[i] = random (0,5);
			n[i] = random (0,5);
			//simpleBomb
			while (n[i] == m[i])
			{
				n[i] = random (0,5);
			}
			v[i] = random (0,5);
			//AI
			while (v[i] == m[i] /*|| v[i] == n[i]*/)
			{
				v[i] = random (0,5);
			}
		}
		//bdim[19].x = NULL;
		int loopCounter;
		loopCounter = 0;
		bool xset = false;
		//Allows me to use one for loop for multiple rows	
		//int c = 0;
		int y = 0;
		//This allows me to be able to set y coord. within the for loop
		int t = 0;
		//For checking if odd or even row
		int r;
		//The wave based on the row
		int w = 3; 
		r = 1;
		//This keeps track of the bomb we are dealing with according to bdim[]
		int b = 0;
		while (xset == false)
		{
			//These are the place variables for each bomb type.
			d = (m[row] + t);
			if (rowDecider[row] == 0)
			{
				g = -1;
				f = n[row] + t;
			}
			else if (rowDecider[row] == 1)
			{
				f = -1;
				g = v[row] + t;
			}
			else
			{
				f = -1;
				g = -1;
			}
			for (int i = t; i < t + 5; i++)
			{
				//Starting enemy in the row. Needed because the row will switch directions here
				if (i == 0 || i == 10 || i == 20)
				{
					//If the starting thing happens to be a point thing
					if (d == i)
					{
						point[row].x = random (868, 1084 - point[row].w); 
						point[row].y = random (y - 192, y - point[row].h);
						continue;
					}
					//simpleBomb
					else if (f == i)
					{
						simpleBomb[row].x = random (868, 1084 - simpleBomb[row].w);
						simpleBomb[row].y = random (y - 192, y - simpleBomb[row].h);
						continue;
					}
					//meatierBomb
					else if (g == i)
					{
						meatierBomb[row].x = random (868, 1084 - meatierBomb[row].w);
						meatierBomb[row].y = random (y - 192, y - meatierBomb[row].h);
						continue;
					}
					else
					{
						bdim[b].x = random (868 , 1084 - bdim[b].w);
						bdim[b].y = random (y - 192, y - bdim[b].h);
						b += 1;
						continue;
					}
				}
				//Every other bomb in odd row
				else if (i > 0 && i < 5 || i > 10 && i < 15 || i > 20 && i < 25)
				{
					// if other things are the point things
					if (d == i)
					{
						if (f == d - 1)
						{
							point[row].x = random (simpleBomb[row].x - 216, simpleBomb[row].x- point[row].w); 
							point[row].y = random (y - 192, y - point[row].h);
							continue;
						}
						else if (g == d - 1)
						{
							point[row].x = random (meatierBomb[row].x - 216, meatierBomb[row].x - point[row].w); 
							point[row].y = random (y - 192, y - point[row].h);
							continue;
						}
						else
						{
							point[row].x = random (bdim[b-1].x - 216, bdim[b-1].x - point[row].w); 
							point[row].y = random (y - 192, y - point[row].h);
							continue;
						}
					}
					else if (f == i)
					{
						if (d == f - 1)
						{
							simpleBomb[row].x = random (point[row].x - 216, point[row].x - simpleBomb[row].w); 
							simpleBomb[row].y = random (y - 192, y - simpleBomb[row].h);
							continue;
						}
						else if (g == f - 1)
						{
							simpleBomb[row].x = random (meatierBomb[row].x - 216, meatierBomb[row].x - simpleBomb[row].w); 
							simpleBomb[row].y = random (y - 192, y - simpleBomb[row].h);
							continue;
						}
						else
						{
							simpleBomb[row].x = random (bdim[b-1].x - 216, bdim[b-1].x - simpleBomb[row].w); 
							simpleBomb[row].y = random (y - 192, y - simpleBomb[row].h);
							continue;
						}
					}
					else if (g == i)
					{
						if (d == g - 1)
						{
							meatierBomb[row].x = random (point[row].x - 216, point[row].x - meatierBomb[row].w); 
							meatierBomb[row].y = random (y - 192, y - meatierBomb[row].h);
							continue;
						}
						else if (f == g - 1)
						{
							meatierBomb[row].x = random (simpleBomb[row].x - 216, simpleBomb[row].x- meatierBomb[row].w); 
							meatierBomb[row].y = random (y - 192, y - meatierBomb[row].h);
							continue;
						}
						else
						{
							meatierBomb[row].x = random (bdim[b-1].x - 216, bdim[b-1].x - meatierBomb[row].w); 
							meatierBomb[row].y = random (y - 192, y - meatierBomb[row].h);
							continue;
						}
					}
					else
					{
						if (d == i - 1)
						{
							bdim[b].x = random (point[row].x - 216, point[row].x - point[row].w);
							bdim[b].y = random (y - 192, y - bdim[b].h);
							b += 1;
							continue;
						}
						else if (f == i - 1)
						{
							bdim[b].x = random (simpleBomb[row].x - 216, simpleBomb[row].x - simpleBomb[row].w);
							bdim[b].y = random (y - 192, y - bdim[b].h);
							b += 1;
							continue;
						}
						else if (g == i - 1)
						{
							bdim[b].x = random (meatierBomb[row].x - 216, meatierBomb[row].x - meatierBomb[row].w);
							bdim[b].y = random (y - 192, y - bdim[b].h);
							b += 1;
							continue;
						}
						else
						{
							bdim[b].x = random (bdim[b-1].x - 216, bdim[b-1].x - bdim[b].w);
							bdim[b].y = random (y - 192, y - bdim[b].h);
							b += 1;
							continue;
						}
					}
				}
				//}
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				////Regular rows
				//else if (r == 0)
				//{
				//starting point in reg row
				else if (i == 5 || i == 15)
				{
					//if thing is a point
					if (d == i)
					{
						//I had a break that check if the computer was reading this yet. Just saying that already passed don't do it again.
						point[row].x = random (0, 216); 
						point[row].y = random (y - 192, y - point[row].h);					
						continue;
					}
					else if (f == 1)
					{
						simpleBomb[row].x = random (0, 216);
						simpleBomb[row].y = random (y - 192, y - simpleBomb[row].h);
						continue;
					}
					else if (g == i)
					{
						meatierBomb[row].x = random (0, 216);
						meatierBomb[row].y = random (y - 192, y - meatierBomb[row].h);
						continue;
					}
					else
					{
						bdim[b].x = random (0, 216);
						bdim[b].y = random (y - 192, y - bdim[b].h);
						b += 1;
						continue;
					}
				}
				//Other objects in reg row
				else if (i > 5 && i < 10 || i > 15 && i < 20)
				{
					//if thing is a point
					if (d == i)
					{
						if (f == d - 1)
						{
							point[row].x = random ((simpleBomb[row].x + simpleBomb[row].w) + 50, simpleBomb[row].x + 216); 
							point[row].y = random (y - 192, y - point[row].h);	
							continue;
						}
						else if (g == d - 1)
						{
							point[row].x = random ((meatierBomb[row].x + meatierBomb[row].w) + 50, meatierBomb[row].x + 216); 
							point[row].y = random (y - 192, y - point[row].h);	
							continue;
						}
						else
						{
							point[row].x = random ((bdim[b-1].x + bdim[b-1].w) + 50, bdim[b-1].x + 216); 
							point[row].y = random (y - 192, y - point[row].h);	
							continue;
						}
					}
					else if (f == i)
					{
						if (d == f - 1)
						{
							simpleBomb[row].x = random ((point[row].x + point[row].w) + 50, point[row].x + 216); 
							simpleBomb[row].y = random (y - 192, y - simpleBomb[row].h);
							continue;
						}
						else if (g == f - 1)
						{
							simpleBomb[row].x = random ((meatierBomb[row].x + meatierBomb[row].w) + 50, meatierBomb[row].x + 216); 
							simpleBomb[row].y = random (y - 192, y - simpleBomb[row].h);
							continue;
						}
						else
						{
							simpleBomb[row].x = random ((bdim[b-1].x + bdim[b-1].w) + 50, bdim[b-1].x + 216); 
							simpleBomb[row].y = random (y - 192, y - simpleBomb[row].h);	
							continue;
						}
					}
					else if (g == i)
					{
						if (f == g - 1)
						{
							meatierBomb[row].x = random ((simpleBomb[row].x + simpleBomb[row].w) + 50, simpleBomb[row].x + 216); 
							meatierBomb[row].y = random (y - 192, y - meatierBomb[row].h);	
							continue;
						}
						else if (d == g - 1)
						{
							meatierBomb[row].x = random ((point[row].x + point[row].w) + 50, point[row].x + 216); 
							meatierBomb[row].y = random (y - 192, y - meatierBomb[row].h);	
							continue;
						}
						else
						{
							meatierBomb[row].x = random ((bdim[b-1].x + bdim[b-1].w) + 50, bdim[b-1].x + 216); 
							meatierBomb[row].y = random (y - 192, y - meatierBomb[row].h);	
							continue;
						}
					}
					else
					{
						if (f == i - 1)
						{
							bdim[b].x = random ((simpleBomb[row].x + simpleBomb[row].w) + 50, simpleBomb[row].x + 216);
							bdim[b].y = random (y - 192, y - bdim[b].h);
							b += 1;
							continue;
						}
						else if (d == i - 1)
						{
							bdim[b].x = random ((point[row].x + point[row].w) + 50, point[row].x + 216);
							bdim[b].y = random (y - 192, y - bdim[b].h);
							b += 1;
							continue;
						}
						else if (g == i - 1)
						{
							// The problem here I think is because of the [i-1] within the array.
							bdim[b].x = random ((meatierBomb[row].x + meatierBomb[row].w) + 50, meatierBomb[row].x + 216);
							bdim[b].y = random (y - 192, y - bdim[b].h);
							b += 1;
							continue;
						}
						else
						{
							// The problem here I think is because of the [i-1] within the array.
							bdim[b].x = random ((bdim[b-1].x + bdim[b-1].w) + 50, bdim[b-1].x + 216);
							bdim[b].y = random (y - 192, y - bdim[b].h);
							b += 1;
							continue;
						}	
					}
				}
				//}
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}
		
			if (loopCounter >= 4)
			{
				xset = true;
			}
			else 
			{
				//Switches set of surfaces
				t += 5;
				//Switches between rows
				y -= 192;
				//Switches row
				row += 1;
				w = row + 3;
				r = w % 2;
				//rect can't be converted from a rect to int. So I did this to go around that.
				//And this is for the point's coordinates. Ex. if d == i within for loop, then assign point coordinates
				d = (m[row] + t);
				//This counts how many times the while loop has cycled through.
				//I may be able to just use a for loop instead of a while loop.
				loopCounter += 1;
			}
		}
	}
}
void bomb::rowBombPlacement (int row)
{
 	for (int i = ((row + 1) * 3) - 3; i < (row + 1)*3; i++)
	{
		iterion = bombCollision.find(i);
		if (iterion != bombCollision.end())
		{
			iterion->second = false;
		}
	}
	iterion = pointCollision.find(row);
	if (iterion != pointCollision.end())
	{
		iterion->second = false;
	}
	iterion = sBombCollision.find(row);
	if (iterion != sBombCollision.end())
	{
		iterion->second = false;
	}
	iterion = mBombCollision.find(row);
	if (iterion != mBombCollision.end())
	{
		iterion->second = false;
	}
	//Makes the rows into number 1-4 so I can multiply them by 4. So I can then set the parameters for the for loop.
	int w;
	//sets the i = ? and i < ? in the for loop depending on the row.
	int t;
	int b;
	//All for the for loop parameters.
	w = row + 1;
	t = (w * 5) - 5;
	b = (w * 3) - 3;
	////This variable is for the 
	//int x;
	////Sets the variable that points to the row before the one that is being set up.
	//if (row == 0)
	//{
	//	x = m[row + 4];
	//}
	//else
	//{
	//	x = m[row - 1];
	//}
	//Decides what type of bomb will be placed in this row.
	//Only two special types atm.
	rowDecider[row] = random (0,2);
	//Points
	m[row] = random (0,5);
	n[row] = random(0,5);
	//simpleBomb
	while (n[row] == m[row])	
	{
		n[row] = random (0,5);
	}
	v[row] = random (0,5);
	//AI
	while (v[row] == m[row]/* || v[row] == n[row]*/)
	{
		v[row] = random (0,5);
	}
	
	////Makes the point position depending on the one preceding it.
	//if ((x - 1) >= 0 && (x + 1) <= 4)
	//{
	//	m[row] = random (x - 1, x + 2);
	//}
	//else if ((x + 1) >> 4)
	//{
	//	m[row] = random ( x - 1, x + 1);
	//}
	//else if ((x - 1) << 0)
	//{
	//	m[row] = random (x, x + 2);
	//}
	//d is the point's value within the entire thing of bombs. It is for the if (d == i) ....
	d = t + m[row];
	if (rowDecider[row] == 0)
	{
		g = -1;
		f = n[row] + t;
	}
	else if (rowDecider[row] == 1)
	{
		f = -1;
		g = v[row] + t;
	}
	else
	{
		f = -1;
		g = -1;
	}
	//set the coordinates for the row.
	for (int i = t; i < t + 5; i++)
	{
		//Starting enemy in the row. Needed because the row will switch directions here
		if (i == 0 || i == 10 || i == 20)
		{
			//If the starting thing happens to be a point thing
			if (d == i)
			{
				point[row].x = random (868, 1084 - point[row].w); 
				point[row].y = random (- 192, - point[row].h);
				continue;
			}
			else if (f == i)
			{
				simpleBomb[row].x = random (868, 1084 - simpleBomb[row].w);
				simpleBomb[row].y = random (- 192, - simpleBomb[row].h);
				continue;
			}
			else if (g == i)
			{
				meatierBomb[row].x = random (868, 1084 - meatierBomb[row].w);
				meatierBomb[row].y = random (- 192, - meatierBomb[row].h);
				continue;
			}
			else
			{
				bdim[b].x = random (868 , 1084 - bdim[b].w);
				bdim[b].y = random (- 192, - bdim[b].h);
				b += 1;
				continue;
			}
		}
		//Every other bomb in odd row
		else if (i > 0 && i < 5 || i > 10 && i < 15 || i > 20 && i < 25)
		{
			// if other things are the point things
			if (d == i)
			{
				if (f == d - 1)
				{
					point[row].x = random (simpleBomb[row].x - 216, simpleBomb[row].x- point[row].w); 
					point[row].y = random (-192, -point[row].h);
					continue;
				}
				else if (g == d - 1)
				{
					point[row].x = random (meatierBomb[row].x - 216, meatierBomb[row].x - point[row].w); 
					point[row].y = random (-192, -point[row].h);
					continue;
				}
				else
				{
					point[row].x = random (bdim[b-1].x - 216, bdim[b-1].x - point[row].w); 
					point[row].y = random (-192, -point[row].h);
					continue;
				}
			}
			else if (f == i)
			{
				if (d == f - 1)
				{
					simpleBomb[row].x = random (point[row].x - 216, point[row].x - simpleBomb[row].w); 
					simpleBomb[row].y = random (-192, -simpleBomb[row].h);
					continue;
				}
				else if (g == f - 1)
				{
					simpleBomb[row].x = random (meatierBomb[row].x - 216, meatierBomb[row].x - simpleBomb[row].w); 
					simpleBomb[row].y = random (-192, -simpleBomb[row].h);
					continue;
				}
				else
				{
					simpleBomb[row].x = random (bdim[b-1].x - 216, bdim[b-1].x - simpleBomb[row].w); 
					simpleBomb[row].y = random (- 92, -simpleBomb[row].h);
					continue;
				}
			}
			else if (g == i)
			{
				if (d == g - 1)
				{
					meatierBomb[row].x = random (point[row].x - 216, point[row].x - meatierBomb[row].w); 
					meatierBomb[row].y = random (-192, -meatierBomb[row].h);
					continue;
				}
				else if (f == g - 1)
				{
					meatierBomb[row].x = random (simpleBomb[row].x - 216, simpleBomb[row].x- meatierBomb[row].w); 
					meatierBomb[row].y = random (-192, -meatierBomb[row].h);
					continue;
				}
				else
				{
					meatierBomb[row].x = random (bdim[b-1].x - 216, bdim[b-1].x - meatierBomb[row].w); 
					meatierBomb[row].y = random (-192, -meatierBomb[row].h);
					continue;
				}
			}
			else
			{
				if (d == i - 1)
				{
					bdim[b].x = random (point[row].x - 216, point[row].x - point[row].w);
					bdim[b].y = random (-192, -bdim[b].h);
					b += 1;
					continue;
				}
				else if (f == i - 1)
				{
					bdim[b].x = random (simpleBomb[row].x - 216, simpleBomb[row].x - simpleBomb[row].w);
					bdim[b].y = random (- 192, - bdim[b].h);
					b += 1;
					continue;
				}
				else if (g == i - 1)
				{
					bdim[b].x = random (meatierBomb[row].x - 216, meatierBomb[row].x - meatierBomb[row].w);
					bdim[b].y = random (- 192, - bdim[b].h);
					b += 1;
					continue;
				}
				else
				{
					bdim[b].x = random (bdim[b-1].x - 216, bdim[b-1].x - bdim[b-1].w);
					bdim[b].y = random (- 192, - bdim[b].h);
					b += 1;
					continue;
				}
			}
		}
		//starting point in reg row
		else if (i == 5 || i == 15)
		{
			//if thing is a point
			if (d == i)
			{
				point[row].x = random (0, 216); 
				point[row].y = random (- 192, - point[row].h);					
				continue;
			}
			else if (f == i)
			{
				simpleBomb[row].x = random (0, 216);
				simpleBomb[row].y = random (- 192, - simpleBomb[row].h);
				continue;
			}
			else if (g == i)
			{
				meatierBomb[row].x = random (0, 216);
				meatierBomb[row].y = random (- 192, - meatierBomb[row].h);
				continue;
			}
			else
			{
				bdim[b].x = random (0, 216);
				bdim[b].y = random (- 192, - bdim[b].h);
				b += 1;
				continue;
			}
		}
		//Other objects in reg row
		else if (i > 5 && i < 10 || i > 15 && i < 20)
		{
			//if thing is a point
			if (d == i)
			{
				if (f == d - 1)
				{
					point[row].x = random ((simpleBomb[row].x + simpleBomb[row].w) + 50, simpleBomb[row].x + 216); 
					point[row].y = random (- 192, - point[row].h);	
					continue;
				}
				else if (g == d - 1)
				{
					point[row].x = random ((meatierBomb[row].x + meatierBomb[row].w) + 50, meatierBomb[row].x + 216); 
					point[row].y = random (- 192, - point[row].h);	
					continue;
				}
				else
				{
					point[row].x = random ((bdim[b-1].x + bdim[b-1].w) + 50, bdim[b-1].x + 216); 
					point[row].y = random (- 192, - point[row].h);	
					continue;
				}
			}
			else if (f == i)
			{
				if (d == f - 1)
				{
					simpleBomb[row].x = random ((point[row].x + point[row].w) + 50, point[row].x + 216); 
					simpleBomb[row].y = random (- 192, - simpleBomb[row].h);
					continue;
				}
				else if (g == f - 1)
				{
					simpleBomb[row].x = random ((meatierBomb[row].x + meatierBomb[row].w) + 50, meatierBomb[row].x + 216); 
					simpleBomb[row].y = random (- 192, - simpleBomb[row].h);
					continue;
				}
				else
				{
					simpleBomb[row].x = random ((bdim[b-1].x + bdim[b-1].w) + 50, bdim[b-1].x + 216); 
					simpleBomb[row].y = random (- 192, - simpleBomb[row].h);	
					continue;
				}
			}
			else if (g == i)
			{
				if (f == g - 1)
				{
					meatierBomb[row].x = random ((simpleBomb[row].x + simpleBomb[row].w) + 50, simpleBomb[row].x + 216); 
					meatierBomb[row].y = random (- 192, - meatierBomb[row].h);	
					continue;
				}
				else if (d == g - 1)
				{
					meatierBomb[row].x = random ((point[row].x + point[row].w) + 50, point[row].x + 216); 
					meatierBomb[row].y = random (- 192, - meatierBomb[row].h);	
					continue;
				}
				else
				{
					meatierBomb[row].x = random ((bdim[b-1].x + bdim[b-1].w) + 50, bdim[b-1].x + 216); 
					meatierBomb[row].y = random (- 192, - meatierBomb[row].h);	
					continue;
				}
			}
			else
			{
				if (f == i - 1)
				{
					bdim[b].x = random ((simpleBomb[row].x + simpleBomb[row].w) + 50, simpleBomb[row].x + 216);
					bdim[b].y = random (- 192, - bdim[b].h);
					b += 1;
					continue;
				}
				else if (d == i - 1)
				{
					bdim[b].x = random ((point[row].x + point[row].w) + 50, point[row].x + 216);
					bdim[b].y = random (- 192, - bdim[b].h);
					b += 1;
					continue;
				}
				else if (g == i - 1)
				{
					// The problem here I think is because of the [i-1] within the array.
					bdim[b].x = random ((meatierBomb[row].x + meatierBomb[row].w) + 50, meatierBomb[row].x + 216);
					bdim[b].y = random (- 192, - bdim[b].h);
					b += 1;
					continue;
				}
				else
				{
					// The problem here I think is because of the [i-1] within the array.
					bdim[b].x = random ((bdim[b-1].x + bdim[b-1].w) + 50, bdim[b-1].x + 216);
					bdim[b].y = random (- 192, - bdim[b].h);
					b += 1;
					continue;
				}
			}
		}	
	}
}
//This sees if there is a collision between a bomb and the player
bool bomb::isThereBombCollision (int bombNum)
{
	iterion = bombCollision.find(bombNum);
	if (iterion != bombCollision.end())
	{
		return iterion->second;
	}
	else 
	{
		return false;
	}
}
bool bomb::insertBomb (int bomb, bool collision)
{			
	iterion = bombCollision.find(bomb);
	if (iterion != bombCollision.end ())
	{
		if (iterion->second != collision)
		{
			if (collision == true)
			{
				iterion->second = collision;
				return true;
			}
			else
			{
				//iterion->second = false;
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else{bombCollision.insert (Col(bomb, collision)); return true;}
}
//This sees if there is a collision between a point and the player
bool bomb::isTherePointCollision (int pointNum)
{
	iterion = pointCollision.find(pointNum);
	if (iterion != pointCollision.end())
	{
		return iterion->second;
	}
	else 
	{
		return false;
	}
}

bool bomb::insertPoint (int point, bool collision)
{			
	iterion = pointCollision.find(point);
	if (iterion != pointCollision.end ())
	{
		if (iterion->second != collision)
		{
			if (collision == true)
			{
				iterion->second = collision;
				return true;
			}
			else
			{
				//iterion->second = collision;
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else{pointCollision.insert (Col(point, collision)); return true;}
}
bool bomb::isThereSBombCollision (int bombNum)
{
	iterion = sBombCollision.find(bombNum);
	if (iterion != sBombCollision.end())
	{
		return iterion->second;
	}
	else 
	{
		return false;
	}
}
bool bomb::insertSBomb (int bomb, bool collision)
{			
	iterion = sBombCollision.find(bomb);
	if (iterion != sBombCollision.end ())
	{
		if (iterion->second != collision)
		{
			if (collision == true)
			{
				iterion->second = collision;
				return true;
			}
			else
			{
				//iterion->second = false;
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else{sBombCollision.insert (Col(bomb, collision));return true;}
}
bool bomb::isThereMBombCollision (int bombNum)
{
	iterion = mBombCollision.find(bombNum);
	if (iterion != mBombCollision.end())
	{
		return iterion->second;
	}
	else 
	{
		return false;
	}
}
bool bomb::insertMBomb (int bomb, bool collision)
{			
	iterion = mBombCollision.find(bomb);
	if (iterion != mBombCollision.end ())
	{
		if (iterion->second != collision)
		{
			if (collision == true)
			{
				iterion->second = collision;
				return true;
			}
			else
			{
				//iterion->second = false;
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else{mBombCollision.insert (Col(bomb, collision)); return true;}
}
void bomb::simpleIA (int row, SDL_Rect player)
{
	int spd = 3;
	if (player.y + 150 >= simpleBomb[row].y)
	{
		if (player.x + (player.w / 2) - spd > simpleBomb[row].x + (simpleBomb[row].w / 2))
		{
			simpleBomb[row].x += spd;
		}
		else if (player.x + (player.w / 2) + spd < simpleBomb[row].x + (simpleBomb[row].w / 2))
		{	
			simpleBomb[row].x -= spd;
		}
	}
	simpleBomb[row].y += speed;  
}
int bomb::checkMovement (int bomb, int check, int pRow[2])
{
	//enum CHECK {fourWayCheck, diagsCheck, curveRightCheck, curveUpCheck, curveLeftCheck, curveDownCheck};
	//Do raytrace that returns the pixels until collision and take the movement(in terms of the player's movements) as a parameter.
	//Use this method with NESW and determine the path that goes the farthest, then return the move that was picked.
	//If the first four paths don't return a distance higher than x, then try using the four diags.
	//This is just incaase I want to the distance traveled by the raytrace for each loop.
	int move;
	int loops = 1;
	int distanceBuf;
	playerRow[0] = pRow[0];
	playerRow[1] = pRow[1];
	ipreviousCheck[bomb] = icurrentCheck[bomb];
	switch (check)
	{
	case fourWayCheck:
		if (ipreviousCheck[bomb] != check)
		{
			move = 1;
			if(insertMBomb(bomb, false) == true)
			{
			}
		}
		else
		{
			loops = 1;
		}
		while (move > 5)
		{
			if ((meatierBomb[bomb].y + meatierBomb[bomb].h > rowy[i]) && (meatierBomb[bomb].y + meatierBomb[bomb].h < rowy[i] + (SCREEN_HEIGHT / 4)))
			{
				meatyRow[0] = i;
			}
			else if ((meatierBomb[bomb].y > rowy[i]) && (meatierBomb[bomb].y < rowy[i] + (SCREEN_HEIGHT / 4)))
			{
				meatyRow[1] = i;
				//Since the search will be going from bottom to top. After the top part of the player's row is determined, the rest of the for loop
				//isn't needed. That is why a break is here. :3
				break;
			}
		}
		for (int i = 0; i < loops; i++)
		{ 
			if (isThereMBombCollision(bomb + 5) == false)
			{
				if (testCollision(bomb) != true)
				{
					//I'm thinking a map would be the best place to store the values returned by this function.
					distanceBuf = raytrace(move, bomb, speed);
				}
				else
				{
					//Store distance along with the selected move within the rayDistance[bomb] map
					if (insertRayDistance(move, distanceBuf, bomb) == true)
					{
						move += 1;
						if (insertMBomb(bomb + 5, true) == true)
						{
						}
					}
				}
			}
		}
		break;
	case diagsCheck:
		break;
	case curveRightCheck:
		break;
	case curveUpCheck:
		break;
	case curveLeftCheck:
		break;
	case curveDownCheck:
		break;
	}
}
int bomb::implementMovement ()
{
	//While the bomb is already in the process of moving is where this function is called.
	//First we will check to see if it is optimal to use a curve at this present area. If so, then we will return
	//the curve that we think should be used. With every move we retu rn, we munst also be setting nodes up.
	//We just need to use nodes in a way that A* is able to search for a decent path.
	//
	//I want the bomb to look for an open path and to just go down it until it has almost reached the end, then checks
	//for a move that can get the bomb around the obstacle and closer to the player. He in that case doesn't need to know
	//the whole environment, it just needs to go down a clear path toward the player and just keep maneuvering around 
	//obstacles when it needs to.
	
	//The A* search can and will be used still. At the end of each path the player is going down we can place a node and
	//calculate the optimal path from a list of similarly placed nodes.


	
	return 0;
}
void bomb::meatierIA(int row, SDL_Rect player)
{
	meatierBomb[row].y += speed;
}
void bomb::applyBombs (SDL_Surface* dest)
{
	for (int i = 0; i < 15; i++)
	{
		iterion = bombCollision.find(i);
		if (iterion != bombCollision.end())
		{
			if (iterion->second != true)
			{
				apply_Surface (bdim[i].x, bdim[i].y, bombs, dest, NULL);
			}
		}
		else
		{
			apply_Surface (bdim[i].x, bdim[i].y, bombs, dest, NULL);
		}
		if (bSplosion[i] == true)
		{
			//apply Splosion
			apply_Surface (bdim[i].x, bdim[i].y, bombSplosion, dest, NULL);
		}
	}
	for (int i = 0; i < 5; i++)
	{
		iterion = pointCollision.find(i);
		if (iterion != pointCollision.end())
		{
			if (iterion->second != true)
			{
				apply_Surface (point[i].x, point[i].y, points, dest, NULL);
			}
		}
		else
		{
			apply_Surface (point[i].x, point[i].y, points, dest, NULL);
		}
		if (pSplosion[i] == true)
		{
			//apply Splosion
			apply_Surface (point[i].x, point[i].y, pointSplosion, dest, NULL);
		}
		if (rowDecider[i] == 0)
		{
			iterion = sBombCollision.find(i);
			if (iterion != sBombCollision.end())
			{
				if (iterion->second != true)
				{
					apply_Surface (simpleBomb[i].x, simpleBomb[i].y, sBomb, dest, NULL);
				}
			}
			else
			{
				apply_Surface (simpleBomb[i].x, simpleBomb[i].y, sBomb, dest, NULL);
			}
			if (bSplosion[i + 15] == true)
			{
				//apply Splosion
				apply_Surface (simpleBomb[i].x, simpleBomb[i].y, bombSplosion, dest, NULL);
			}
		}
		else
		{
			iterion = mBombCollision.find(i);
			if (iterion != mBombCollision.end())
			{
				if (iterion->second != true)
				{
					apply_Surface (meatierBomb[i].x, meatierBomb[i].y, mBomb, dest, NULL);
				}
			}
			else
			{
				apply_Surface (meatierBomb[i].x, meatierBomb[i].y, mBomb, dest, NULL);
			}
			if (bSplosion[i + 15] == true)
			{
				//apply Splosion
				apply_Surface (meatierBomb[i].x, meatierBomb[i].y, bombSplosion, dest, NULL);
			}
		}
	}
}
void bomb::moveBombs (SDL_Rect player)
{
	for (int i = 0; i < 20; i++)
	{
		bdim[i].y += speed;
		//if (i <= 4)
		//{
		//	point[i].y += speed;
		//}
	}
	for (int i = 0; i < 5; i++)
	{
		//These have been updated to fit the new screen height
		//768-spd
		if (rowy[i] <= SCREEN_HEIGHT)
		{
			rowy[i] += speed;
			point[i].y += speed;
			if (rowDecider[i] == 0)
			{
				simpleIA (i, player);
				//simpleBomb[i].y += speed;
			}
			else
			{
				meatierIA (i, player);
				//meatierBomb[i].y += speed;
			}
		}
		else 
		{
			rowy[i] = -(SCREEN_HEIGHT / 4);
			redrawRow[i] = true;
			point[i].y += speed;
			if (rowDecider[i] == 0)
			{
				simpleIA (i, player);
				//simpleBomb[i].y += speed;
			}
			else
			{
				meatierIA (i, player);
				//meatierBomb[i].y += speed;
			}
			if (i == 4)
			{
				wave += 1;
				if (wave % 5 == 0)
				{
					waveCount += 1;
				}
				spawnBombs();
			}
		}
	}
}
