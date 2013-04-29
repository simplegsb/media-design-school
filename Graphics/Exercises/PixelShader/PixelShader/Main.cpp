#include <sstream>

#include <windows.h>
#include <commctrl.h>

#include "DebugEngine.h"
#include "Direct3D10Light.h"
#include "Direct3D10ModelFactory.h"
#include "Direct3D10RenderingEngine.h"
#include "Direct3D10Text.h"
#include "resource.h"
#include "Timer.h"
#include "WinApiEngine.h"

using namespace std;

const float PI = 3.14159265358979323f;

BOOL CALLBACK DialogWndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
void setupLights();
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow);

Direct3D10Light* parallelLight = false;
Direct3D10Light* pointLight = false;

BOOL CALLBACK DialogWndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CLOSE)
	{
		DestroyWindow(window);
		return true;
	}
	else if (message == WM_COMMAND)
	{
		if (LOWORD(wParam) == IDC_PARALLEL)
		{
			if (parallelLight->isActive())
			{
				parallelLight->deactivate();
			}
			else
			{
				parallelLight->activate();
			}
			return true;
		}
		else if (LOWORD(wParam) == IDC_POINT)
		{
			if (pointLight->isActive())
			{
				pointLight->deactivate();
			}
			else
			{
				pointLight->activate();
			}
			return true;
		}
	}

	return false;
}

void setupLights()
{
	parallelLight = new Direct3D10Light("parallel");
	parallelLight->deactivate();
	parallelLight->setAmbientComponent(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
	parallelLight->setAttenuation(D3DXVECTOR3(0.005f, 0.005f, 0.005f));
	parallelLight->setDiffuseComponent(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	parallelLight->setDirection(D3DXVECTOR3(-0.3f, -0.3f, -0.4f));
	parallelLight->setRange(100.0f);
	parallelLight->setSpecularComponent(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	parallelLight->setStrength(0.1f);
	parallelLight->setTranslation(D3DXVECTOR3(-15.0f, 15.0f, 15.0f));

	pointLight = new Direct3D10Light("point");
	pointLight->deactivate();
	pointLight->setAmbientComponent(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
	pointLight->setAttenuation(D3DXVECTOR3(0.005f, 0.005f, 0.005f));
	pointLight->setDirection(D3DXVECTOR3(-0.3f, -0.3f, -0.4f));
	pointLight->setDiffuseComponent(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	pointLight->setRange(100.0f);
	pointLight->setSpecularComponent(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	pointLight->setStrength(0.1f);
	pointLight->setTranslation(D3DXVECTOR3(15.0f, 15.0f, 15.0f));
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow)
{
    WinApiEngine* windowEngine = new WinApiEngine(L"Pixel Shader", instance, commandShow);
    windowEngine->init();

	Direct3D10RenderingEngine* renderingEngine = new Direct3D10RenderingEngine(windowEngine->getWindow());
	renderingEngine->init();

	// Setup the camera.
	Direct3D10Camera* camera = new Direct3D10Camera();
	camera->setPerspective(0.25f * PI, float(800) / 600);
	renderingEngine->setCamera(camera);

	// Setup the lights.
	setupLights();
	renderingEngine->addLight(parallelLight);
	renderingEngine->addLight(pointLight);

	// Setup the shader.
	//Direct3D10Shader* shader = new Direct3D10Shader(*renderingEngine->getDevice(), L"shader.fx", "defaultTechnique");
	Direct3D10Shader* shader = new Direct3D10Shader(*renderingEngine->getDevice(), L"shader.fx", "cartoonTechnique");
	renderingEngine->setShader(shader);

	// Add the models.
	D3DXCOLOR beachSand(1.0f, 0.96f, 0.62f, 1.0f);
	renderingEngine->addModel(Direct3D10ModelFactory::createCube(*renderingEngine->getDevice(), 10.0f, beachSand));

	RECT performanceTextRectangle;
	performanceTextRectangle.left = 5;
	performanceTextRectangle.top = 5;
	D3DXCOLOR black(0.0f, 0.0f, 0.0f, 1.0f);
	Direct3D10Text* performanceText = new Direct3D10Text(*renderingEngine->getDevice(), L"Performance:", performanceTextRectangle, black);
	renderingEngine->addModel(performanceText);

	// Augment the engines with debug capabilities.
	DebugEngine debugWindowEngine(windowEngine);
	DebugEngine debugRenderingEngine(renderingEngine);

	// Create the control panel.
	HWND controlPanel = CreateDialog(instance, MAKEINTRESOURCE(IDD_CONTROL_PANEL), NULL, DialogWndProc);
	ShowWindow(controlPanel, SW_SHOW);
	windowEngine->setDialog(controlPanel);

	// Initialise timer.
	Timer timer;
	timer.reset();
	float performanceUpdateTime = timer.getGameTime();

    while(windowEngine->getMessage().message != WM_QUIT)
    {
		timer.tick();

		// Update the camera position as a function of time.
		float radius = 100.0f;
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

			// Update the performance stats.
			wostringstream outs;   
			outs.precision(6);
			outs << L"Performance:\n"
				<< L"WindowEngine: " << debugWindowEngine.getAdvanceTime() * 1000.0f << L"ms\n"
				<< L"RenderingEngine: " << debugRenderingEngine.getAdvanceTime() * 1000.0f << L"ms";
			performanceText->setText(outs.str());
		}
    }

    renderingEngine->destroy();
	windowEngine->destroy();

    return windowEngine->getMessage().wParam;
}
