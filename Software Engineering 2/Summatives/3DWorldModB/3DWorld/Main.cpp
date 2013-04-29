//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// Copyright (c) 2012 Media Design School
//
// SVN Info
// $Author: MEDIADESIGN\gary.buyn $
// $Date: 2013-01-23 09:43:23 +1300 (Wed, 23 Jan 2013) $
// $Rev: 1109 $
//
#include <gazengine/EntityFactory.h>
#include <gazengine/Events.h>
#include <gazengine/GazEngine.h>
#include <gazengine/input/KeyboardButtonEvent.h>
#include <gazengine/math/Math.h>
#include <gazengine/math/MathFunctions.h>
#include <gazengine/Messages.h>
#include <gazengine/model/Direct3D10ModelFactory.h>
#include <gazengine/model/shape/Sphere.h>
#include <gazengine/model/Text.h>
#include <gazengine/physics/PhysXEngine.h>
#include <gazengine/physics/PhysXPhysicsFactory.h>
#include <gazengine/rendering/Direct3D10RenderingEngine.h>
#include <gazengine/rendering/Direct3D10RenderingFactory.h>
#include <gazengine/rendering/Direct3D10ShaderFactory.h>
#include <gazengine/rendering/SimpleDirect3D10Renderer.h>
#include <gazengine/scene/Direct3D10Camera.h>
#include <gazengine/scene/Direct3D10Light.h>
#include <gazengine/scene/FlyingCameraEngine.h>
#include <gazengine/windowing/WinApiEngine.h>

#include "DepthSortingEngine.h"
#include "DriverEngine.h"
#include "GunnerEngine.h"
#include "Scene.h"

using namespace std;

void CreateControlInfo();
void CreateSlopeTests();
void OnKeyboardButton(const void* message);
void SetCameraPosition();
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int iCommandShow);

unsigned short gCamera = 0;
SimpleTree* gpBarrelCameraNode = NULL;
Camera* gpCamera = NULL;
SimpleTree* gpChassisCameraNode = NULL;
Direct3D10Shader* gpNoDepthTestShader = NULL;
SimpleDirect3D10Renderer* gpRenderer = NULL;
Direct3D10Shader* gpStandardShader = NULL;

void CreateControlInfo()
{
	Text* pControlInfoLine0 = new Text(Vector2(10.0f, 10.0f), "Controls:");
	pControlInfoLine0->setColour(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	EntityFactory::createAndAddEntity(Matrix44(), pControlInfoLine0, 0);

	Text* pControlInfoLine1 = new Text(Vector2(10.0f, 30.0f), "Move Forward: Up Arrow");
	pControlInfoLine1->setColour(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	EntityFactory::createAndAddEntity(Matrix44(), pControlInfoLine1, 0);

	Text* pControlInfoLine2 = new Text(Vector2(10.0f, 50.0f), "Move Backward: Down Arrow");
	pControlInfoLine2->setColour(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	EntityFactory::createAndAddEntity(Matrix44(), pControlInfoLine2, 0);

	Text* pControlInfoLine3 = new Text(Vector2(10.0f, 70.0f), "Turn Left: Left Arrow");
	pControlInfoLine3->setColour(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	EntityFactory::createAndAddEntity(Matrix44(), pControlInfoLine3, 0);

	Text* pControlInfoLine4 = new Text(Vector2(10.0f, 90.0f), "Turn Right: Right Arrow");
	pControlInfoLine4->setColour(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	EntityFactory::createAndAddEntity(Matrix44(), pControlInfoLine4, 0);

	Text* pControlInfoLine5 = new Text(Vector2(10.0f, 110.0f), "Rotate Turret: Mouse");
	pControlInfoLine5->setColour(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	EntityFactory::createAndAddEntity(Matrix44(), pControlInfoLine5, 0);
}

void CreateSlopeTests()
{
	Matrix44 transformation;

	setTranslation(transformation, Vector3(-10.0f, 8.0f, 50.0f));
	Model* pShallowSlope = ModelFactory::getInstance()->createPrismMesh(Vector3(5.0f, 8.0f, 5.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	pShallowSlope->setMaterial(CreatePrismRectangleModelMaterial());
	EntityFactory::createAndAddEntity(transformation, pShallowSlope, 0, CreateObstacleBodyMaterial());

	setTranslation(transformation, Vector3(0.0f, 8.5f, 50.0f));
	Model* pMediumSlope = ModelFactory::getInstance()->createPrismMesh(Vector3(5.0f, 8.5f, 5.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	pMediumSlope->setMaterial(CreatePrismRectangleModelMaterial());
	EntityFactory::createAndAddEntity(transformation, pMediumSlope, 0, CreateObstacleBodyMaterial());

	setTranslation(transformation, Vector3(10.0f, 9.0f, 50.0f));
	Model* pSteepSlope = ModelFactory::getInstance()->createPrismMesh(Vector3(5.0f, 9.0f, 5.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	pSteepSlope->setMaterial(CreatePrismRectangleModelMaterial());
	EntityFactory::createAndAddEntity(transformation, pSteepSlope, 0, CreateObstacleBodyMaterial());
}

void OnKeyboardButton(const void* _kpMessage)
{
	const KeyboardButtonEvent* kpEvent = static_cast<const KeyboardButtonEvent*>(_kpMessage);
	if (kpEvent->buttonState != Button::UP)
	{
		return;
	}

	if (kpEvent->button == Keyboard::F1)
	{
		gCamera = ++gCamera % 3;
		SetCameraPosition();
	}
	else if (kpEvent->button == Keyboard::F2)
	{
		gCamera = 0;
		SetCameraPosition();
	}
	else if (kpEvent->button == Keyboard::F3)
	{
		gCamera = 1;
		SetCameraPosition();
	}
	else if (kpEvent->button == Keyboard::F4)
	{
		gCamera = 2;
		SetCameraPosition();
	}
	else if (kpEvent->button == Keyboard::Z)
	{
		if (gpRenderer->getShader() == gpNoDepthTestShader)
		{
			gpRenderer->setShader(gpStandardShader);
		}
		else
		{
			gpRenderer->setShader(gpNoDepthTestShader);
		}
	}
}

void SetCameraPosition()
{
	if (gCamera == 0)
	{
		gpChassisCameraNode->getTransformation().setIdentity();
		rotate(gpChassisCameraNode->getTransformation(), Math::PI, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
		getTranslation3(gpChassisCameraNode->getTransformation()).Y() = -0.75f;
		getTranslation3(gpChassisCameraNode->getTransformation()).Z() = 5.0f;
		gpCamera->setNode(gpChassisCameraNode);
	}
	else if (gCamera == 1)
	{
		getTranslation3(gpBarrelCameraNode->getTransformation()).X() = 1.0f;
		gpCamera->setNode(gpBarrelCameraNode);
	}
	else if (gCamera == 2)
	{
		gpChassisCameraNode->getTransformation().setIdentity();
		rotate(gpChassisCameraNode->getTransformation(), Math::PI, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
		rotate(gpChassisCameraNode->getTransformation(), Math::PI * 0.15f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		getTranslation3(gpChassisCameraNode->getTransformation()).Y() = -10.0f;
		getTranslation3(gpChassisCameraNode->getTransformation()).Z() = -10.0f;
		gpCamera->setNode(gpChassisCameraNode);
	}
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int _iCommandShow)
{
	// Unsorted scene
	SimpleTree* pRoot = new SimpleTree;
	GazEngine::addWorldRepresentation(pRoot);
	gpChassisCameraNode = new SimpleTree;
	gpBarrelCameraNode = new SimpleTree;
	SimpleTree* pHeadlight0Node = new SimpleTree;
	SimpleTree* pHeadlight1Node = new SimpleTree;
	SimpleTree* pTurretLightNode = new SimpleTree;

	// Window
    WinApiEngine* pWindowEngine = new WinApiEngine(L"3D World", instance, _iCommandShow);
	pWindowEngine->setWidth(1024);
	pWindowEngine->setHeight(768);
    pWindowEngine->createWindow();
	GazEngine::addEngine(pWindowEngine);

	// Physics
	PhysXEngine* pPhysicsEngine = new PhysXEngine(Vector3(0.0f, -9.8f, 0.0f), 1.0f / 60.0f);
	GazEngine::addEngine(pPhysicsEngine);
	PhysicsFactory::setInstance(new PhysXPhysicsFactory(pPhysicsEngine->getPhysics(), pPhysicsEngine->getCooking()));

	// Manual Depth Sorting
	CDepthSortingEngine* pDepthSortingEngine = new CDepthSortingEngine(pRoot, gpChassisCameraNode);
	GazEngine::addEngine(pDepthSortingEngine);

	// Rendering
	Direct3D10RenderingEngine* pRenderingEngine = new Direct3D10RenderingEngine(pWindowEngine->getWindow());
	pDepthSortingEngine->setRenderingEngine(pRenderingEngine);
	pRenderingEngine->setWidth(1024);
	pRenderingEngine->setHeight(768);

	gpRenderer = new SimpleDirect3D10Renderer(*pRenderingEngine->getDevice());
	Direct3D10ShaderFactory* shaderFactory = new Direct3D10ShaderFactory(*pRenderingEngine->getDevice());
	gpNoDepthTestShader = shaderFactory->createShader(L"3dworld.fx", "litNoDepthTestTechnique");
	gpStandardShader = shaderFactory->createShader(L"3dworld.fx", "headlightTechnique");
	gpRenderer->setShader(gpStandardShader);
	pRenderingEngine->addRenderer(gpRenderer);

	gpCamera = new Direct3D10Camera;
	gpCamera->setNode(gpChassisCameraNode);
	gpCamera->setPerspective(0.25f * Math::PI, 4.0f / 3.0f);
	pRenderingEngine->setCamera(gpCamera);

	Direct3D10Light* pTheOnlyLight = new Direct3D10Light("theOnly");
	pTheOnlyLight->setAmbientComponent(Vector4(0.1f, 0.1f, 0.1f, 1.0f));
	pTheOnlyLight->setDiffuseComponent(Vector4(0.1f, 0.1f, 0.1f, 1.0f));
	pTheOnlyLight->setSpecularComponent(Vector4(0.1f, 0.1f, 0.1f, 1.0f));
	pTheOnlyLight->setTranslation(Vector3(0.0f, 100.0f, 0.0f));
	pRenderingEngine->addLight(pTheOnlyLight);

	Direct3D10Light* pHeadlight0 = new Direct3D10Light("headlight0");
	pHeadlight0->setAmbientComponent(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	pHeadlight0->setAttenuation(Vector3(0.5f, 0.05f, 0.005f));
	pHeadlight0->setDiffuseComponent(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	pHeadlight0->setInnerAngle(10.0f / 180.0f * Math::PI);
	pHeadlight0->setOuterAngle(30.0f / 180.0f * Math::PI);
	pHeadlight0->setRange(50.0f);
	pHeadlight0->setSpecularComponent(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	pHeadlight0->setStrengthAtRange(0.0f);
	pHeadlight0->setNode(pHeadlight0Node);
	pRenderingEngine->addLight(pHeadlight0);

	Direct3D10Light* pHeadlight1 = new Direct3D10Light("headlight1");
	pHeadlight1->setAmbientComponent(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	pHeadlight1->setAttenuation(Vector3(0.5f, 0.05f, 0.005f));
	pHeadlight1->setDiffuseComponent(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	pHeadlight1->setInnerAngle(10.0f / 180.0f * Math::PI);
	pHeadlight1->setOuterAngle(30.0f / 180.0f * Math::PI);
	pHeadlight1->setRange(50.0f);
	pHeadlight1->setSpecularComponent(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	pHeadlight1->setStrengthAtRange(0.0f);
	pHeadlight1->setNode(pHeadlight1Node);
	pRenderingEngine->addLight(pHeadlight1);

	Direct3D10Light* pTurretLight = new Direct3D10Light("turret");
	pTurretLight->setAmbientComponent(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	pTurretLight->setAttenuation(Vector3(0.5f, 0.05f, 0.005f));
	pTurretLight->setDiffuseComponent(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	pTurretLight->setInnerAngle(10.0f / 180.0f * Math::PI);
	pTurretLight->setOuterAngle(30.0f / 180.0f * Math::PI);
	pTurretLight->setRange(25.0f);
	pTurretLight->setSpecularComponent(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	pTurretLight->setStrengthAtRange(0.0f);
	pTurretLight->setNode(pTurretLightNode);
	pRenderingEngine->addLight(pTurretLight);

	GazEngine::addEngine(pRenderingEngine);
	ModelFactory::setInstance(new Direct3D10ModelFactory(*pRenderingEngine->getDevice()));
	RenderingFactory::setInstance(new Direct3D10RenderingFactory(*pRenderingEngine->getDevice()));

	// Flying Camera
	//pRoot->addChild(gpChassisCameraNode);
	//getTranslation3(gpChassisCameraNode->getTransformation()).Y() = 11.0f;
	//GazEngine::addEngine(new FlyingCameraEngine(gpCamera, 10.0f));
	//pTheOnlyLight->setNode(gpChassisCameraNode);

	// Scene
	CreateControlInfo();
	CreateWorld(*pRoot);
	CreateShapes(*pRoot);
	SetCameraPosition();

	// Tank
	Matrix44 tankTransformation;
	setTranslation(tankTransformation, Vector3(25.0f, 0.75f, 25.0f));
	SimpleTree* pTurretNode = NULL;
	SimpleTree* pBarrelNode = NULL;
	Body* pTankBody = CreateTank(*pRoot, tankTransformation, gpChassisCameraNode, gpBarrelCameraNode, &pTurretNode,
		&pBarrelNode, pHeadlight0Node, pHeadlight1Node, pTurretLightNode,
		RenderingFactory::getInstance()->createTexture("tank.jpg"));

	// Slope tests
	CreateSlopeTests();

	// Driver
	GazEngine::addEngine(new CDriverEngine(pTankBody, 2.0f * 5.0f, 1.0f, 50.0f));

	// Gunner
	GazEngine::addEngine(new CGunnerEngine(pRoot, pTurretNode, pBarrelNode, 0.05f, 0.25f));

	// Messaging
	Messages::registerRecipient(Events::KEYBOARD_BUTTON, OnKeyboardButton);

	// GO!!!
	GazEngine::setMaxFrameRate(60);
	GazEngine::play();

    return 0;
}
