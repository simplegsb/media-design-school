
#include <gazengine/gdi/rendering/GDIRenderer.h>
#include <gazengine/gdi/rendering/GDIRenderingEngine.h>
#include <gazengine/Timer.h>
#include <gazengine/winapi/WinApiEngine.h>

#include "ParticleSystem.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow)
{
    WinApiEngine windowEngine(L"Particle System", instance, commandShow);
    windowEngine.init();

	GDIRenderingEngine renderingEngine(windowEngine.getWindow());
	renderingEngine.init();
	renderingEngine.addRenderer(new GDIRenderer(renderingEngine.getBuffer()));

	CParticleSystem* particleSystem = new CParticleSystem(50, 1.0f, 100.0f, Vector2(400.0f, 300.0f));
	renderingEngine.addModel(particleSystem);

	Timer timer;
	timer.reset();

    while(windowEngine.getMessage().message != WM_QUIT)
    {
		timer.tick();

		particleSystem->advance(timer.getDeltaTime());
		windowEngine.advance();
		renderingEngine.advance();
    }

	renderingEngine.destroy();
    windowEngine.destroy();

    return windowEngine.getMessage().wParam;
}
