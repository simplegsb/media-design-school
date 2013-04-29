//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// Copyright (c) 2012 Media Design School
//
// SVN Info
// $Author: MEDIADESIGN\gary.buyn $
// $Date: 2012-09-24 08:28:44 +1200 (Mon, 24 Sep 2012) $
// $Rev: 68 $
//
#include "Task.h"

#include <windows.h> // Just to get NULL? Seriously?

TTask::TTask() :
	data(NULL),
	runnable(NULL)
{
}

TTask::TTask(const TTask& _rOriginal)
{
	operator=(_rOriginal);
}

void TTask::operator()()
{
	if (runnable != NULL)
	{
		runnable(data);
	}
}

TTask& TTask::operator=(const TTask& _rOriginal)
{
	data = _rOriginal.data;
	runnable = _rOriginal.runnable;

	return *this;
}
