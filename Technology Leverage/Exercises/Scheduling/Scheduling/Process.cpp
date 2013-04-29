#include "stdafx.h"

#include "Process.h"

bool CompareFinishTime::operator()(const Process* a, const Process* b) const
{
	return compareFinishTime(a, b);
}

bool CompareFinishTime::operator()(const Process& a, const Process& b) const
{
	return compareFinishTime(a, b);
}

bool CompareRemainingTime::operator()(const Process* a, const Process* b) const
{
	return compareRemainingTime(a, b);
}

bool CompareRemainingTime::operator()(const Process& a, const Process& b) const
{
	return compareRemainingTime(a, b);
}

bool compareFinishTime(const Process* a, const Process* b)
{
	return compareFinishTime(*a, *b);
}

bool compareFinishTime(const Process& a, const Process& b)
{
	return a.cpuBurst + a.startTime < b.cpuBurst + b.startTime;
}

bool compareRemainingTime(const Process* a, const Process* b)
{
	return compareRemainingTime(*a, *b);
}

bool compareRemainingTime(const Process& a, const Process& b)
{
	return a.remainingTime > b.remainingTime;
}
