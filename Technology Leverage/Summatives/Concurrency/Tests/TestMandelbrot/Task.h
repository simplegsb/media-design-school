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
#ifndef TASK_H_
#define TASK_H_

struct TTask
{
	/**
	 * A function that can be run.
	 */
	typedef void(Runnable)(void*);

	TTask();

	TTask(const TTask& _rOriginal);

	void* data;

	Runnable* runnable;

	void operator()();

	TTask& operator=(const TTask& _rOriginal);
};

#endif /* TASK_H_ */
