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
#include "GDE/Core/Logger.h"

#include "CriticalScope.h"
#include "ThreadPoolDeluxe.h"

void executeQueuedTasks(void* _pData)
{
	CThread* pThread = CThread::GetCurrentThread();

	CThreadPoolDeluxe* pThreadPool = static_cast<CThreadPoolDeluxe*>(_pData);
	while (pThreadPool->RunNextTask())
	{
		GDE::CLogger::GetDebugPrintLog().PrintF("A task was run on thread %i.\n", pThread->GetId());
	}
}

CThreadPoolDeluxe::CThreadPoolDeluxe(UINT32 _uiThreadCount) :
	m_criticalSection(),
	m_fTimeTaken(0.0f),
	m_tasks(),
	m_threads(),
	m_timer(),
	m_uiThreadCount(_uiThreadCount)
{
	InitializeCriticalSection(&m_criticalSection);
	m_threads.reserve(m_uiThreadCount);
}

CThreadPoolDeluxe::~CThreadPoolDeluxe()
{
	DeleteCriticalSection(&m_criticalSection);

	for (UINT32 uiIndex = 0; uiIndex < m_threads.size(); uiIndex++)
	{
		delete m_threads[uiIndex];
	}
}

bool CThreadPoolDeluxe::GetNextTask(TTask& _task)
{
	CCriticalScope criticalScope(m_criticalSection);

	if (m_tasks.empty())
	{
		return false;
	}

	_task = m_tasks.front();
	m_tasks.pop();

	return true;
}

float CThreadPoolDeluxe::GetTimeTaken() const
{
	return m_fTimeTaken;
}

void CThreadPoolDeluxe::Join()
{
	Join(INFINITE);
}

void CThreadPoolDeluxe::Join(UINT32 _uiMilliseconds)
{
	for (UINT32 uiIndex = 0; uiIndex < m_threads.size(); uiIndex++)
	{
		m_threads[uiIndex]->Join(_uiMilliseconds);
	}
}

void CThreadPoolDeluxe::QueueTask(const TTask& _krTask)
{
	m_tasks.push(_krTask);
}

bool CThreadPoolDeluxe::RunNextTask()
{
	TTask task;
	if (!GetNextTask(task))
	{
		return false;
	}

	task();

	m_timer.tick();
	m_fTimeTaken = m_timer.getGameTime();

	return true;
}

void CThreadPoolDeluxe::Start()
{
	GDE::CLogger::GetDebugPrintLog().PrintF("Starting thread pool.\n");

	m_timer.reset();

	TTask poolTask;
	poolTask.data = this;
	poolTask.runnable = &executeQueuedTasks;

	for (UINT32 uiIndex = 0; uiIndex < m_uiThreadCount; uiIndex++)
	{
		CThread* pThread = new CThread(poolTask);
		m_threads.push_back(pThread);
		pThread->Start();
	}
}
