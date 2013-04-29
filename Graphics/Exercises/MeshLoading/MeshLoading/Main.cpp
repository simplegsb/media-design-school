#include <sstream>

#include <windows.h>
#include <commctrl.h>

#include <gazengine/direct3d10/model/Direct3D10Mesh.h>
#include <gazengine/direct3d10/model/Direct3D10ModelFactory.h>
#include <gazengine/direct3d10/rendering/Direct3D10RenderingEngine.h>
#include <gazengine/Events.h>
#include <gazengine/input/MouseButtonEvent.h>
#include <gazengine/Messages.h>
#include <gazengine/Timer.h>
#include <gazengine/winapi/WinApiEngine.h>

using namespace std;

const float PI = 3.14159265358979323f;

void onMouseButton(const void* message);
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow);

Direct3D10Camera* g_camera = NULL;
Direct3D10RenderingEngine* g_renderingEngine = NULL;

void onMouseButton(const void* message)
{
	const MouseButtonEvent* mouseButtonEvent = static_cast<const MouseButtonEvent*>(message);

	float x = ((float) mouseButtonEvent->x / 800.0f * g_camera->getFrameWidth()) - (g_camera->getFrameWidth() / 2.0f);
	float y = ((float) (600 - mouseButtonEvent->y) / 600.0f * g_camera->getFrameHeight()) - (g_camera->getFrameHeight() / 2.0f);

	D3DXMATRIX transformation;
	transformation._41 = x;
	transformation._42 = x;

	// Setup the shader.
	Direct3D10Shader* shader = new Direct3D10Shader(*g_renderingEngine->getDevice(), L"shader.fx", "defaultTechnique");

	// Setup the model.
	D3DXCOLOR beachSand(1.0f, 0.96f, 0.62f, 1.0f);
	Model* cube = Direct3D10ModelFactory::createCube(*g_renderingEngine->getDevice(), 10, beachSand);
	static_cast<Direct3D10Mesh*>(cube)->setTransformation(transformation);
	static_cast<Direct3D10Mesh*>(cube)->setShader(shader);
	g_renderingEngine->addModel(cube);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int commandShow)
{
    WinApiEngine* windowEngine = new WinApiEngine(L"Mesh Loading", instance, commandShow);
    windowEngine->init();

	g_renderingEngine = new Direct3D10RenderingEngine(windowEngine->getWindow());
	g_renderingEngine->init();

	// Setup the camera.
	g_camera = new Direct3D10Camera();
	g_camera->setPerspective(0.25f * PI, float(800) / 600);
	g_renderingEngine->setCamera(g_camera);

	// Setup the lights.
	Direct3D10Light* parallelLight = new Direct3D10Light("parallel");
	parallelLight->setAmbientComponent(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
	parallelLight->setAttenuation(D3DXVECTOR3(0.005f, 0.005f, 0.005f));
	parallelLight->setDiffuseComponent(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	parallelLight->setDirection(D3DXVECTOR3(-0.3f, -0.3f, -0.4f));
	parallelLight->setRange(1000.0f);
	parallelLight->setSpecularComponent(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	parallelLight->setStrength(0.1f);
	parallelLight->setTranslation(D3DXVECTOR3(-150.0f, 150.0f, 150.0f));
	g_renderingEngine->addLight(parallelLight);

	Direct3D10Light* pointLight = new Direct3D10Light("point");
	pointLight->setAmbientComponent(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
	pointLight->setAttenuation(D3DXVECTOR3(0.005f, 0.005f, 0.005f));
	pointLight->setDirection(D3DXVECTOR3(-0.3f, -0.3f, -0.4f));
	pointLight->setDiffuseComponent(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	pointLight->setRange(1000.0f);
	pointLight->setSpecularComponent(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	pointLight->setStrength(0.1f);
	pointLight->setTranslation(D3DXVECTOR3(150.0f, 150.0f, 150.0f));
	g_renderingEngine->addLight(pointLight);

	// Setup the shader.
	Direct3D10Shader* shader = new Direct3D10Shader(*g_renderingEngine->getDevice(), L"shader.fx", "cartoonTechnique");
	//Direct3D10Shader* shader = new Direct3D10Shader(*g_renderingEngine->getDevice(), L"shader.fx", "defaultTechnique");
	//Direct3D10Shader* shader = new Direct3D10Shader(*g_renderingEngine->getDevice(), L"shader.fx", "litTechnique");
	
	// Setup the model.
	D3DXCOLOR beachSand(1.0f, 0.96f, 0.62f, 1.0f);
	//Model* model = Direct3D10ModelFactory::createCube(*g_renderingEngine->getDevice(), 10, beachSand);
	Model* model = Direct3D10ModelFactory::loadObjModel(*g_renderingEngine->getDevice(), "palm.obj");
	//Model* model = Direct3D10ModelFactory::loadObjModel(*g_renderingEngine->getDevice(), "chuck2.obj");
	static_cast<Direct3D10Mesh*>(model)->setShader(shader);
	g_renderingEngine->addModel(model);

	// Register the mouse observer.
	Messages::registerRecipient(MOUSE_BUTTON_EVENT, &onMouseButton);

	// Initialise timer.
	Timer timer;
	timer.reset();

    while(windowEngine->getMessage().message != WM_QUIT)
    {
		timer.tick();

		// Update the camera position as a function of time.
		float radius = 500.0f;
		float x = radius * cosf(0.5f * timer.getGameTime());
		float z = radius * sinf(0.5f * timer.getGameTime());
		float y = 50.0f * sinf(0.5f * timer.getGameTime()) + 50.0f;

		g_camera->setTranslation(D3DXVECTOR3(x, y, z));
		g_camera->lookAt(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));

		// Advance the demo.
		windowEngine->advance();
        g_renderingEngine->advance();
    }

	g_renderingEngine->destroy();
    windowEngine->destroy();

    return windowEngine->getMessage().wParam;
}
