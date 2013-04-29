#pragma once

#include "Scheduler.h"

class ShortestFirstScheduler : public Scheduler
{
	public:
		ShortestFirstScheduler();

		void schedule(std::vector<Process>& processes);
};
