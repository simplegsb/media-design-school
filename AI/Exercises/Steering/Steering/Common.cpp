#include <cstdlib>
#include <time.h>

#include "Common.h"

namespace Common
{
	bool randomIsSeeded = false;

	float getRandomFloat(float min, float max)
	{
		if (!randomIsSeeded)
		{
			srand((unsigned) time(NULL));
			randomIsSeeded = true;
		}

		return min + (float) rand() / ((float) RAND_MAX / (max - min));
	}

	int getRandomInt(int min, int max)
	{
		if (!randomIsSeeded)
		{
			srand((unsigned) time(NULL));
			randomIsSeeded = true;
		}

		return rand() % (max - min + 1) + min;
	}
}
