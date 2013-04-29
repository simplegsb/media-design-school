#pragma once

#include "windows.h"

class Thread
{
	public:
		/**
		 * A function that can be run by a thread.
		 */
		typedef unsigned(__stdcall Runnable)(void*);

		/**
		 * A function that can be run by a thread.
		 *
		 * @param function The function the thread will run.
		 * @param data The data that will be passed to the thread.
		 */
		Thread(Runnable function, void* data);

		~Thread();

		/**
		 * Notify the thread that it should stop as soon as possible.
		 */
		void interrupt();

		/**
		 * Determines if the thread has been notified that it should stop as soon as possible.
		 */
		bool isInterrupted();

		/**
		 * Determines if the thread is currently running.
		 */
		bool isRunning();

		/**
		 * Wait forever for the thread to finish.
		 */
		bool join();

		/**
		 * Wait for the given amount of time for the thread to finish.
		 *
		 * @param milliseconds The time to wait for the thread to finish.
		 */
		bool join(unsigned int milliseconds);

		/**
		 * Starts the thread.
		 */
		bool start();

	private:
		bool interrupted;

		unsigned int id;

		HANDLE threadHandle;
};
