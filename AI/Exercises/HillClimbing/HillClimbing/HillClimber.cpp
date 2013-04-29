#include <iostream>

#include "HillClimber.h"
#include "RandomFloat.h"

using namespace std;

namespace hillclimbing
{
	// The amount of retries made if a 'higher' point is not found. More retries tends to be more accurate.
	int MAX_RETRIES = 10;

	HillClimber::~HillClimber()
	{
	}
	
	float HillClimber::climb(TwoDClimbable function)
	{
		float maximum = -999.0f;
		float nextMaximum = -999.0f;
		int retries = 0;
		float x = getRandomFloat(-10.0f, 10.0f);
		float y = getRandomFloat(-10.0f, 10.0f);

		do
		{
			maximum = nextMaximum;

			float nextX = x;
			float nextY = y;
			float result = 0;
			float distance = 0;

			distance = getRandomFloat(0.0f, getMaximumVariance());
			result = function(x - distance, y);
			if (useResult(result, maximum, nextMaximum))
			{
				nextMaximum = result;
				nextX = x - distance;
				nextY = y;
			}
			
			distance = getRandomFloat(0.0f, getMaximumVariance());
			result = function(x + distance, y);
			if (useResult(result, maximum, nextMaximum))
			{
				nextMaximum = result;
				nextX = x + distance;
				nextY = y;
			}
			
			distance = getRandomFloat(0.0f, getMaximumVariance());
			result = function(x, y - distance);
			if (useResult(result, maximum, nextMaximum))
			{
				nextMaximum = result;
				nextX = x;
				nextY = y - distance;
			}
			
			distance = getRandomFloat(0.0f, getMaximumVariance());
			result = function(x, y + distance);
			if (useResult(result, maximum, nextMaximum))
			{
				nextMaximum = result;
				nextX = x;
				nextY = y + distance;
			}
			
			x = nextX;
			y = nextY;
			
			onMove();
			cout << "Climbed to: " << nextMaximum << " at [" << x << ", " << y << "]" << endl;
			
			retries = updateRetries(nextMaximum > maximum, retries);
		}
		while (retries < MAX_RETRIES || nextMaximum > maximum);

		return maximum;
	}

	int HillClimber::updateRetries(bool higherPointFound, int retries)
	{
		if (higherPointFound)
		{
			return 0;
		}

		return retries + 1;
	}
}