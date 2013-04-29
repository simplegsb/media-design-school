#include "stdafx.h"

#include <iostream>
#include <string>

#include "FirstComeFirstServedScheduler.h"
#include "RoundRobinScheduler.h"
#include "ShortestFirstScheduler.h"
#include "ShortestTimeRemainingScheduler.h"

int _tmain(int argc, _TCHAR* argv[])
{
	char option = 'p';
	std::string processCpuBurst;
	std::string processId;
	std::string processStartTime;
	std::vector<Process> processes;

	do
	{
		std::cout << "Enter process details:" << std::endl;
		std::cout << "\tID: ";
		std::cin >> processId;
		std::cout << "\tCPU burst: ";
		std::cin >> processCpuBurst;
		std::cout << "\tStart time: ";
		std::cin >> processStartTime;

		Process process;
		process.cpuBurst = atoi(processCpuBurst.c_str());
		process.id = atoi(processId.c_str());
		process.remainingTime = process.cpuBurst;
		process.startTime = atoi(processStartTime.c_str());
		process.waitTime = 0;
		processes.push_back(process);

		std::cout << "What do you want to do? Add another (P)rocess, Select an (A)lgorithm: ";
		std::cin >> option;
	}
	while (option == 'p' || option == 'P');

	std::cout << "What scheduler do you want to use?" << std::endl
		<< "- (F)irst come first served" << std::endl
		<< "- Shortest (T)ime remaining first" << std::endl
		<< "- (S)hortest first" << std::endl
		<< "- (R)ound robin" << std::endl
		<< ": ";
	std::cin >> option;

	if (option == 'f' || option == 'F')
	{
		FirstComeFirstServedScheduler scheduler;
		scheduler.schedule(processes);
	}
	else if (option == 't' || option == 'T')
	{
		ShortestTimeRemainingScheduler scheduler;
		scheduler.schedule(processes);
	}
	else if (option == 's' || option == 'S')
	{
		ShortestFirstScheduler scheduler;
		scheduler.schedule(processes);
	}
	else if (option == 'r' || option == 'R')
	{
		RoundRobinScheduler scheduler;
		scheduler.schedule(processes);
	}

	std::string pause;
	std::cin >> pause;

	return 0;
}
