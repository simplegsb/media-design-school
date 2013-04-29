#pragma once

#include "Scheduler.h"

class ShortestTimeRemainingScheduler : public Scheduler
{
	public:
		ShortestTimeRemainingScheduler();

		void schedule(std::vector<Process>& processes);
};
