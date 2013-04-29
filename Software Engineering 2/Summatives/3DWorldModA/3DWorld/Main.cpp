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
#include <gazengine/Events.h>
#include <gazengine/GazEngine.h>
#include <gazengine/input/KeyboardButtonEvent.h>
#include <gazengine/math/Math.h>
#include <gazengine/math/MathFunctions.h>
#include <gazengine/Messages.h>
#include <gazengine/model/Direct3D10ModelFactory.h>
#include <gazengine/model/shape/Sphere.h>
#include <gazengine/model/Text.h>
#include <gazengine/physics/BulletEngine.h>
#include <gazengine/physics/BulletPhysicsFactory.h>
#include <gazengine/physics/PhysXEngine.h>
#include <gazengine/physics/PhysXPhysicsFactory.h>
#include <gazengine/rendering/Direct3D10RenderingEngine.h>
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

void CreateControlInfo(SimpleTree& _rRoot);
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

void CreateControlInfo(SimpleTree& _rRoot)
{
	Entity* pControlInfo = new Entity;

	Text* pControlInfoLine0 = new Text(Vector2(10.0f, 10.0f), "Controls:");
	pControlInfoLine0->setColour(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	pControlInfo->addComponent(pControlInfoLine0);
	pControlInfoLine0->setEntity(pControlInfo);
	SimpleTree* pControlInfoLine0Node = new SimpleTree;
	pControlInfoLine0Node->setModel(pControlInfoLine0);
	_rRoot.addChild(pControlInfoLine0Node);

	Text* pControlInfoLine1 = new Text(Vector2(10.0f, 30.0f), "Move Forward: Up Arrow");
	pControlInfoLine1->setColour(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	pControlInfo->addComponent(pControlInfoLine1);
	pControlInfoLine1->setEntity(pControlInfo);
	SimpleTree* pControlInfoLine1Node = new SimpleTree;
	pControlInfoLine1Node->setModel(pControlInfoLine1);
	_rRoot.addChild(pControlInfoLine1Node);

	Text* pControlInfoLine2 = new Text(Vector2(10.0f, 50.0f), "Move Backward: Down Arrow");
	pControlInfoLine2->setColour(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	pControlInfo->addComponent(pControlInfoLine2);
	pControlInfoLine2->setEntity(pControlInfo);
	SimpleTree* pControlInfoLine2Node = new SimpleTree;
	pControlInfoLine2Node->setModel(pControlInfoLine2);
	_rRoot.addChild(pControlInfoLine2Node);

	Text* pControlInfoLine3 = new Text(Vector2(10.0f, 70.0f), "Turn Left: Left Arrow");
	pControlInfoLine3->setColour(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	pControlInfo->addComponent(pControlInfoLine3);
	pControlInfoLine3->setEntity(pControlInfo);
	SimpleTree* pControlInfoLine3Node = new SimpleTree;
	pControlInfoLine3Node->setModel(pControlInfoLine3);
	_rRoot.addChild(pControlInfoLine3Node);

	Text* pControlInfoLine4 = new Text(Vector2(10.0f, 90.0f), "Turn Right: Right Arrow");
	pControlInfoLine4->setColour(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	pControlInfo->addComponent(pControlInfoLine4);
	pControlInfoLine4->setEntity(pControlInfo);
	SimpleTree* pControlInfoLine4Node = new SimpleTree;
	pControlInfoLine4Node->setModel(pControlInfoLine4);
	_rRoot.addChild(pControlInfoLine4Node);

	Text* pControlInfoLine5 = new Text(Vector2(10.0f, 110.0f), "Rotate Turret: Mouse");
	pControlInfoLine5->setColour(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	pControlInfo->addComponent(pControlInfoLine5);
	pControlInfoLine5->setEntity(pControlInfo);
	SimpleTree* pControlInfoLine5Node = new SimpleTree;
	pControlInfoLine5Node->setModel(pControlInfoLine5);
	_rRoot.addChild(pControlInfoLine5Node);

	GazEngine::addEntity(pControlInfo);
}

void OnKeyboardButton(const void* _kpMessage)
{
	const KeyboardButtonEvent* kpEvent = static_cast<const KeyboardButtonEvent*>(_kpMessage);
	if (kpEvent->buttonState != Button::UP)
	{
		return;
	}

	if (kpEvent->button == Keyboard::Z)
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
	else if (kpEvent->button == Keyboard::F1)
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
		rotate(gpChassisCameraNode->getTransformation(), Math::PI * 0.25f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
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
	/*GazEngine::addEngine(new BulletEngine(Vector3(0.0f, 0.0f, 0.0f), 1.0f / 60.0f));
	PhysicsFactory::setInstance(new BulletPhysicsFactory);*/

	PhysXEngine* pPhysicsEngine = new PhysXEngine(Vector3(0.0f, -10.0f, 0.0f), 1.0f / 60.0f);
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
	gpStandardShader = shaderFactory->createShader(L"3dworld.fx", "litTechnique");
	gpRenderer->setShader(shaderFactory->createShader(L"3dworld.fx", "headlightTechnique"));
	pRenderingEngine->addRenderer(gpRenderer);

	gpCamera = new Direct3D10Camera;
	gpCamera->setNode(gpChassisCameraNode);
	gpCamera->setPerspective(0.25f * Math::PI, 4.0f / 3.0f);
	pRenderingEngine->setCamera(gpCamera);

	Light* pTheOnlyLight = new Direct3D10Light("theOnly");
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

	// Flying Camera
	/*pRoot->addChild(pCameraNode);
	getTranslation3(pCameraNode->getTransformation()).Y() = 11.0f;
	GazEngine::addEngine(new FlyingCameraEngine(pCamera, 10.0f));*/

	// Scene
	CreateControlInfo(*pRoot);
	CreateWorld(*pRoot);
	CreateShapes(*pRoot);
	SetCameraPosition();

	// Tank
	Matrix44 tankTransformation;
	setTranslation(tankTransformation, Vector3(25.0f, 0.75f, 25.0f));
	SimpleTree* pTurretNode = NULL;
	SimpleTree* pBarrelNode = NULL;
	Body* pTankBody = CreateTank(*pRoot, tankTransformation, gpChassisCameraNode, gpBarrelCameraNode, &pTurretNode,
		&pBarrelNode, pHeadlight0Node, pHeadlight1Node, pTurretLightNode);

	// Driver
	GazEngine::addEngine(new CDriverEngine(pTankBody, 2500.0f));

	// Gunner
	GazEngine::addEngine(new CGunnerEngine(pTurretNode, pBarrelNode, 0.05f));

	// Messaging
	Messages::registerRecipient(Events::KEYBOARD_BUTTON, OnKeyboardButton);

	// GO!!!
	GazEngine::setMaxFrameRate(60);
	GazEngine::play();

    return 0;
}
