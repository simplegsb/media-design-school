#include <stdlib.h>

#include "RandomFloat.h"

namespace hillclimbing
{
	float getRandomFloat(float max, float min)
	{
		return min + (float) rand() / ((float) RAND_MAX / (max - min));
	}
}
