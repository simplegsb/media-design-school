#include "process.h"

#include "Thread.h"

Thread::Thread(Runnable function, void* data) :
	id(0),
	interrupted(false),
	threadHandle(NULL)
{
	threadHandle = (HANDLE) _beginthreadex(NULL, 0, function, data, CREATE_SUSPENDED, &id);
}

Thread::~Thread()
{
	if (threadHandle == 0)
	{
		return;
	}

	CloseHandle(threadHandle);
}

void Thread::interrupt()
{
	interrupted = true;
}

bool Thread::isInterrupted()
{
	return interrupted;
}

bool Thread::isRunning()
{
	if (threadHandle == 0)
	{
		return false;
	}

	DWORD exitCode;
	GetExitCodeThread(threadHandle, &exitCode);

	return exitCode == STILL_ACTIVE;
}

bool Thread::join()
{
	return join(INFINITE);
}

bool Thread::join(unsigned int milliseconds)
{
	if (threadHandle == 0)
	{
		return false;
	}

	DWORD result = WaitForSingleObject(threadHandle, milliseconds);

	return result == WAIT_OBJECT_0;
}

bool Thread::start()
{
	if (threadHandle == 0)
	{
		return false;
	}

	return ResumeThread(threadHandle) != -1;
}
