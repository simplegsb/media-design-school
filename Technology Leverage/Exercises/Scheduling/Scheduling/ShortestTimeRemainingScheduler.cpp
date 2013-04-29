#include "stdafx.h"

#include <iostream>
#include <queue>
#include <set>

#include "ShortestTimeRemainingScheduler.h"

ShortestTimeRemainingScheduler::ShortestTimeRemainingScheduler()
{
}

void ShortestTimeRemainingScheduler::schedule(std::vector<Process>& processes)
{
	std::priority_queue<Process*, std::vector<Process*>, CompareRemainingTime> queuedProcesses;
	std::priority_queue<Process*, std::vector<Process*>, CompareFinishTime> unstartedProcesses;
	for (unsigned int index = 0; index < processes.size(); index++)
	{
		if (processes[index].startTime == 0)
		{
			queuedProcesses.push(&processes[index]);
		}
		else
		{
			unstartedProcesses.push(&processes[index]);
		}
	}

	Process* currentProcess = NULL;
	unsigned int kernelTime = 0;
	unsigned int waitTime = 0;

	while (!queuedProcesses.empty() || !unstartedProcesses.empty())
	{
		if (queuedProcesses.empty())
		{
			currentProcess = unstartedProcesses.top();
			unstartedProcesses.pop();
			kernelTime += currentProcess->startTime - kernelTime;
		}
		else
		{
			currentProcess = queuedProcesses.top();
			queuedProcesses.pop();
		}

		if (!unstartedProcesses.empty())
		{
			unsigned int nextProcessFinishTime = unstartedProcesses.top()->startTime + unstartedProcesses.top()->cpuBurst;
			unsigned int currentProcessFinishTime = kernelTime + currentProcess->remainingTime;
			if (nextProcessFinishTime < currentProcessFinishTime)
			{
				unsigned int timeToNextProcessStart = unstartedProcesses.top()->startTime - kernelTime;
				currentProcess->remainingTime -= timeToNextProcessStart;

				std::cout << "Process " << currentProcess->id << " executed for " << timeToNextProcessStart
					<< " (incomplete)" << std::endl;

				queuedProcesses.push(currentProcess);
				queuedProcesses.push(unstartedProcesses.top());
				unstartedProcesses.pop();
				kernelTime += timeToNextProcessStart;

				continue;
			}
		}

		unsigned int previouslyRemainingTime = currentProcess->remainingTime;
		currentProcess->remainingTime = 0;
		currentProcess->waitTime = kernelTime + previouslyRemainingTime - currentProcess->cpuBurst;

		std::cout << "Process " << currentProcess->id << " executed for " << previouslyRemainingTime
			<< " (complete, waited " << currentProcess->waitTime << ")" << std::endl;

		kernelTime += previouslyRemainingTime;
		waitTime += currentProcess->waitTime;
	}

	std::cout << "Total wait time: " << waitTime << std::endl;
	std::cout << "Average wait time: " << float(waitTime) / processes.size() << std::endl;
}
