#include "player.h"
#include <iostream>
#include "init.h"
#include "SDL.h"

using namespace std;

//#include "init.h"
bool main_Input;
PlayerEngine::PlayerEngine (int x, int y)
{
	pdim.x = x;
	pdim.y = y;
	pdim.w = 75;
	pdim.h = 75;
	//Gotta make my game portable :3 This is the speed of the player
	xspeed = 16;
	yspeed = 16; 
	boostSpd =  12;
	atkSpd = 10;
	hitPoints = 100;
	attack = false;
	attackRight = false;
	attackUp = false;
	attackLeft = false;
	attackDown = false;
	//The toast isn't initially moving
	moving = false;
	startCurve = false;
	x = 0;
	y = 0;
	theta = 0;
	toast = IMG_load ("newToastGreat.bmp");
	bacon = IMG_load ("toastGreat.bmp");
	waffle = IMG_load ("newToastDead.bmp");
}
PlayerEngine::~PlayerEngine ()
{
	//prenderer->~RenderEngine ();
	//delete prenderer;
	//prenderer = NULL;
	//SDL_FreeSurface (playerSprite);
}
void PlayerEngine::showChars ()
{
	apply_Surface (300, 300, toast, screen, NULL);
	apply_Surface (400, 300, bacon, screen, NULL);
	apply_Surface (500, 300, waffle, screen, NULL);
}
void PlayerEngine::freeChars (int i)
{
	if (i == 1)
	{
		//These won't be needed after the char has been selected
		SDL_FreeSurface(toast);
		SDL_FreeSurface(bacon);
		SDL_FreeSurface(waffle);
	}
	if (i == 2)
	{
		SDL_FreeSurface(effect);
		//SDL_FreeSurface(playerSprite);
		SDL_FreeSurface(playerGreat);
		SDL_FreeSurface(playerGood);
		SDL_FreeSurface(playerOk);
		SDL_FreeSurface(playerMeh);
		SDL_FreeSurface(playerZzz);
		SDL_FreeSurface(playerDead);
		SDL_FreeSurface(attack1);
		SDL_FreeSurface(attack2);
		SDL_FreeSurface(attack3);
		SDL_FreeSurface(attack4);
	}
}
void PlayerEngine::show (SDL_Surface* source, SDL_Surface* destination, SDL_Surface* effect)
{
	if (moving == true)
	{
		apply_Surface (pdim.x + effectOffset.x, pdim.y + effectOffset.y, effect, screen, NULL);
	}
	if (attackRight == true)
	{
		apply_Surface (pdim.x + 35, pdim.y - 10, attack1, screen, NULL);
	}
	else if (attackUp == true)
	{
		apply_Surface (pdim.x - 7, pdim.y - 30, attack2, screen, NULL);
	}
	else if (attackLeft == true)
	{
		apply_Surface (pdim.x - 25, pdim.y - 10, attack3, screen, NULL);
	}	
	else if (attackDown == true)
	{
		apply_Surface (pdim.x - 7, pdim.y + 29, attack4, screen, NULL);
	}	
	apply_Surface (pdim.x, pdim.y, source, destination, NULL);
}
SDL_Surface* PlayerEngine::playerGet ()
{
	if (hitPoints > 40) 
	{
		playerSprite = playerGreat;
	}
	else if (hitPoints <= 40 && hitPoints > 30)
	{
		playerSprite = playerGood;
	}
	else if (hitPoints <= 30 && hitPoints > 20)
	{
		playerSprite = playerOk;
	}
	else if (hitPoints <= 20 && hitPoints > 10)
	{
		playerSprite = playerMeh;
	}
	else if (hitPoints <= 10 && hitPoints > 0)
	{
		playerSprite = playerZzz;
	}
	else if (hitPoints <= 0)
	{
		playerSprite = playerDead;
		if (death != true)
		{
			death = true;
		}
	}
	return playerSprite;
}
SDL_Surface* PlayerEngine::effectGet ()
{
	return effect;
}
void PlayerEngine::setSpeed (int spd_x, int spd_y)
{
	xspeed = spd_x;
	yspeed = spd_y;
}
int PlayerEngine::beginDirectionCalc ()
{
	prevMKeysPressed = mKeysPressed;
	prevCKeysPressed = cKeysPressed;
	mKeysPressed = 0;
	cKeysPressed = 0;
	if (w)
	{
		mKeysPressed += 1;
	}
	if (a)
	{
		mKeysPressed += 1;
	}
	if (s)
	{
		mKeysPressed += 1;
	}
	if (d)
	{
		mKeysPressed += 1;
	}
	if (right)
	{
		cKeysPressed += 1;
	}
	if (up)
	{
		cKeysPressed += 1;
	}
	if (left)
	{
		cKeysPressed += 1;
	}
	if (down)
	{
		cKeysPressed += 1;
	}
	//Standby
	if (mKeysPressed == 0)
	{
		if (cKeysPressed == 0)
		{
		oneKey = false;
		standby = true;
		twoKeys = false;
		special = false;
		return 0;
		}
		//Mayber later I'll include this in a ranged shooting system.
		/*if (cKeysPressed == 1)
		{
			oneKey = false;
			standby = true;
			twoKeys = false;
			special = true;
			return 1;
		}*/
	}
	else if (mKeysPressed == 1)
	{
		if (cKeysPressed == 0)
		{
			oneKey = true;
			standby = false;
			twoKeys = false;
			special = false;
			return 1;
		}
		if (cKeysPressed == 1)
		{
			oneKey = true;
			standby = false;
			twoKeys = false;
			special = true;
			return 1;
		}
	}
	else if (mKeysPressed == 2)
	{
		if (cKeysPressed == 0)
		{
			oneKey = false;
			standby = false;
			twoKeys = true;
			special = false;
			return 1;
		}
		if (cKeysPressed == 1)
		{
			oneKey = false;
			standby = false;
			twoKeys = true;
			special = true;
			return 1;
		}
	}
}
int PlayerEngine::endDirectionCalc ()
{
	ipreviousMove = icurrentMove;
	attack = false;
	attackRight = false;
	attackUp = false;
	attackLeft = false;
	attackDown = false;
	if (standby != true)
	{
		if (oneKey == true)
		{
			if (special != true)
			{
				if (d)
				{
					icurrentMove = 1;			
				}
				if (w)
				{
					icurrentMove = 2;
				}
				if (a)
				{
					icurrentMove = 3;
				}
				if (s)
				{
					icurrentMove = 4;
				}
			}
			else
			{
				if (d)
				{
					if (up)
					{
						if (right)
						{
							icurrentMove = 6;
						}
						else if (left)
						{
							icurrentMove = 11;//7
						}
						else
						{
							icurrentMove = 5;
						}
					}
					else if (down)
					{
						if (left)
						{
							icurrentMove = 6;//10
						}
						else if (right)
						{
							icurrentMove = 11;
						}
						else
						{
							icurrentMove = 12;
						}
					}
					else if (left)
					{
						//attack bomb to the right
						icurrentMove = 17;
						attack = true;
					}
					else if (right)
					{
						//move to the right at a higher speed.
						icurrentMove = 1;
						xspeed = boostSpd;
					}
				}
				if (w)
				{
					if (right)
					{
						if (up)
						{
							icurrentMove = 5;
						}
						else if (down)
						{
							icurrentMove = 8;//12
						}
						else
						{
							icurrentMove = 6;
						}
					}
					else if (left)
					{
						if (up)
						{
							icurrentMove = 8;
						}
						else if (down)
						{
							icurrentMove = 5;//9
						}
						else
						{
							icurrentMove = 7;
						}
					}
					else if (up)
					{
						//boost up
						icurrentMove = 2;
						yspeed = boostSpd;
					}
					else if (down)
					{
						//attack up
						icurrentMove = 18;
						attack = true;
					}
				}
				if (a)
				{
					if (up)
					{
						if (left)
						{
							icurrentMove = 7;
						}
						else if (right)
						{
							icurrentMove = 10;//6
						}
						else
						{
							icurrentMove = 8;
						}
					}
					else if (down)
					{
						if (left)
						{
							icurrentMove = 10;
						}
						else if (right)
						{
							icurrentMove = 7;//11
						}
						else
						{
							icurrentMove = 9;
						}
					}
					else if (left)
					{
						//boost left
						icurrentMove = 3;
						xspeed = boostSpd;
					}
					else if (right)
					{
						//attack left
						icurrentMove = 19;
						attack = true;
					}
				}
				if (s)
				{
					if (left)
					{
						if (up)
						{
							icurrentMove = 12;//8
						}
						else if (down)
						{
							icurrentMove = 9;
						}
						else
						{
							icurrentMove = 10;
						}
					}
					else if (right)
					{
						if (up)
						{
							icurrentMove = 9;//5
						}
						else if (down)
						{
							icurrentMove = 12;
						}
						else
						{
							icurrentMove = 11;
						}
					}
					else if (up)
					{
						//attack down
						icurrentMove = 20;
						attack = true;
					}
					else if (down)
					{
						//boost down
						icurrentMove = 4;
						yspeed = boostSpd;
					}
				}
			}
		}
		else if (twoKeys == true)
		{
			if (d && w)
			{
				icurrentMove = 13;		
			}
			if (w && a)
			{
				icurrentMove = 14;
			}
			if (a && s)
			{
				icurrentMove = 15;
			}
			if (s && d)
			{
				icurrentMove = 16;
			}
		}
	} 
	else
	{
		icurrentMove = 0;
	}
	return icurrentMove;
}
void PlayerEngine::move (int Direction)
{
	if (death != true)
	{
		enum DIRECTION {standby, right, up, left, down, curve1, curve2, curve3, curve4, curve5, curve6, curve7, curve8, diag1, diag2, diag3, diag4, atk1, atk2, atk3, atk4};
		switch (Direction)
		{
		case standby:
			break;
		case right:
			pdim.x += xspeed;
			break;
		case up:
			pdim.y -= yspeed;
			break;
		case left:
			pdim.x -= xspeed;
			break;
		case down:
			pdim.y += yspeed;
			break;
		case curve1:
			if (ipreviousMove != 5)
			{
				startCurve = false;
			}
			if (startCurve == false)
			{
				//vertexX = pdim.x + 100;
				//vertexY = pdim.y;
				xCurve = 0;
				yCurve = yspeed * 1.25;
				//X = 100;
				//Y = 0;
				startCurve = true;
				//theta = 3.141592;
			}
			if (xCurve > xspeed * 1.25 && yCurve < 0)
			{
				pdim.x += xspeed;
			}
			else 
			{
				xCurve += 0.4;//xspeed / 25;
				yCurve -= 0.4;//yspeed / 25;
				pdim.x += xCurve;// += X;
				pdim.y -= yCurve;
			}
			/*if (theta > (3.14/2))
			{
				theta -= curveSpd;
				xCurve += .00008;
				yCurve -= .00008;
				x = (cos(theta))* 100;
				y = (sin(theta))* 100;
				//X = x + X;	//is is how
				// = y - Y;
				pdim.x += xCurve;// += X;
				pdim.y -= yCurve;// -= Y;//vertexY - y - pdim.y;
			}
			else
			{
				pdim.x += xspeed;
			}*/
			break;
		case curve2:
			if (ipreviousMove != 6)
			{
				startCurve = false;
			}
			if (startCurve == false)
			{
				xCurve = xspeed * 1.25;
				yCurve = 0;
				startCurve = true;
			}
			if (xCurve < 0 && yCurve > yspeed * 1.25)
			{
				pdim.y -= yspeed;
			}
			else 
			{
				xCurve -= 0.4;//xspeed / 25;
				yCurve += 0.4;//yspeed / 25;
				pdim.x += xCurve;// += X;
				pdim.y -= yCurve;
			}
			break;
		case curve3:
			if (ipreviousMove != 7)
			{
				startCurve = false;
			}
			if (startCurve == false)
			{
				xCurve = xspeed * 1.25;
				yCurve = 0;
				startCurve = true;
			}
			if (xCurve < 0 && yCurve > yspeed * 1.25)
			{
				pdim.y -= yspeed;
			}
			else 
			{
				xCurve -= 0.4;//xspeed / 25;
				yCurve += 0.4;//yspeed / 25;
				pdim.x -= xCurve;// += X;
				pdim.y -= yCurve;
			}
			break;
		case curve4:
			if (ipreviousMove != 8)
			{
				startCurve = false;
			}
			if (startCurve == false)
			{
				xCurve = 0;
				yCurve = yspeed * 1.25;
				startCurve = true;
			}
			if (xCurve > xspeed * 1.25 && yCurve < 0)
			{
				pdim.x -= xspeed;
			}
			else 
			{
				xCurve += 0.4;//xspeed / 25;
				yCurve -= 0.4;//yspeed / 25;
				pdim.x -= xCurve;// += X;
				pdim.y -= yCurve;
			}
			break;
		case curve5:
			if (ipreviousMove != 9)
			{
				startCurve = false;
			}
			if (startCurve == false)
			{
				xCurve = 0;
				yCurve = yspeed * 1.25;
				startCurve = true;
			}
			if (xCurve > xspeed * 1.25 && yCurve < 0)
			{
				pdim.x -= xspeed;
			}
			else 
			{
				xCurve += 0.4;//xspeed / 25;
				yCurve -= 0.4;//yspeed / 25;
				pdim.x -= xCurve;// += X;
				pdim.y += yCurve;
			}
			break;
		case curve6:
			if (ipreviousMove != 10)
			{
				startCurve = false;
			}
			if (startCurve == false)
			{
				xCurve = xspeed * 1.25;
				yCurve = 0;
				startCurve = true;
			}
			if (xCurve < 0 && yCurve > yspeed * 1.25)
			{
				pdim.y += yspeed;
			}
			else 
			{
				xCurve -= 0.4;//xspeed / 25;
				yCurve += 0.4;//yspeed / 25;
				pdim.x -= xCurve;// += X;
				pdim.y += yCurve;
			}
			break;
		case curve7:
			if (ipreviousMove != 11)
			{
				startCurve = false;
			}
			if (startCurve == false)
			{
				xCurve = xspeed * 1.25;
				yCurve = 0;
				startCurve = true;
			}
			if (xCurve < 0 && yCurve > yspeed * 1.25)
			{
				pdim.y += yspeed;
			}
			else 
			{
				xCurve -= 0.4;//xspeed / 25;
				yCurve += 0.4;//yspeed / 25;
				pdim.x += xCurve;// += X;
				pdim.y += yCurve;
			}
			break;
		case curve8:
			if (ipreviousMove != 12)
			{
				startCurve = false;
			}
			if (startCurve == false)
			{
				xCurve = 0;
				yCurve = yspeed * 1.25;
				startCurve = true;
			}
			if (xCurve > xspeed * 1.25 && yCurve < 0)
			{
				pdim.x += xspeed;
			}
			else 
			{
				xCurve += 0.4;//xspeed / 25;
				yCurve -= 0.4;//yspeed / 25;
				pdim.x += xCurve;// += X;
				pdim.y += yCurve;
			}
			break;
		case diag1:
			pdim.x += xspeed/1.5;
			pdim.y -= yspeed/1.5;
			break;
		case diag2:
			pdim.x -= xspeed/1.5;
			pdim.y -= yspeed/1.5;
			break;
		case diag3:
			pdim.x -= xspeed/1.5;
			pdim.y += yspeed/1.5;
			break;
		case diag4:
			pdim.x += xspeed/1.5;
			pdim.y += yspeed/1.5;
			break;
		case atk1:
			//Use something similar to the curve movements to use the atk. So holding down these buttons is the only way
			//of completing the atk.
			attackRight = true;
			pdim.x += atkSpd;
			break;
		case atk2:
			attackUp = true;
			pdim.y -= atkSpd;
			break;
		case atk3:
			attackLeft = true;
			pdim.x -= atkSpd;
			break;
		case atk4:
			attackDown = true;
			pdim.y += atkSpd;
			break;
		}
	}
}
bool PlayerEngine::chooseChar (int character)
{
	if (character == 1)
	{
		//1 == toast
		//playerSprite = toast;
		player = "toast";
		effectOffset.x = 0;
		effectOffset.y = 67;
		fileGreat = "newToastGreat.bmp";
		fileGood = "newToastGood.bmp";
		fileOk = "newToastOk.bmp";
		fileMeh = "newToastMeh.bmp";
		fileZzz = "newToastZzz.bmp";
		fileDead = "newToastDead.bmp";
		fileEffect = "jetpackflame.bmp";
		atk1 = "toastAtk1.bmp";
		atk2 = "toastAtk2.bmp";
		atk3 = "toastAtk3.bmp";
		atk4 = "toastAtk4.bmp";
	}
	if (character == 2)
	{
		//2 == bacon
		playerSprite = bacon;
		//effect =
		//effectOffset.x = ;
		//effectOffset.y = ;
		atk1 = "atk1.bmp";
		atk2 = "atk2.bmp";
		atk3 = "atk3.bmp";
		atk4 = "atk4.bmp";
	}
	if (character == 3)
	{
		//3 == waffle
		playerSprite = waffle;
		//effect =
		//effectOffset.x = ;
		//effectOffset.y = ;
		atk1 = "atk1.bmp";
		atk2 = "atk2.bmp";
		atk3 = "atk3.bmp";
		atk4 = "atk4.bmp";
	}
	bool merr = loadPlayer();
	if (merr == false)
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool PlayerEngine::loadPlayer()
{
	playerGreat = IMG_load(fileGreat); 
	if (playerGreat == NULL)
	{
		cout << "playerGreat in PlayerEngine::loadPlayer() has failed to load!\n";
		return false;
	}
	playerGood = IMG_load(fileGood);
	if (playerGood == NULL)
	{
		cout << "playerGood in PlayerEngine::loadPlayer() has failed to load!\n";
		return false;
	}
	playerOk = IMG_load(fileOk);
	if (playerOk == NULL)
	{
		cout << "playerOk in PlayerEngine::loadPlayer() has failed to load!\n";
		return false;
	}
	playerMeh = IMG_load(fileMeh);
	if (playerMeh == NULL)
	{
		cout << "playerMeh in PlayerEngine::loadPlayer() has failed to load!\n";
		return false;
	}
	playerZzz = IMG_load(fileZzz);
	if (playerZzz == NULL)
	{
		cout << "playerZzz in PlayerEngine::loadPlayer() has failed to load!\n";
		return false;
	}
	playerDead = IMG_load(fileDead);
	if (playerDead == NULL)
	{
		cout << "playerDead in PlayerEngine::loadPlayer() has failed to load!\n";
		return false;
	}
	effect = IMG_load(fileEffect);
	if (effect == NULL)
	{
		cout << "effect in PlayerEngine::loadPlayer() has failed to load!\n";
		return false;
	}
	attack1 = IMG_load(atk1);
	if (attack1 == NULL)
	{
		cout << "attack1 in PlayerEngine::loadPlayer() has failed to load!\n";
		return false;
	}
	attack2 = IMG_load(atk2);
	if (attack2 == NULL)
	{
		cout << "attack2 in PlayerEngine::loadPlayer() has failed to load!\n";
		return false;
	}
	attack3 = IMG_load(atk3);
	if (attack3 == NULL)
	{
		cout << "attack3 in PlayerEngine::loadPlayer() has failed to load!\n";
		return false;
	}
	attack4 = IMG_load(atk4);
	if (attack4 == NULL)
	{
		cout << "attack4 in PlayerEngine::loadPlayer() has failed to load!\n";
		return false;
	}

	return true;
}
void PlayerEngine::takeDamage (int damage)
{
	hitPoints -= damage;
}