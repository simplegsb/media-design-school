#include "stdafx.h"

#include <iostream>
#include <list>
#include <string>

#include "RoundRobinScheduler.h"

RoundRobinScheduler::RoundRobinScheduler()
{
}

void RoundRobinScheduler::schedule(std::vector<Process>& processes)
{
	std::list<Process*> activeProcesses;
	for (unsigned int index = 0; index < processes.size(); index++)
	{
		activeProcesses.push_back(&processes[index]);
	}

	std::cout << "Enter the round robin time slice: ";
	std::string timeSliceString;
	std::cin >> timeSliceString;
	unsigned int timeSlice = atoi(timeSliceString.c_str());

	Process* currentProcess = NULL;
	unsigned int kernelTime = 0;
	unsigned int waitTime = 0;

	while (!activeProcesses.empty())
	{
		for (std::list<Process*>::iterator iter = activeProcesses.begin();
			iter != activeProcesses.end(); iter++)
		{
			currentProcess = *iter;

			if (timeSlice >= currentProcess->remainingTime)
			{
				unsigned int previouslyRemainingTime = currentProcess->remainingTime;
				currentProcess->remainingTime = 0;
				currentProcess->waitTime = kernelTime + previouslyRemainingTime;

				std::cout << "Process " << currentProcess->id << " executed for " << previouslyRemainingTime
					<< " (complete, waited " << currentProcess->waitTime << ")" << std::endl;

				iter = activeProcesses.erase(iter);
				if (!activeProcesses.empty())
				{
					// Erase returns the element after the one erased, which would then be skipped by the for loop so
					// decrement.
					iter--;
				}

				kernelTime += previouslyRemainingTime;
				// For round robin, the wait time is up until the completion of the process.
				waitTime += kernelTime;

				// Dirty hack to stop the for loop from breaking when attempting to increment the iterator.
				if (activeProcesses.empty())
				{
					break;
				}
			}
			else
			{
				currentProcess->remainingTime -= timeSlice;

				std::cout << "Process " << currentProcess->id << " executed for " << timeSlice << " (incomplete)"
					<< std::endl;

				kernelTime += timeSlice;
			}
		}
	}

	std::cout << "Total wait time: " << waitTime << std::endl;
	std::cout << "Average wait time: " << float(waitTime) / processes.size() << std::endl;
}
