//#include "menu.h"
//#include "SDL.h"
//#include <iostream>
//#include "init.h"
//#include "InputEngine.h"
//
//using namespace std;
//
//const int arrowx = 300;
//int arrowy = 300;
//
//SDL_Surface *intro;
//SDL_Surface *menu;
//SDL_Surface *arrow;
//
//GameEngine *mengine = NULL;
//
//int showMenu ()
//{
//	/////////////////////////////////////////////////////
//    mload_Images ();
//	if (mload_Images () == false)
//	{	
//		cout << "mload_image () failed to initialize\n";
//	}
//	/////////////////////////////////////////////////////
//
//	mengine = GameEngine::instance ();
//	if (mengine == NULL)
//	{
//		cout << "mengine failed to initialize\n";
//	}
//
//	//set up the different variables and functions that I will need for the menu
//	bool quit_Menu = false;	
//	int menustate = 0;
//	int optselect = 3;
//	bool keyFlag = false;
//	switchOptions (optselect);
//
//	//main input loop for the menu
//	while (quit_Menu == false)
//	{
//		//{SDL_Event event; while (SDL_PollEvent(&event));}
//		//SDL_PumpEvents ();
//
//		mengine->handle_Input ();
//		//check if x on window is clicked
//		if (mengine->Quit () == true)
//		{
//			mclean_Up ();
//			return 0;
//		}
//		//intro to menu
//		if (menustate == 0)
//		{
//			apply_Surface (0, 0, intro, screen, NULL);
//			SDL_Flip (screen);
//
//			if (mengine->key_Down("y"))
//			{
//				menustate = 1;
//			}
//			if (mengine->key_Down("n"))
//			{
//				mclean_Up ();
//				return 0;
//			}
//		}
//
//		//main menu
//		if (menustate == 1)
//		{
//			apply_Surface (0, 0, menu, screen, NULL);
//			apply_Surface (arrowx, arrowy, arrow, screen, NULL);
//			SDL_Flip (screen);
//
//			//check for user input to switch the options in the main menu
//			if (mengine->key_Down("w") || mengine->key_Down("s"))
//			{
//				if (keyFlag == false)
//				{
//					keyFlag = true;
//					if (mengine->key_Down("w"))
//					{
//						optselect++; if (optselect > 3) {optselect = 1;}
//					}
//					else
//					{
//						optselect--; if (optselect < 1) {optselect = 3;}
//					}
//					//switch the arrow's location after user input detection
//					switchOptions (optselect);
//				}
//			}
//			else {keyFlag = false;}
//			SDL_Flip (screen);
//
//			//check for user hitting enter to proceed into the game further
//			if (mengine->key_Down("enter"))
//			{
//				if (optselect == 3)
//				{
//					mclean_Up ();
//					return 1;
//				}
//				if (optselect == 2)
//				{
//					//instructions page is under construction %^&*(*&^%$#@#$%^
//				}
//				if (optselect == 1)
//				{
//					mclean_Up ();
//					return 0;
//				}
//			}
//		}
//		//instructions part of the menu
//		//if (menustate == 3)
//		//{
//
//		//}
//	}//end while loop for the menu input
//	return 1;
//}
//
//void switchOptions (int options)
//{
//	//Start Game
//	if (options == 3)
//	{
//		arrowy = 300;
//	}
//	//instructions
//	if (options == 2)
//	{
//		arrowy = 370;
//	}
//	//Quit Game
//	if (options == 1)
//	{
//		arrowy = 440;
//	}
//}
//
//bool mload_Images ()
//{	
//	intro = IMG_load ("intro_back.bmp");
//	if (intro == NULL)
//	{
//		cout << "error while loading intro page/n";
//		return false;
//	}
//
//	menu = IMG_load ("menu_back.bmp");
//	if (menu == NULL)
//	{
//		cout << "error while loading menu page/n";
//		return false;
//	}
//
//	arrow = IMG_load ("arrow.bmp");
//	if (arrow == NULL)
//	{
//		cout << "error while loading arrow/n";
//		return false;
//	}
//	return true;
//}
//
//void mclean_Up ()
//{
//	mengine->~GameEngine ();
//	SDL_FreeSurface (intro);
//	SDL_FreeSurface (menu);
//	SDL_FreeSurface (arrow);
//}