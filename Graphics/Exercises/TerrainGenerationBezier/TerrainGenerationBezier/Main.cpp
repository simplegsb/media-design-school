#include <sstream>

#include <windows.h>
#include <commctrl.h>

#include <gazengine/direct3d10/model/Direct3D10Mesh.h>
#include <gazengine/direct3d10/model/Direct3D10TerrainFactory.h>
#include <gazengine/direct3d10/model/Direct3D10Text.h>
#include <gazengine/direct3d10/rendering/Direct3D10RenderingEngine.h>
#include <gazengine/direct3d10/scene/Direct3D10Light.h>
#include <gazengine/engine/DebugEngine.h>
#include <gazengine/Timer.h>
#include <gazengine/winapi/WinApiEngine.h>

using namespace std;

const float PI = 3.14159265358979323f;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow);

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow)
{
    WinApiEngine* windowEngine = new WinApiEngine(L"Terrain Generation (Bezier)", instance, commandShow);
    windowEngine->init();

	Direct3D10RenderingEngine* renderingEngine = new Direct3D10RenderingEngine(windowEngine->getWindow());
	renderingEngine->init();

	// Setup the camera.
	Direct3D10Camera* camera = new Direct3D10Camera();
	camera->setPerspective(0.25f * PI, float(800) / 600);
	camera->setTranslation(D3DXVECTOR3(0.0f, 0.0f, -40.0f));
	camera->lookAt(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	renderingEngine->setCamera(camera);

	// Setup the shader.
	Direct3D10Shader* shader = new Direct3D10Shader(*renderingEngine->getDevice(), L"shader.fx", "defaultTechnique");
	renderingEngine->setShader(shader);

	// Setup the models.
	Model* terrain = NULL;
	D3DXCOLOR beachSand(1.0f, 0.96f, 0.62f, 1.0f);
	terrain = Direct3D10TerrainFactory::createMidPointDisplacementTerrain(*renderingEngine->getDevice(), 30.0f, 2, beachSand, 20);
	renderingEngine->addModel(terrain);

	RECT performanceTextRectangle;
	performanceTextRectangle.left = 5;
	performanceTextRectangle.top = 5;
	D3DXCOLOR black(0.0f, 0.0f, 0.0f, 1.0f);
	Direct3D10Text* performanceText = new Direct3D10Text(*renderingEngine->getDevice(), L"Performance:", performanceTextRectangle, black);
	renderingEngine->addModel(performanceText);

	// Augment the engines with debug capabilities.
	DebugEngine debugWindowEngine(windowEngine);
	DebugEngine debugRenderingEngine(renderingEngine);

	// Initialise timer.
	Timer timer;
	timer.reset();
	float performanceUpdateTime = timer.getGameTime();

    while(windowEngine->getMessage().message != WM_QUIT)
    {
		timer.tick();

		// Advance the demo.
		debugWindowEngine.advance();
        debugRenderingEngine.advance();

		// Once per second.
		if (timer.getGameTime() - performanceUpdateTime > 1.0f)
		{
			performanceUpdateTime = timer.getGameTime();

			// Update the performance stats.
			wostringstream outs;   
			outs.precision(6);
			outs << L"Performance:\n"
				<< L"WindowEngine: " << debugWindowEngine.getAdvanceTime() * 1000.0f << L"ms\n"
				<< L"RenderingEngine: " << debugRenderingEngine.getAdvanceTime() * 1000.0f << L"ms";
			performanceText->setText(outs.str());
		}
    }

	debugRenderingEngine.destroy();
    debugWindowEngine.destroy();

    return windowEngine->getMessage().wParam;
}
