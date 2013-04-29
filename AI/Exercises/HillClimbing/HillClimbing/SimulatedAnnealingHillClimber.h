#pragma once

#include "HillClimber.h"

namespace hillclimbing
{
	class SimulatedAnnealingHillClimber : public HillClimber
	{
		public:
			SimulatedAnnealingHillClimber();

			virtual ~SimulatedAnnealingHillClimber();

		private:
			float maximumVariance;

			float getMaximumVariance();

			void onMove();

			bool useResult(float result, float maximum, float nextMaximum);
	};
}
