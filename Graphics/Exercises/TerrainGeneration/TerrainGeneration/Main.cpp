#include <sstream>

#include <windows.h>
#include <commctrl.h>

#include "DebugEngine.h"
#include "Direct3D10Light.h"
#include "Direct3D10ModelFactory.h"
#include "Direct3D10RenderingEngine.h"
#include "Direct3D10Text.h"
#include "Timer.h"
#include "WinApiEngine.h"

using namespace std;

const float PI = 3.14159265358979323f;

void setupLights();
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow);

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow)
{
    WinApiEngine* windowEngine = new WinApiEngine(L"Terrain Generation", instance, commandShow);
    windowEngine->init();

	Direct3D10RenderingEngine* renderingEngine = new Direct3D10RenderingEngine(windowEngine->getWindow());
	renderingEngine->init();

	// Setup the camera.
	Direct3D10Camera* camera = new Direct3D10Camera();
	camera->setPerspective(0.25f * PI, float(800) / 600);
	//camera->setTranslation(D3DXVECTOR3(0.0f, 0.0f, -40.0f));
	//camera->lookAt(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	renderingEngine->setCamera(camera);

	// Setup the lights.
	Direct3D10Light* parallelLight = new Direct3D10Light("parallel");
	parallelLight->setAmbientComponent(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
	parallelLight->setAttenuation(D3DXVECTOR3(0.005f, 0.005f, 0.005f));
	parallelLight->setDiffuseComponent(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	parallelLight->setDirection(D3DXVECTOR3(-0.3f, -0.3f, -0.4f));
	parallelLight->setRange(100.0f);
	parallelLight->setSpecularComponent(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	parallelLight->setStrength(0.1f);
	parallelLight->setTranslation(D3DXVECTOR3(-15.0f, 15.0f, 15.0f));
	renderingEngine->addLight(parallelLight);

	Direct3D10Light* pointLight = new Direct3D10Light("point");
	pointLight->setAmbientComponent(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
	pointLight->setAttenuation(D3DXVECTOR3(0.005f, 0.005f, 0.005f));
	pointLight->setDirection(D3DXVECTOR3(-0.3f, -0.3f, -0.4f));
	pointLight->setDiffuseComponent(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	pointLight->setRange(100.0f);
	pointLight->setSpecularComponent(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	pointLight->setStrength(0.1f);
	pointLight->setTranslation(D3DXVECTOR3(15.0f, 15.0f, 15.0f));
	renderingEngine->addLight(pointLight);

	// Setup the shader.
	//Direct3D10Shader* shader = new Direct3D10Shader(*renderingEngine->getDevice(), L"shader.fx", "cartoonTechnique");
	//Direct3D10Shader* shader = new Direct3D10Shader(*renderingEngine->getDevice(), L"shader.fx", "defaultTechnique");
	Direct3D10Shader* shader = new Direct3D10Shader(*renderingEngine->getDevice(), L"shader.fx", "litTechnique");
	renderingEngine->setShader(shader);

	// Setup the models.
	Model* terrain = NULL;
	D3DXCOLOR beachSand(1.0f, 0.96f, 0.62f, 1.0f);
	//terrain = Direct3D10ModelFactory::create2DPerlinTerrain(*renderingEngine->getDevice(), 30.0f, 100, 7, 0.25f, beachSand);
	terrain = Direct3D10ModelFactory::create3DPerlinTerrain(*renderingEngine->getDevice(), 100.0f, 100.0f, 5.0f, 25, 6, 0.5f, beachSand);
	//terrain = Direct3D10ModelFactory::createMidPointDisplacementTerrain(*renderingEngine->getDevice(), 30.0f, 7, beachSand);
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

		// Update the camera position as a function of time.
		float radius = 200.0f;
		float x = radius * cosf(0.5f * timer.getGameTime());
		float z = radius * sinf(0.5f * timer.getGameTime());
		float y = 50.0f * sinf(0.5f * timer.getGameTime()) + 50.0f;

		camera->setTranslation(D3DXVECTOR3(x, y, z));
		camera->lookAt(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));

		// Advance the demo.
		debugWindowEngine.advance();
        debugRenderingEngine.advance();

		// Once per second.
		if (timer.getGameTime() - performanceUpdateTime > 1.0f)
		{
			performanceUpdateTime = timer.getGameTime();

			// Re-create terrain.
			/*renderingEngine->removeModel(*terrain);
			terrain = Direct3D10ModelFactory::createMidPointDisplacementTerrain(*renderingEngine->getDevice(), 30.0f, 7, beachSand);
			renderingEngine->addModel(terrain);
			// Unfortunately my stupid engine requires the text to be added last so we'll remove and re-add it.
			renderingEngine->removeModel(*performanceText);
			renderingEngine->addModel(performanceText);*/

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
