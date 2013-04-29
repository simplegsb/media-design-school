#pragma once

#include "HillClimber.h"

namespace hillclimbing
{
	class StochasticHillClimber : public HillClimber
	{
		public:
			virtual ~StochasticHillClimber();

		private:
			float getMaximumVariance();

			void onMove();

			bool useResult(float result, float maximum, float nextMaximum);
	};
}
