
#include <string>

#include <windows.h>

#include "DigitalClock.h"

CDigitalClock::CDigitalClock() :
	m_time(0L)
{
}

void CDigitalClock::Draw(HDC buffer) const
{
	long timeInSeconds = m_time / 1000L;
	long seconds = m_time % 60L;
	long timeInMinutes = timeInSeconds / 60L;
	long minutes = timeInMinutes % 60L;
	long timeInHours = timeInMinutes / 60L;
	long hours = timeInHours % 24L;

	std::wstring text = L"Hello, World!";
	RECT rectangle;
	rectangle.left = 0L;
	rectangle.top = 0L;
	rectangle.right = 100L;
	rectangle.bottom = 100L;
	DrawText(buffer, text.c_str(), text.size(), &rectangle, DT_TOP | DT_LEFT);
}

void CDigitalClock::Update(DWORD _time)
{
	m_time = _time;
}
