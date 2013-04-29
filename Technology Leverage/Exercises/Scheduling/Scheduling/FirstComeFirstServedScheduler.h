#pragma once

#include "Scheduler.h"

class FirstComeFirstServedScheduler : public Scheduler
{
	public:
		FirstComeFirstServedScheduler();

		void schedule(std::vector<Process>& processes);
};
