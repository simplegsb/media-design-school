#include <sstream>

#include <windows.h>
#include <commctrl.h>

#include <gazengine/direct3d10/model/Direct3D10Mesh.h>
#include <gazengine/direct3d10/model/Direct3D10Text.h>
#include <gazengine/direct3d10/rendering/Direct3D10RenderingEngine.h>
#include <gazengine/direct3d10/scene/Direct3D10Light.h>
#include <gazengine/engine/DebugEngine.h>
#include <gazengine/Events.h>
#include <gazengine/input/MouseButtonEvent.h>
#include <gazengine/math/Interpolation.h>
#include <gazengine/Messages.h>
#include <gazengine/Timer.h>
#include <gazengine/winapi/WinApiEngine.h>

using namespace std;

const float PI = 3.14159265358979323f;

void onMouseButton(const void* message);
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow);

D3DXCOLOR g_beachSand(1.0f, 0.96f, 0.62f, 1.0f);
Model* g_bezier = NULL;
vector<D3DXVECTOR3> g_bezierControlPoints;
Direct3D10Camera* g_camera = NULL;
Direct3D10RenderingEngine* g_renderingEngine = NULL;

void onMouseButton(const void* message)
{
	const MouseButtonEvent* mouseButtonEvent = static_cast<const MouseButtonEvent*>(message);

	float x = ((float) mouseButtonEvent->x / 800.0f * g_camera->getFrameWidth()) - (g_camera->getFrameWidth() / 2.0f);
	float y = ((float) (600 - mouseButtonEvent->y) / 600.0f * g_camera->getFrameHeight()) - (g_camera->getFrameHeight() / 2.0f);
	g_bezierControlPoints.push_back(D3DXVECTOR3(x, y, 0.0f));

	vector<D3DXVECTOR3> interpolatedControlPoints = Interpolation::interpolateBezier(g_bezierControlPoints, g_bezierControlPoints.size() * 2);
	vector<Vertex> bezierPoints;
	for (unsigned int index = 0; index < interpolatedControlPoints.size(); index++)
	{
		Vertex vertex;
		vertex.color = g_beachSand;
		vertex.normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		vertex.pos = interpolatedControlPoints.at(index);
		bezierPoints.push_back(vertex);
	}

	if (g_bezier != NULL)
	{
		g_renderingEngine->removeModel(*g_bezier);
		g_bezier = NULL;
	}

	g_bezier = new Direct3D10Mesh(*g_renderingEngine->getDevice(), bezierPoints);
	g_renderingEngine->addModel(g_bezier);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow)
{
    WinApiEngine* windowEngine = new WinApiEngine(L"Terrain Generation", instance, commandShow);
    windowEngine->init();

	g_renderingEngine = new Direct3D10RenderingEngine(windowEngine->getWindow());
	g_renderingEngine->init();

	// Setup the camera.
	g_camera = new Direct3D10Camera();
	g_camera->setOrthogonal(40.0f, 30.0f);
	g_camera->setTranslation(D3DXVECTOR3(0.0f, 0.0f, -2.0f));
	g_camera->lookAt(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	g_renderingEngine->setCamera(g_camera);

	// Setup the shader.
	Direct3D10Shader* shader = new Direct3D10Shader(*g_renderingEngine->getDevice(), L"shader.fx", "defaultTechnique");
	g_renderingEngine->setShader(shader);

	// Augment the engines with debug capabilities.
	DebugEngine debugWindowEngine(windowEngine);
	DebugEngine debugRenderingEngine(g_renderingEngine);

	// Register the mouse observer.
	Messages::registerRecipient(MOUSE_BUTTON_EVENT, &onMouseButton);

    while(windowEngine->getMessage().message != WM_QUIT)
    {
		// Advance the demo.
		debugWindowEngine.advance();
        debugRenderingEngine.advance();
    }

	debugRenderingEngine.destroy();
    debugWindowEngine.destroy();

    return windowEngine->getMessage().wParam;
}
