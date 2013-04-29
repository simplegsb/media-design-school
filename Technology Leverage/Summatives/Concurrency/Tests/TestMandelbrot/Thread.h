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
#ifndef THREAD_H_
#define THREAD_H_

#include "windows.h"

#include "Task.h"

class CThread
{
	public:
		/**
		 * A function that can be run by a thread.
		 *
		 * @param function The task that will run.
		 */
		CThread(const TTask& _krTask);

		~CThread();

		/**
		 * Retrieves the currently executing thread.
		 */
		static CThread* GetCurrentThread();

		/**
		 * Retrieves the unique ID for this thread.
		 */
		UINT32 GetId();

		/**
		 * Notify the thread that it should stop as soon as possible.
		 */
		void Interrupt();

		/**
		 * Determines if the thread has been notified that it should stop as soon as possible.
		 */
		bool IsInterrupted();

		/**
		 * Determines if the thread is currently running.
		 */
		bool IsRunning();

		/**
		 * Wait forever for the thread to finish.
		 */
		bool Join();

		/**
		 * Wait for the given amount of time for the thread to finish.
		 *
		 * @param _uiMilliseconds The time to wait for the thread to finish.
		 */
		bool Join(unsigned int _uiMilliseconds);

		/**
		 * Starts the thread.
		 */
		bool Start();

	private:
		bool m_bInterrupted;

		TTask m_task;

		HANDLE m_threadHandle;

		UINT32 m_uiId;
};

#endif /* THREAD_H_ */
