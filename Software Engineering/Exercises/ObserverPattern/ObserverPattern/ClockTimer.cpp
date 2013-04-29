
#include <algorithm>

#include <windows.h>
#include <mmsystem.h>

#include "ClockTimer.h"

CClockTimer::CClockTimer() :
	m_observers(),
	m_time(0)
{
}

void CClockTimer::Attach(CObserver* observer)
{
	m_observers.push_back(observer);
}

void CClockTimer::Detach(CObserver* observer)
{
	m_observers.erase(find(m_observers.begin(), m_observers.end(), observer));
}

void CClockTimer::Notify()
{
	for (std::list<CObserver*>::iterator iter = m_observers.begin();
		iter != m_observers.end();
		iter++)
	{
		(*iter)->Update(0.0f);
	}
}

void CClockTimer::Tick()
{
	m_time = timeGetTime();
}
