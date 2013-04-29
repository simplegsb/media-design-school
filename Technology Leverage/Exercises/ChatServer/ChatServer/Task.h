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
// $Date: 2013-01-23 09:45:25 +1300 (Wed, 23 Jan 2013) $
// $Rev: 1110 $
//
#ifndef TASK_H_
#define TASK_H_

struct TTask
{
	TTask();

	TTask(const TTask& _rOriginal);

	/**
	 * A function that can be run.
	 */
	typedef void(Runnable)(void*);

	void* data;

	Runnable* runnable;

	void operator()();

	TTask& operator=(const TTask& _rOriginal);
};

#endif /* TASK_H_ */
