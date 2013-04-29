#include "RandomFloat.h"
#include "StochasticHillClimber.h"

namespace hillclimbing
{
	StochasticHillClimber::~StochasticHillClimber()
	{
	}

	float StochasticHillClimber::getMaximumVariance()
	{
		return 1.0f;
	}

	void StochasticHillClimber::onMove()
	{
	}

	bool StochasticHillClimber::useResult(float result, float maximum, float nextMaximum)
	{
		if (result > maximum)
		{
			if (nextMaximum == maximum)
			{
				return true;
			}

			return getRandomFloat(0.0f, 1.0f) >= 0.5f;
		}

		return false;
	}
}
