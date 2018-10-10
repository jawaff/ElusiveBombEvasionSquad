#include "score.h"
#include "init.h"

ScoreKeeper::ScoreKeeper ()
{
	numberInterval = 45;
	numberRect.x = 9 + (numberInterval * number);
	numberRect.y = 15;
	numberRect.w = 30;
	numberRect.h = 32;
	fontMap = IMG_load("8bit_font.bmp");
	//for each 10-digits of this score array we will
	//set it to zero. The first element corresponds with the first digit of the actual number. 
	for (int i = 0; i < 10; i++)
	{
		//reseting.
		score[i] = 0;
	}
}
void ScoreKeeper::updateRect ()
{
	numberRect.x = 9 + (numberInterval * number);
}
void ScoreKeeper::applyScore ()
{
	for (int i = 0; i < 10; i++)
	{
		number = score[i];
		updateRect();
		apply_Surface(1030 - (numberInterval * i), 20, fontMap, screen, &numberRect);
	}
}
void ScoreKeeper::incrementScore (int howMuch)
{
	int i = 2;
	if ((score[i] + howMuch) >= 10)
	{		
		score[i] += howMuch - 10;
		score[i+1] += 1;
		for (int j = i + 1; j < 10; j++)
		{
			if ((score[j]) >= 10)
			{
				score[j] = 0;
				score[j+1] += 1;
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		score[i] += howMuch;
	}
}

void ScoreKeeper::decrementScore (int howMuch)
{
	int i = 2;
	if ((score[i] - howMuch) < 0)
	{
		if ((score[i+1] == 0) && (score[i+2] == 0) && (score[i+3] == 0) && (score[i+4] == 0) && (score[i+5] == 0) && (score[i+6] == 0) && (score[i+7] == 0))
		{
			score[i] = 0;
		}
		else
		{
			score[i] = (score[i] - howMuch) + 10;
			score[i+1] -= 1;
		
			for (int j = i + 1; j < 10; j++)
			{
				if ((score[j]) + 10 < 10)
				{
					score[j] = 9;
					score[j+1] -= 1;
				}
				else
				{
					break;
				}
			}
		}
	}
	else
	{
		score[i] -= howMuch;
	}
}
