#include "init.h"

//menu functions
int showMenu ();
void switchMenuOptions (int option);
bool mload_Images ();
void mclean_Up ();

//game functions
void switchChar (int character);
bool collisionDetect (SDL_Rect A, SDL_Rect B);
int distanceFormula (SDL_Rect A, SDL_Rect B);
int playGame ();
void switchPauseOptions (int option);
void clean_Up ();
