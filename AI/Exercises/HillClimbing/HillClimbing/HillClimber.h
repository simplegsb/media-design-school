#pragma once

namespace hillclimbing
{
	typedef float(TwoDClimbable)(float x, float y);

	class HillClimber
	{
		public:
			virtual ~HillClimber();

			float climb(TwoDClimbable function);

		protected:
			virtual float getMaximumVariance() = 0;

			virtual void onMove() = 0;

			int updateRetries(bool higherPointFound, int retries);

			virtual bool useResult(float result, float maximum, float nextMaximum) = 0;
	};
}
