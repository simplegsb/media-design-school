#pragma once

#include <vector>

#include "Process.h"

class Scheduler
{
	public:
		virtual void schedule(std::vector<Process>& processes) = 0;
};
