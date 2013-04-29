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
#ifndef THREADPOOLDELUXE_H_
#define THREADPOOLDELUXE_H_

#include <queue>
#include <vector>

#include "Timer.h"
#include "Thread.h"

class CThreadPoolDeluxe
{
	public:
		CThreadPoolDeluxe(UINT32 _uiThreadCount);

		~CThreadPoolDeluxe();

		bool GetNextTask(TTask& _task);

		float GetTimeTaken() const;

		void Join();

		void Join(UINT32 _uiMilliseconds);

		void QueueTask(const TTask& _krTask);

		bool RunNextTask();

		void Start();

		friend void executeQueuedTasks(void* _pData);

	private:
		CRITICAL_SECTION m_criticalSection;

		float m_fTimeTaken;

		std::queue<TTask> m_tasks;

		std::vector<CThread*> m_threads;

		Timer m_timer;

		UINT32 m_uiThreadCount;
};

#endif /* THREADPOOLDELUXE_H_ */
