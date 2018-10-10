#include "SDL.h"
#include <string>

#define SCREEN_WIDTH 1084
#define SCREEN_HEIGHT 768
#define SCREEN_BPP 32
#define FRAMES_PER_SECOND 60

extern SDL_Surface *screen;
extern SDL_Surface *bombs;
extern SDL_Surface *sBomb;
extern SDL_Surface *mBomb;
extern SDL_Surface *points;
extern SDL_Surface *bombSplosion;
extern SDL_Surface *pointSplosion;
extern SDL_Surface *playerGreat;
extern SDL_Surface *playerGood;
extern SDL_Surface *playerOk;
extern SDL_Surface *playerMeh;
extern SDL_Surface *playerZzz;
extern SDL_Surface *playerDead;
SDL_Surface *IMG_load (std::string filename);

void apply_Surface (int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);

int init_Game ();