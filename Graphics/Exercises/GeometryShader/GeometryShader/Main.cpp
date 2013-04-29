#include <sstream>

#include <windows.h>

#include "DebugEngine.h"
#include "Direct3D10Light.h"
#include "Direct3D10Mesh.h"
#include "Direct3D10RenderingEngine.h"
#include "Direct3D10Text.h"
#include "Timer.h"
#include "WinApiEngine.h"

using namespace std;

const float PI = 3.14159265358979323f;

BOOL CALLBACK DialogWndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow);

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow)
{
    WinApiEngine* windowEngine = new WinApiEngine(L"Geometry Shader", instance, commandShow);
    windowEngine->init();

	Direct3D10RenderingEngine* renderingEngine = new Direct3D10RenderingEngine(windowEngine->getWindow());
	renderingEngine->init();

	// Setup the camera.
	Direct3D10Camera* camera = new Direct3D10Camera();
	camera->setPerspective(0.25f * PI, float(800) / 600);
	camera->setTranslation(D3DXVECTOR3(0.0f, 0.0f, 50.0f));
	camera->lookAt(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	renderingEngine->setCamera(camera);

	// Setup the shader.
	//Direct3D10Shader* shader = new Direct3D10Shader(*renderingEngine->getDevice(), L"shader.fx", "defaultTechnique");
	Direct3D10Shader* shader = new Direct3D10Shader(*renderingEngine->getDevice(), L"shader.fx", "starTechnique");
	renderingEngine->setShader(shader);

	// Add the models.
	vector<DWORD> indices;
	indices.reserve(3);
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	vector<Vertex> vertices(3, Vertex());
	D3DXCOLOR beachSand(1.0f, 0.96f, 0.62f, 1.0f);
	vertices.at(0).color = beachSand;
	vertices.at(0).normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertices.at(0).pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	vertices.at(1).color = beachSand;
	vertices.at(1).normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertices.at(1).pos = D3DXVECTOR3(-20.0f, -10.0f, 0.0f);
	vertices.at(2).color = beachSand;
	vertices.at(2).normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vertices.at(2).pos = D3DXVECTOR3(20.0f, -10.0f, 0.0f);

	renderingEngine->addModel(new Direct3D10Mesh(*renderingEngine->getDevice(), indices, vertices));

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

    debugWindowEngine.destroy();
	debugRenderingEngine.destroy();

    return windowEngine->getMessage().wParam;
}
