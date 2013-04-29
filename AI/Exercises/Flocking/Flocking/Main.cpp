#include <sstream>

#include <windows.h>
#include <commctrl.h>

#include <gazengine/gdi/model/GDICircle.h>
#include <gazengine/gdi/rendering/GDIRenderingEngine.h>
#include <gazengine/Timer.h>
#include <gazengine/winapi/WinApiEngine.h>

#include "FlockingAgent.h"

using namespace std;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow)
{
    WinApiEngine* windowEngine = new WinApiEngine(L"Flocking", instance, commandShow);
    windowEngine->init();

	GDIRenderingEngine* renderingEngine = new GDIRenderingEngine(windowEngine->getWindow());
	renderingEngine->init();

	vector<CFlockingAgent*> flock;

	CFlockingAgent agent0(flock);
	flock.push_back(&agent0);
	agent0.SetHeading(Vector2(0.0f, -1.0f));
	agent0.SetPosition(Vector2(380.0f, 380.0f));
	Circle* model0 = new GDICircle(renderingEngine->getBuffer(), agent0.GetPosition(), 5.0f);
	renderingEngine->addModel(model0);
	agent0.SetModel(model0);

	CFlockingAgent agent1(flock);
	flock.push_back(&agent1);
	agent1.SetHeading(Vector2(0.0f, -1.0f));
	agent1.SetPosition(Vector2(420.0f, 380.0f));
	Circle* model1 = new GDICircle(renderingEngine->getBuffer(), agent1.GetPosition(), 5.0f);
	renderingEngine->addModel(model1);
	agent1.SetModel(model1);

	CFlockingAgent agent2(flock);
	flock.push_back(&agent2);
	agent2.SetHeading(Vector2(0.0f, -1.0f));
	agent2.SetPosition(Vector2(380.0f, 420.0f));
	Circle* model2 = new GDICircle(renderingEngine->getBuffer(), agent2.GetPosition(), 5.0f);
	renderingEngine->addModel(model2);
	agent2.SetModel(model2);

	CFlockingAgent agent3(flock);
	flock.push_back(&agent3);
	agent3.SetHeading(Vector2(0.0f, -1.0f));
	agent3.SetPosition(Vector2(420.0f, 420.0f));
	Circle* model3 = new GDICircle(renderingEngine->getBuffer(), agent3.GetPosition(), 5.0f);
	renderingEngine->addModel(model3);
	agent3.SetModel(model3);

	Timer timer;
	timer.reset();

    while(windowEngine->getMessage().message != WM_QUIT)
    {
		timer.tick();

		for (unsigned int uiIndex = 0; uiIndex < flock.size(); uiIndex++)
		{
			flock[uiIndex]->Update(timer.getDeltaTime());
		}

		// Advance the demo.
		windowEngine->advance();
        renderingEngine->advance();
    }

	renderingEngine->destroy();
    windowEngine->destroy();

    return windowEngine->getMessage().wParam;
}
