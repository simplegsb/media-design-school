#pragma once

#include "Observer.h"

class CDigitalClock : public CObserver
{
	public:
		CDigitalClock();

		void Draw(HDC buffer) const;

		void Update(DWORD _time);

	private:
		long m_time;
};
