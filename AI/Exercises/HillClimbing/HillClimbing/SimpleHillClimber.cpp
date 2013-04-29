#include "SimpleHillClimber.h"

namespace hillclimbing
{
	SimpleHillClimber::~SimpleHillClimber()
	{
	}

	float SimpleHillClimber::getMaximumVariance()
	{
		return 1.0f;
	}

	void SimpleHillClimber::onMove()
	{
	}

	bool SimpleHillClimber::useResult(float result, float maximum, float nextMaximum)
	{
		return result > nextMaximum;
	}
}
