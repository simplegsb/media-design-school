#include "SimpleAgent.h"
#include "SteeringAgent.h"
#include "Timer.h"
#include "WinApiEngine.h"

using namespace std;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow)
{
    WinApiEngine windowEngine(L"Steering", instance, commandShow);
    windowEngine.init();

	//Agent* agent = new SimpleAgent();
	Agent* agent = new SteeringAgent();
	agent->position.x = 400.0f;
	agent->position.y = 100.0f;
	agent->velocity.x = 0.0f;
	agent->velocity.y = 1.0f;

	windowEngine.setAgent(agent);

	Timer timer;
	timer.reset();

    while(windowEngine.getMessage().message != WM_QUIT)
    {
		timer.tick();

		// Advance the demo.
		windowEngine.advance();

		agent->think(windowEngine.getRepulsionPoints(), windowEngine.getWaypoints(), timer.getDeltaTime());
    }

    windowEngine.destroy();

    return windowEngine.getMessage().wParam;
}
