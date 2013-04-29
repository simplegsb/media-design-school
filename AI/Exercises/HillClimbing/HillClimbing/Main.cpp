#include <cmath>
#include <iostream>
#include <string>
#include <time.h>

#include "SimpleHillClimber.h"
#include "SimulatedAnnealingHillClimber.h"
#include "StochasticHillClimber.h"

using namespace hillclimbing;
using namespace std;

float dualPeaks(float x, float y)
{
	return exp(-1 * (pow(x, 2) + pow(y, 2))) + 2.0f * exp(-1 * (pow(x - 1.7f, 2) + pow(y - 1.7f, 2)));
}

float coneAroundOrigin(float x, float y)
{
	return 10.0f - sqrt(pow(x, 2) + pow(y, 2));
}

int main(int argc, char** argv)
{
	srand((unsigned) time(0));

	// Choose a hill climber imlpementation:
	//SimpleHillClimber hillClimber;
	SimulatedAnnealingHillClimber hillClimber;
	//StochasticHillClimber hillClimber;
	
	// Choose a hill:
	//cout << "Climb complete, climbed to: " << hillClimber.climb(coneAroundOrigin);
	cout << "Climb complete, climbed to: " << hillClimber.climb(dualPeaks);

	string endClimb;
	cin >> endClimb;

	return 0;
}
