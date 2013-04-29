#include "stdafx.h"

#include <iostream>
#include <queue>

#include "ShortestFirstScheduler.h"

ShortestFirstScheduler::ShortestFirstScheduler()
{
}

void ShortestFirstScheduler::schedule(std::vector<Process>& processes)
{
	std::priority_queue<Process*, std::vector<Process*>, CompareRemainingTime> queuedProcesses;
	for (unsigned int index = 0; index < processes.size(); index++)
	{
		queuedProcesses.push(&processes[index]);
	}

	unsigned int kernelTime = 0;
	unsigned int waitTime = 0;

	while (!queuedProcesses.empty())
	{
		queuedProcesses.top()->remainingTime = 0;
		queuedProcesses.top()->waitTime = kernelTime;

		std::cout << "Process " << queuedProcesses.top()->id << " executed for " << queuedProcesses.top()->cpuBurst
			<< " (complete, waited " << queuedProcesses.top()->waitTime << ")" << std::endl;

		kernelTime += queuedProcesses.top()->cpuBurst;
		waitTime += queuedProcesses.top()->waitTime;

		queuedProcesses.pop();
	}

	std::cout << "Total wait time: " << waitTime << std::endl;
	std::cout << "Average wait time: " << float(waitTime) / processes.size() << std::endl;
}
