
#include <gazengine/Events.h>
#include <gazengine/gdi/model/GDICircle.h>
#include <gazengine/gdi/model/GDIMesh.h>
#include <gazengine/gdi/rendering/GDIRenderingEngine.h>
#include <gazengine/input/MouseButtonEvent.h>
#include <gazengine/Messages.h>
#include <gazengine/Timer.h>
#include <gazengine/winapi/WinApiEngine.h>

#include "ClockTimer.h"
#include "DigitalClock.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow)
{
    WinApiEngine windowEngine(L"Observer Pattern (Clocks)", instance, commandShow);
    windowEngine.init();

	CClockTimer timer;

	CDigitalClock* pDigitalClock = new CDigitalClock;
	timer.Attach(pDigitalClock);

	/*CAnalogueClock* pAnalogueClock = new CAnalogueClock
	timer.Attach(pAnalogueClock);
	renderingEngine.addModel(pAnalogueClock);*/

	// Setup GDI
	RECT windowRect;
	GetClientRect(windowEngine.getWindow(), &windowRect);

	HDC frontBuffer = GetDC(windowEngine.getWindow());
	HDC backBuffer = CreateCompatibleDC(frontBuffer);
	HBITMAP backBitmap = CreateCompatibleBitmap(frontBuffer, windowRect.right, windowRect.bottom);
	HBITMAP backBitmapOld = (HBITMAP) SelectObject(backBuffer, backBitmap);

    while(windowEngine.getMessage().message != WM_QUIT)
    {
		timer.Tick();

		BitBlt(backBuffer, 0, 0, windowRect.right, windowRect.bottom, NULL, 0, 0, WHITENESS);

		pDigitalClock->Draw(backBuffer);
		//pAnalogueClock->Draw(backBuffer);

		BitBlt(frontBuffer, 0, 0, windowRect.right, windowRect.bottom, backBuffer, 0, 0, SRCCOPY);
		ValidateRect(windowEngine.getWindow(), &windowRect);

		windowEngine.advance();
    }

	// Release GDI
	SelectObject(backBuffer, backBitmapOld);
	DeleteObject(backBitmap);
	ReleaseDC(windowEngine.getWindow(), backBuffer);
	ReleaseDC(windowEngine.getWindow(), frontBuffer);

    windowEngine.destroy();

    return windowEngine.getMessage().wParam;
}
