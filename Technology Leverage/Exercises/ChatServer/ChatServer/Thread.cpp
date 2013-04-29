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
#include <map>

#include <process.h>

#include "Thread.h"

std::map<DWORD, CThread*> g_threads;

unsigned __stdcall runOnThread(void* _pData)
{
	TTask* task = static_cast<TTask*>(_pData);
	(*task)();

	return 0;
}

CThread::CThread(const TTask& _krTask) :
	m_bInterrupted(false),
	m_uiId(0),
	m_task(_krTask),
	m_threadHandle(NULL)
{
	m_threadHandle = (HANDLE) _beginthreadex(NULL, 0, runOnThread, &m_task, CREATE_SUSPENDED, &m_uiId);
	g_threads[m_uiId] = this;
}

CThread::~CThread()
{
	if (m_threadHandle == 0)
	{
		return;
	}

	CloseHandle(m_threadHandle);
	g_threads.erase(m_uiId);
}

CThread* CThread::GetCurrentThread()
{
	DWORD threadId = GetCurrentThreadId();
	if (g_threads.find(threadId) != g_threads.end())
	{
		return g_threads[threadId];
	}

	return NULL;
}

UINT32 CThread::GetId()
{
	return m_uiId;
}

void CThread::Interrupt()
{
	m_bInterrupted = true;
}

bool CThread::IsInterrupted()
{
	return m_bInterrupted;
}

bool CThread::IsRunning()
{
	if (m_threadHandle == 0)
	{
		return false;
	}

	DWORD exitCode;
	GetExitCodeThread(m_threadHandle, &exitCode);

	return exitCode == STILL_ACTIVE;
}

bool CThread::Join()
{
	return Join(INFINITE);
}

bool CThread::Join(unsigned int _uiMilliseconds)
{
	if (m_threadHandle == 0)
	{
		return false;
	}

	DWORD result = WaitForSingleObject(m_threadHandle, _uiMilliseconds);

	return result == WAIT_OBJECT_0;
}

bool CThread::Start()
{
	if (m_threadHandle == 0)
	{
		return false;
	}

	return ResumeThread(m_threadHandle) != -1;
}
