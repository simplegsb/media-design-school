#pragma once

#include <list>

#include <windows.h>

#include "Observer.h"

class CClockTimer
{
	public:
		CClockTimer();

		void Attach(CObserver* observer);

		void Detach(CObserver* observer);

		void Tick();

	private:
		std::list<CObserver*> m_observers;

		DWORD m_time;

		void Notify();
};
