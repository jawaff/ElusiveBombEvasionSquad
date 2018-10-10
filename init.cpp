#include "init.h"

SDL_Surface *screen;
SDL_Surface *bombs;
SDL_Surface *sBomb;
SDL_Surface *mBomb;
SDL_Surface *points;
SDL_Surface *bombSplosion;
SDL_Surface *pointSplosion;
SDL_Surface *playerGreat;
SDL_Surface *playerGood;
SDL_Surface *playerOk;
SDL_Surface *playerMeh;
SDL_Surface *playerZzz;
SDL_Surface *playerDead;

SDL_Surface *IMG_load (std::string filename)
{
	//temporary storage for the image that's loaded
	SDL_Surface* loadedimage = NULL;

	//the optimized image that will be used
	SDL_Surface* optimizedimage = NULL;

	//load the image
	loadedimage = SDL_LoadBMP (filename.c_str());
	
	//if nothing went wrong in loading the image
	if (loadedimage != NULL)
	{
		//create an optimized image
		optimizedimage = SDL_DisplayFormat (loadedimage);

		//free the old image
		SDL_FreeSurface (loadedimage);
		
		if (optimizedimage != NULL)
		{
			//Map the color key
            Uint32 colorkey = SDL_MapRGB(optimizedimage->format, 200, 0, 200);

            //Set all pixels of color R 255, G 0, B 255(pink) to be transparent
            SDL_SetColorKey(optimizedimage, SDL_SRCCOLORKEY, colorkey);
		}
	}
	else 
	{
		return NULL;
	}

	//return the optimized image
	return optimizedimage;
}

void apply_Surface (int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL)
{
    //Make a temporary rectangle to hold the offsets
    SDL_Rect offset;

    //Give the offsets to the rectangle
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface (source, clip, destination, &offset);
}


int init_Game()
{
	//initialize sdl and all of its subsystems
	if (SDL_Init (SDL_INIT_EVERYTHING) == -1)
	{
		return 0;
	}

	atexit (SDL_Quit);

	//setup the screen
	screen = SDL_SetVideoMode (SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

	if (screen == NULL)
	{
		return 0;
	}

	//if (Mix_OpenAudio (22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	//{
	//	return 0;
	//}
	SDL_WM_SetCaption ("YESH!", NULL);
	return 1;
}