#pragma once

#include "Scheduler.h"

class RoundRobinScheduler : public Scheduler
{
	public:
		RoundRobinScheduler();

		void schedule(std::vector<Process>& processes);
};
