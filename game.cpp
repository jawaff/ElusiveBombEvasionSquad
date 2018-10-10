#include "game.h"
#include <iostream>
#include "InputEngine.h"
#include "player.h"
#include "bomb.h"
#include "score.h"
#include "timer.h"

using namespace std;

GameEngine *engine = NULL;

//mununununununununununununununu... That is all for today.
 
////////////////////////////////////////////////////
//menu things
int arrowx = 300;
int arrowy = 300;
SDL_Surface *intro;
SDL_Surface *menu;
SDL_Surface *arrow;
//Game things
SDL_Surface *background;
SDL_Surface *gameOver;
SDL_Surface *rawr;
SDL_Surface *pauseBack;
SDL_Surface *selectBack;
SDL_Surface *star;
bool acceptFlag = false;
bool collision;
int distanceAB;
//playerRow is an array because it holds the row for the top and bottom of the player. [0] = bottom, [1] = top.
int playerRow[2];
//This runs the menu function that is to be returned to main.cpp's update_State() method
int showMenu ()
{
	/////////////////////////////////////////////////////
    mload_Images ();
	if (mload_Images () == false)
	{	
		cout << "mload_image () failed to initialize\n";
	}
	engine = GameEngine::instance ();
	if (engine == NULL)
	{
		cout << "engine failed to initialize\n";
	}
	/////////////////////////////////////////////////////
	//set up the different variables and functions that I will need for the menu
	bool quit_Menu = false;	
	int menustate = 0;
	int optselect = 3;
	bool keyFlag = false;
	switchMenuOptions (optselect);

	//main input loop for the menu
	while (quit_Menu == false)
	{
		//{SDL_Event event; while (SDL_PollEvent(&event));}
		//SDL_PumpEvents ();

		engine->handle_Input ();
		//check if x on window is clicked
		if (engine->Quit () == true)
		{
			mclean_Up ();
			return 0;
		}
		//intro to menu
		if (menustate == 0)
		{
			apply_Surface (0, 0, intro, screen, NULL);
			SDL_Flip (screen);

			if (engine->key_Down("y"))
			{
				menustate = 1;
			}
			if (engine->key_Down("n"))
			{
				mclean_Up ();
				return 0;
			}
		}

		//main menu
		if (menustate == 1)
		{
			apply_Surface (0, 0, menu, screen, NULL);
			apply_Surface (arrowx, arrowy, arrow, screen, NULL);
			SDL_Flip (screen);

			//check for user input to switch the options in the main menu
			if (engine->key_Down("up") || engine->key_Down("down"))
			{
				if (keyFlag == false)
				{
					keyFlag = true;
					if (engine->key_Down("up"))
					{
						optselect++; if (optselect > 3) {optselect = 1;}
					}
					else
					{
						optselect--; if (optselect < 1) {optselect = 3;}
					}
					//switch the arrow's location after user input detection
					switchMenuOptions (optselect);
				}
			}
			else {keyFlag = false;}

			//check for user hitting enter to proceed into the game further
			if (engine->key_Down("enter"))
			{
				if (acceptFlag == false)
				{
					acceptFlag = true;
					//Continue
					if (optselect == 3)
					{
						SDL_FreeSurface (intro);
						SDL_FreeSurface (menu);
						return 1;
					}
					//Instructions
					if (optselect == 2)
					{
						//instructions page is under construction %^&*(*&^%$#@#$%^
					}
					//Exit Game
					if (optselect == 1)
					{
						mclean_Up ();
						return 0;
					}
				}
			}
			else
			{ 
				acceptFlag = false;
			}
		}
		//instructions part of the menu
		//if (menustate == 3)
		//{

		//}
	}//end while loop for the menu input
	return 1;
}

void switchMenuOptions (int options)
{
	//Start Game
	if (options == 3)
	{
		arrowy = 300;
	}
	//instructions
	if (options == 2)
	{
		arrowy = 370;
	}
	//Quit Game
	if (options == 1)
	{
		arrowy = 440;
	}
}

bool mload_Images ()
{	
	intro = IMG_load ("intro_back.bmp");
	if (intro == NULL)
	{
		cout << "error while loading intro page/n";
		return false;
	}

	menu = IMG_load ("menu_back.bmp");
	if (menu == NULL)
	{
		cout << "error while loading menu page/n";
		return false;
	}

	arrow = IMG_load ("arrow.bmp");
	if (arrow == NULL)
	{
		cout << "error while loading arrow/n";
		return false;
	}
	return true;
}

void mclean_Up ()
{
	SDL_FreeSurface (intro);
	SDL_FreeSurface (menu);
	SDL_FreeSurface (arrow);
}

void switchChar (int character)
{
	//toast
	if (character == 1)
	{
		arrowx = 325;
	}
	//bacon
	if (character == 2)
	{
		arrowx = 425;
	}
	//waffle
	if (character == 3)
	{
		arrowx = 525;
	}
}

bool collisionDetect (SDL_Rect A, SDL_Rect B)
{
	//Checks if the images are touching in terms of the x-axis. Disregards the y axis.
	if (A.x > B.x && A.x < B.x + B.w || A.x + A.w > B.x && A.x + A.w < B.x + B.w)
	{
		//Checks if the images are both touching in terms of the y-axis. Completely disregards the x-axis.
		if (A.y > B.y && A.y < B.y + B.h || A.y + A.h > B.y && A.y + A.h < B.y + B.h)
		{
			return true;
		}
	}
}

//This is for my newer collision system that will be based on the radius of the bombs and the player's collision zone. It will return distance squared.
//P.S. This distance formula will be made specifically for the toast and bombs.
int distanceFormula (SDL_Rect A, SDL_Rect B)
{
	int Ax, Ay, Bx, By;
	//Offsetting toast's x & y position to the middle
	Ax = A.x + 38;
	Ay = A.y + 35;

	//Offsetting bomb's x & y position to the middle
	Bx = B.x + 25;
	By = B.y + 25;

	int distanceSquared;
	distanceSquared = ((Bx - Ax)*(Bx - Ax)) + ((By - Ay)*(By - Ay));

	return distanceSquared;
}
//14

//bool collisionDetect (SDL_Rect A, SDL_Rect B)
//{
//	A.x += A.x + (A.w / 2);
//	A.y += A.y + (A.h / 2);
//	B.x += B.x + (B.w / 2);
//	B.x += B.x + (B.h / 2);
//	
//}
int playGame ()
{
	//////////////////////////////////////////////////////////////
	//This will result with the selection screen.
	selectBack = IMG_load ("choosechar_back.bmp");
	if (selectBack == NULL)
	{
		cout << "selectBack failed to load\n";
	}
	star = IMG_load ("star.bmp");
	if (star == NULL)
	{
		cout << "star failed to load\n";
	}
	//The height is 95 with the flames from the jetpack.
	PlayerEngine Player (400, 500);
	bool selectQuit = false;
	bool keyFlag = false;
	int optselect = 1;
	arrowy = 320;
	arrowx = 325;
	while (selectQuit == false)
	{		
		engine->handle_Input ();
		//check if x on window is clicked
		if (engine->Quit () == true)
		{
			clean_Up ();
			exit (1);
		}
		apply_Surface (0, 0, selectBack, screen, NULL);
		Player.showChars ();
		apply_Surface (arrowx, arrowy, star, screen, NULL);
		SDL_Flip (screen);
		if (engine->key_Down("left") || engine->key_Down("right"))
		{
				if (keyFlag == false)
				{
					keyFlag = true;
					if (engine->key_Down("right"))
					{
						optselect++; if (optselect > 3) {optselect = 1;}
					}
					else
					{
						optselect--; if (optselect < 1) {optselect = 3;}
					}
					//switch the arrow's location after user input detection
					switchChar (optselect);
				}
		}
		else
		{
			keyFlag = false;
		}
		if (engine->key_Down("enter"))
		{
			if (acceptFlag == false)
			{
				acceptFlag = true;
				if(Player.chooseChar(optselect) == NULL)
				{
					selectQuit = false;
				}
				else
				{
					selectQuit = true;
				}
			}
		}
		else
		{
			acceptFlag = false;
		}
	}
	//Cleaning is good for the environment bro!
	SDL_FreeSurface (selectBack);
	SDL_FreeSurface (star);
	Player.freeChars(1);
	///////////////////////////////////////////////////
	//Loading images
	background = IMG_load ("background.bmp");
	if (background == NULL)
	{
		cout << "background failed to load within gameGo ()\n";
	}
	bombs = IMG_load ("bomb.bmp");
	if (bombs == NULL)
	{
		cout << "Bombs failed to load!\n";
	}
	sBomb = IMG_load ("simpleBomb.bmp");
	if (sBomb == NULL)
	{
		cout << "sBomb failed to load!\n";
	}
	mBomb = IMG_load ("meatierBomb.bmp");
	if (mBomb == NULL)
	{
		cout << "mBomb failed to load!\n";
	}
	points = IMG_load ("point.bmp");
	if (points == NULL)
	{
		cout << "points did not load correctly!\n";
	}
	bombSplosion = IMG_load ("splosion.bmp");
	if (bombs == NULL)
	{
		cout << "Bombs failed to load!\n";
	}
	pointSplosion = IMG_load ("splosion.bmp");
	if (points == NULL)
	{
		cout << "points did not load correctly!\n";
	}
	pauseBack = IMG_load ("pause_back.bmp");
	if (pauseBack == NULL)
	{
		cout << "The pause background failed to load!\n";
	}
	///////////////////////////////////////////////////
	////This is defined within showMenu (). Don't forget to delete this after uncommenting it.
	//engine = GameEngine::instance ();
	//if (engine == NULL)
	//{
	//	cout << "engine failed to initialize\n";
	//}

	//Bomb = bomb::instance ();
	//if (Bomb == NULL)
	//{
	//	cout << "Bomb failed to initialize\n";
	//}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Setting up the game and such
	//IMPORTANT FOR CLASSES!!!!!!! IF THERE AREN'T ANY PARAMETERS NEEDED THEN DOWN USE THE "()"!!!!!!!!!!!!
	bomb Bomb;
	Bomb.spawnBombs ();
	Bomb.bombPlacement ();
	//Initialize our score keeper class
	ScoreKeeper Score;
	//This is for the movement system and determining the type of move.
	timer Timer;
	//!!!!!!!!!!!!!!!! IF YOU CHANGE THE SPEED MAKE SURE TO CHANGE THE SPEED in the else statement that changes
	//the speed back to normal after Lshift isn't being pressed anymore.
	Player.setSpeed (Player.xspeed, Player.yspeed);
	//This is for the while loop
	bool game = true;
	//The player hasn't collided with anything yet, so this needs to be set to false.
	collision = false;
	bool secondKeyFlag = false;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	while (game == true)
	{
		engine->handle_Input ();
		if (engine->Quit() == true)
		{
			clean_Up ();
			Player.freeChars(2);
			exit (1);
		}
		apply_Surface (0, 0, background, screen, NULL);
		Player.show (Player.playerGet(), screen, Player.effectGet());
		Bomb.applyBombs (screen);
		Score.applyScore();
		Bomb.moveBombs (Player.pdim);
		SDL_Flip (screen);
		if (Player.death == true)
		{
			game = false;
		}
		//Checks if the rows have hit the bottom of the screen yet. So that they can then have the coordinates reassigned
		//and redrawn.
		for (int i = 0; i < 5; i++)
		{
			if (Bomb.redrawRow[i] == true)
			{
				Bomb.rowBombPlacement (i);
				Bomb.redrawRow[i] = false;
			}
			if ((Player.pdim.y + Player.pdim.h > Bomb.rowy[i]) && (Player.pdim.y + Player.pdim.h < Bomb.rowy[i] + (SCREEN_HEIGHT / 4)))
			{
				playerRow[0] = i;
			}
			else if ((Player.pdim.y > Bomb.rowy[i]) && (Player.pdim.y < Bomb.rowy[i] + (SCREEN_HEIGHT / 4)))
			{
				playerRow[1] = i;
				//Since the search will be going from bottom to top. After the top part of the player's row is determined, the rest of the for loop
				//isn't needed. That is why a break is here. :3
				break;
			}
		}
		for (int i = 0; i < 25; i++)
		{
			if (Timer.get_Ticks(i) > Bomb.splosionTime)
			{
				Bomb.bSplosion[i] = false;
				Timer.stop(i);
			}
		}
		//This variable is basically for the i < ... in a for loop. It is the last bomb that will be checked for collision.
		int bombMax;
		if (playerRow[0] == playerRow[1])
		{
			bombMax = (playerRow[0] + 1) * 3; 
			for (int i = bombMax - 3; i < bombMax; i++)
			{
				distanceAB = distanceFormula (Player.pdim, Bomb.bdim[i]);
				//52 was determined by adding the radius' of the bomb and player together. Since the player is a square, I just chose a good
				//radius that seemed ok (27.)
				if (Bomb.isThereBombCollision(i) == false)
				{
					if (distanceAB < 2916)
					{
						if (Bomb.insertBomb(i, true) == true)
						{
							if (Player.attack != true)
							{
								/*Decrement score*/
								Score.decrementScore(Bomb.bombCost);
								Player.takeDamage(Bomb.bombDamage);
								Timer.start(i);
								Bomb.bSplosion[i] = true;
							}
							else
							{
								Timer.start(i);
								Bomb.bSplosion[i] = true;
							}
						}
					}	
				}
				else 
				{
					if (distanceAB > 2916)
					{
						if (Bomb.insertBomb(i, false) == true)
						{
							//
						}
					}
				}
			}

			distanceAB = distanceFormula (Player.pdim, Bomb.point[playerRow[0]]);
			if (Bomb.isTherePointCollision(playerRow[0]) == false)
			{
				if (distanceAB < 2916)
				{
					if (Bomb.insertPoint(playerRow[0], true) == true)
					{
						/*Decrement score*/
						Score.incrementScore(Bomb.pointCost);
						Timer.start(20 + playerRow[0]);
						Bomb.pSplosion[playerRow[0]] = true;
					}
				}	
			}
			else 
			{
				if (distanceAB > 2916)
				{
					if (Bomb.insertPoint(playerRow[0], false) == false)
					{
						//
					}
				}
			}
			distanceAB = distanceFormula (Player.pdim, Bomb.simpleBomb[playerRow[0]]);
			if (Bomb.isThereSBombCollision(playerRow[0]) == false)
			{
				if (distanceAB < 2916)
				{
					if (Bomb.insertSBomb(playerRow[0], true) == true)
					{
						if (Player.attack != true)
						{
							/*Decrement score*/
							Score.decrementScore(Bomb.bombCost);
							Player.takeDamage(Bomb.bombDamage);
							Timer.start(playerRow[0] + 15);
							Bomb.bSplosion[playerRow[0] + 15] = true;
						}
						else
						{
							Timer.start(playerRow[0] + 15);
							Bomb.bSplosion[playerRow[0] + 15] = true;
						}
					}
				}	
			}
			else 
			{
				if (distanceAB > 2916)
				{
					if (Bomb.insertSBomb(playerRow[0], false) == true)
					{
						//
					}
				}
			}
			distanceAB = distanceFormula (Player.pdim, Bomb.meatierBomb[playerRow[0]]);
			if (Bomb.isThereMBombCollision(playerRow[0]) == false)
			{
				if (distanceAB < 2916)
				{
					if (Bomb.insertMBomb(playerRow[0], true) == true)
					{
						if (Player.attack != true)
						{
							/*Decrement score*/
							Score.decrementScore(Bomb.bombCost);
							Player.takeDamage(Bomb.bombDamage);
							Timer.start(playerRow[0] + 15);
							Bomb.bSplosion[playerRow[0] + 15] = true;
						}
						else
						{
							Timer.start(playerRow[0] + 15);
							Bomb.bSplosion[playerRow[0] + 15] = true;
						}
					}
				}	
			}
			else 
			{
				if (distanceAB > 2916)
				{
					if (Bomb.insertMBomb(playerRow[0], false) == true)
					{
						//
					}
				}
			}
		}
		else 
		{
			bombMax = (playerRow[1] + 1) * 3;
			for (int i = bombMax - 3; i < bombMax; i++)
			{
				distanceAB = distanceFormula (Player.pdim, Bomb.bdim[i]);
				//52 was determined by adding the radius' of the bomb and player together. Since the player is a square, I just chose a good
				//Toast radius is 29px, 2916 is (25px + 29px) ^2
				if (Bomb.isThereBombCollision(i) == false)
				{
					if (distanceAB < 2916)
					{
						if (Bomb.insertBomb(i, true) == true)
						{
							if (Player.attack != true)
							{
								/*Decrement score*/
								Score.decrementScore(Bomb.bombCost);
								Player.takeDamage(Bomb.bombDamage);
								Timer.start(i);
								Bomb.bSplosion[i] = true;
							}
							else
							{
								Timer.start(i);
								Bomb.bSplosion[i] = true;
							}
						}
					}	
				}
				else 
				{
					if (distanceAB > 2916)
					{
						if (Bomb.insertBomb(i, false) == true)
						{
							//
						}
					}
				}				
			}

			distanceAB = distanceFormula (Player.pdim, Bomb.point[playerRow[1]]);
			if (Bomb.isTherePointCollision(playerRow[1]) == false)
			{
				if (distanceAB < 2916)
				{
					if (Bomb.insertPoint(playerRow[1], true) == true)
					{
						/*Increment score*/
						Score.incrementScore(Bomb.pointCost);
						Timer.start(20 + playerRow[1]);
						Bomb.pSplosion[playerRow[1]] = true;
					}
				}	
			}
			else 
			{
				if (distanceAB > 2916)
				{
					if (Bomb.insertPoint(playerRow[1], false) == true)
					{
						//Bomb.insertPoint(playerRow[1],false);
					}
				}
			}			
			distanceAB = distanceFormula (Player.pdim, Bomb.simpleBomb[playerRow[1]]);
			if (Bomb.isThereSBombCollision(playerRow[1]) == false)
			{
				if (distanceAB < 2916)
				{
					if (Bomb.insertSBomb(playerRow[1], true) == true)
					{
						if (Player.attack != true)
						{
							/*Decrement score*/
							Score.decrementScore(Bomb.bombCost);
							Player.takeDamage(Bomb.bombDamage);
							Timer.start(playerRow[1] + 15);
							Bomb.bSplosion[playerRow[1] + 15] = true;
						}
						else
						{
							Timer.start(playerRow[1] + 15);
							Bomb.bSplosion[playerRow[1] + 15] = true;
						}
					}
				}	
			}
			else 
			{
				if (distanceAB > 2916)
				{
					if (Bomb.insertSBomb(playerRow[1], false) == true)
					{
						//
					}
				}
			}
			distanceAB = distanceFormula (Player.pdim, Bomb.meatierBomb[playerRow[1]]);
			if (Bomb.isThereMBombCollision(playerRow[1]) == false)
			{
				if (distanceAB < 2916)
				{
					if (Bomb.insertMBomb(playerRow[1], true) == true)
					{
						if (Player.attack != true)
						{
							/*Decrement score*/
							Score.decrementScore(Bomb.bombCost);
							Player.takeDamage(Bomb.bombDamage);
							Timer.start(playerRow[1] + 15);
							Bomb.bSplosion[playerRow[1] + 15] = true;
						}
						else
						{
							Timer.start(playerRow[1] + 15);
							Bomb.bSplosion[playerRow[1] + 15] = true;
						}
					}
				}	
			}
			else 
			{
				if (distanceAB > 2916)
				{
					if (Bomb.insertMBomb(playerRow[1], false) == true)
					{
						//
					}
				}
			}
			bombMax = (playerRow[0] + 1) * 3;
			for (int i = bombMax - 3; i < bombMax; i++)
			{
				distanceAB = distanceFormula (Player.pdim, Bomb.bdim[i]);
				//52 was determined by adding the radius' of the bomb and player together. Since the player is a square, I just chose a good
				//Toast radius is 29px, 2916 is (25px + 29px) ^2
				if (Bomb.isThereBombCollision(i) == false)
				{
					if (distanceAB < 2916)
					{
						if (Bomb.insertBomb(i, true) == true)
						{
							if (Player.attack != true)
							{
								/*Decrement score*/
								Score.decrementScore(Bomb.bombCost);
								Player.takeDamage(Bomb.bombDamage);
								Timer.start(i);
								Bomb.bSplosion[i] = true;
							}
							else
							{
								Timer.start(i);
								Bomb.bSplosion[i] = true;
							}
						}
					}	
				}
				else 
				{
					if (distanceAB > 2916)
					{
						if (Bomb.insertBomb(i, false) == true)
						{
							//
						}
					}
				}	
			}

			distanceAB = distanceFormula (Player.pdim, Bomb.point[playerRow[0]]);
			if (Bomb.isTherePointCollision(playerRow[0]) == false)
			{
				if (distanceAB < 2916)
				{
					if (Bomb.insertPoint(playerRow[0], true) == true)
					{
						/*Increment score*/
						Score.incrementScore(Bomb.pointCost);
						Timer.start(20 + playerRow[0]);
						Bomb.pSplosion[playerRow[0]] = true;
					}
				}
			}
			else 
			{
				if (distanceAB > 2916)
				{
					if (Bomb.insertPoint(playerRow[0], false) == true)
					{
						//Bomb.insertPoint(playerRow[0],false);
					}
				}
			}	
			distanceAB = distanceFormula (Player.pdim, Bomb.simpleBomb[playerRow[0]]);
			if (Bomb.isThereSBombCollision(playerRow[0]) == false)
			{
				if (distanceAB < 2916)
				{
					if (Bomb.insertSBomb(playerRow[0], true) == true)
					{
						if (Player.attack != true)
						{
							/*Decrement score*/
							Score.decrementScore(Bomb.bombCost);
							Player.takeDamage(Bomb.bombDamage);
							Timer.start(playerRow[0] + 15);
							Bomb.bSplosion[playerRow[0] + 15] = true;
						}
						else
						{
							Timer.start(playerRow[0] + 15);
							Bomb.bSplosion[playerRow[0] + 15] = true;
						}
					}
				}	
			}
			else 
			{
				if (distanceAB > 2916)
				{
					if (Bomb.insertSBomb(playerRow[0], false) == true)
					{
						//Bomb.insertBomb(i,false);
					}
				}
			}
			distanceAB = distanceFormula (Player.pdim, Bomb.meatierBomb[playerRow[0]]);
			if (Bomb.isThereMBombCollision(playerRow[0]) == false)
			{
				if (distanceAB < 2916)
				{
					if (Bomb.insertMBomb(playerRow[0], true) == true)
					{
						if (Player.attack != true)
						{
							/*Decrement score*/
							Score.decrementScore(Bomb.bombCost);
							Player.takeDamage(Bomb.bombDamage);
							Timer.start(playerRow[0] + 15);
							Bomb.bSplosion[playerRow[0] + 15] = true;
						}
						else
						{
							Timer.start(playerRow[0] + 15);
							Bomb.bSplosion[playerRow[0] + 15] = true;
						}
					}
				}	
			}
			else 
			{
				if (distanceAB > 2916)
				{
					if (Bomb.insertMBomb(playerRow[0], false) == true)
					{
						//Bomb.insertBomb(i,false);
					}
				}
			}
		}

		////This detects collisions between the player and the bombs. 
		//for (int i = 0; i < 20; i++)
		//{
		//	collision = collisionDetect (Player.pdim, Bomb.bdim[i]);
		//	if (collision == true)
		//	{
		//		game = false;
		//	}
		//}
		bool pause = false;
		if (engine->key_Down("esc"))
		{
			//Pause the game.
			pause = true;
			arrowx = 250;
			int optselect = 3;
			bool keyFlag = false;
			switchPauseOptions (optselect);
			while (pause == true)
			{
				engine->handle_Input();

				if (engine->Quit() == true)
				{
					clean_Up ();
					Player.freeChars(2);
					exit (1);
				}

				apply_Surface (0, 0, pauseBack, screen, NULL);
				apply_Surface (arrowx, arrowy, arrow, screen, NULL);
				SDL_Flip (screen);
				//check for user input to switch the options in the main menu
				if (engine->key_Down("up") || engine->key_Down("down"))
				{
					if (keyFlag == false)
					{
						keyFlag = true;
						if (engine->key_Down("up"))
						{
							optselect++; if (optselect > 3) {optselect = 1;}
						}
						else
						{
							optselect--; if (optselect < 1) {optselect = 3;}
						}
						//switch the arrow's location after user input detection
						switchPauseOptions (optselect);
					}
				}
				else {keyFlag = false;}

				//check for user hitting enter to proceed
				if (engine->key_Down("enter"))
				{
					//continue
					if (optselect == 3)
					{
						pause = false;
					}
					if (optselect == 2)
					{
						//instructions page is under construction %^&*(*&^%$#@#$%^
					}
					if (optselect == 1)
					{
						clean_Up ();
						Player.freeChars(2);
						exit (1);
					}
				}
			}
		}
		if (engine->key_Down("Lshift"))
		{
			//Makes the player go the same speed as the bombs to allow better mobility
			Player.setSpeed (Bomb.speed, Bomb.speed);
		}
		else if (engine->key_Down("Lshift") == false)
		{
			Player.setSpeed (8, 8);
		}
		Player.w = engine->key_Down("w");
		Player.a = engine->key_Down("a");
		Player.s = engine->key_Down("s");
		Player.d = engine->key_Down("d");
		Player.up = engine->key_Down("up");
		Player.down = engine->key_Down("down");
		Player.left = engine->key_Down("left");
		Player.right = engine->key_Down("right");
		Player.beginDirectionCalc();
		int move = Player.endDirectionCalc();
		Player.move(move);
		//If the player isn't moving then moving = false which then extinguishes the flames of the jetpack
		//Now that more players have been added this just shuts off the player's move effect
		if (engine->key_Down("d") == false && engine->key_Down("w") == false && engine->key_Down("a") == false && engine->key_Down("s") == false)
		{
			Player.moving = false;
		}
		else
		{
			Player.moving = true;
		}
	}
	gameOver = IMG_load ("gameover_back.bmp");
	if (gameOver == NULL)
	{
		cout << "game.cpp gameOver failed to load.\n";
	}
	apply_Surface (0, 0, gameOver, screen, NULL);
	SDL_Flip (screen);
	bool quit = false;
	while (quit == false)
	{
		engine->handle_Input();
		if (engine->Quit() == true)
		{
			quit = true;
		}
		//check for user hitting enter to proceed
		if (engine->key_Down("enter"))
		{
			quit = true;
		}
	}
	//Cleans the game's surfaces and such so the game can close without leaking memory.
	clean_Up ();
	Player.freeChars(2);
	SDL_FreeSurface(gameOver);
	return 1;
}
void switchPauseOptions (int options)
{
	//Continue Game
	if (options == 3)
	{
		arrowy = 340;
	}
	//Instructions
	if (options == 2)
	{
		arrowy = 400;
	}
	//Quit Game
	if (options == 1)
	{
		arrowy = 460;
	}
}
void clean_Up ()
{
	//game stuff
	SDL_FreeSurface (background);
	//SDL_FreeSurface (player);
	SDL_FreeSurface (rawr);
	SDL_FreeSurface (bombs);
	SDL_FreeSurface (sBomb);
	SDL_FreeSurface (mBomb);
	SDL_FreeSurface (points);
	SDL_FreeSurface (bombSplosion);
	SDL_FreeSurface (pointSplosion);
	SDL_FreeSurface (pauseBack);
	//menu stuff
	SDL_FreeSurface (arrow);
}