#pragma once

#include "HillClimber.h"

namespace hillclimbing
{
	class SimpleHillClimber : public HillClimber
	{
		public:
			virtual ~SimpleHillClimber();

		private:
			float getMaximumVariance();

			void onMove();

			bool useResult(float result, float maximum, float nextMaximum);
	};
}
