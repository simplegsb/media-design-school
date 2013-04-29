#include <iostream>

#include "RandomFloat.h"
#include "SimulatedAnnealingHillClimber.h"

using namespace std;

namespace hillclimbing
{
	SimulatedAnnealingHillClimber::SimulatedAnnealingHillClimber() :
			maximumVariance(10.0f)
	{
	}

	SimulatedAnnealingHillClimber::~SimulatedAnnealingHillClimber()
	{
	}

	float SimulatedAnnealingHillClimber::getMaximumVariance()
	{
		return maximumVariance;
	}

	void SimulatedAnnealingHillClimber::onMove()
	{
		maximumVariance *= 0.9f;
		cout << "Maximum variance: " << maximumVariance << endl;
	}

	bool SimulatedAnnealingHillClimber::useResult(float result, float maximum, float nextMaximum)
	{
		return result > nextMaximum;
	}
}
