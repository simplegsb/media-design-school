#include "stdafx.h"

#include <iostream>

#include "FirstComeFirstServedScheduler.h"

FirstComeFirstServedScheduler::FirstComeFirstServedScheduler()
{
}

void FirstComeFirstServedScheduler::schedule(std::vector<Process>& processes)
{
	unsigned int kernelTime = 0;
	unsigned int waitTime = 0;

	for (unsigned int index = 0; index < processes.size(); index++)
	{
		processes[index].remainingTime = 0;
		processes[index].waitTime = kernelTime;

		std::cout << "Process " << processes[index].id << " executed for " << processes[index].cpuBurst
			<< " (complete, waited " << processes[index].waitTime << ")" << std::endl;

		kernelTime += processes[index].cpuBurst;
		waitTime += processes[index].waitTime;
	}

	std::cout << "Total wait time: " << waitTime << std::endl;
	std::cout << "Average wait time: " << float(waitTime) / processes.size() << std::endl;
}
