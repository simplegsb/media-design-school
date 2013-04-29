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
#include <gazengine/physics/BulletBody.h>
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
#include "Scene.h"

using namespace std;

Direct3D10Shader* gpNoDepthTestShader = NULL;
SimpleDirect3D10Renderer* gpRenderer = NULL;
Direct3D10Shader* gpStandardShader = NULL;

void CreateControlInfo(SimpleTree& _rRoot);
void ToggleDepthTest(const void* message);
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int iCommandShow);

void CreateControlInfo(SimpleTree& _rRoot)
{
	Entity* pControlInfo = new Entity;

	Text* pControlInfoLine0 = new Text(Vector2(10.0f, 10.0f), "Controls:");
	pControlInfo->addComponent(pControlInfoLine0);
	pControlInfoLine0->setEntity(pControlInfo);
	SimpleTree* pControlInfoLine0Node = new SimpleTree;
	pControlInfoLine0Node->setModel(pControlInfoLine0);
	_rRoot.addChild(pControlInfoLine0Node);

	Text* pControlInfoLine1 = new Text(Vector2(10.0f, 30.0f), "Move Forward: Up Arrow");
	pControlInfo->addComponent(pControlInfoLine1);
	pControlInfoLine1->setEntity(pControlInfo);
	SimpleTree* pControlInfoLine1Node = new SimpleTree;
	pControlInfoLine1Node->setModel(pControlInfoLine1);
	_rRoot.addChild(pControlInfoLine1Node);

	Text* pControlInfoLine2 = new Text(Vector2(10.0f, 50.0f), "Move Backward: Down Arrow");
	pControlInfo->addComponent(pControlInfoLine2);
	pControlInfoLine2->setEntity(pControlInfo);
	SimpleTree* pControlInfoLine2Node = new SimpleTree;
	pControlInfoLine2Node->setModel(pControlInfoLine2);
	_rRoot.addChild(pControlInfoLine2Node);

	Text* pControlInfoLine3 = new Text(Vector2(10.0f, 70.0f), "Turn Left: Left Arrow");
	pControlInfo->addComponent(pControlInfoLine3);
	pControlInfoLine3->setEntity(pControlInfo);
	SimpleTree* pControlInfoLine3Node = new SimpleTree;
	pControlInfoLine3Node->setModel(pControlInfoLine3);
	_rRoot.addChild(pControlInfoLine3Node);

	Text* pControlInfoLine4 = new Text(Vector2(10.0f, 90.0f), "Turn Right: Right Arrow");
	pControlInfo->addComponent(pControlInfoLine4);
	pControlInfoLine4->setEntity(pControlInfo);
	SimpleTree* pControlInfoLine4Node = new SimpleTree;
	pControlInfoLine4Node->setModel(pControlInfoLine4);
	_rRoot.addChild(pControlInfoLine4Node);

	Text* pControlInfoLine5 = new Text(Vector2(10.0f, 110.0f), "Move Up: +");
	pControlInfo->addComponent(pControlInfoLine5);
	pControlInfoLine5->setEntity(pControlInfo);
	SimpleTree* pControlInfoLine5Node = new SimpleTree;
	pControlInfoLine5Node->setModel(pControlInfoLine5);
	_rRoot.addChild(pControlInfoLine5Node);

	Text* pControlInfoLine6 = new Text(Vector2(10.0f, 130.0f), "Move Down: -");
	pControlInfo->addComponent(pControlInfoLine6);
	pControlInfoLine6->setEntity(pControlInfo);
	SimpleTree* pControlInfoLine6Node = new SimpleTree;
	pControlInfoLine6Node->setModel(pControlInfoLine6);
	_rRoot.addChild(pControlInfoLine6Node);

	GazEngine::addEntity(pControlInfo);
}

void ToggleDepthTest(const void* _kpMessage)
{
	const KeyboardButtonEvent* kpEvent = static_cast<const KeyboardButtonEvent*>(_kpMessage);
	if (kpEvent->button != Keyboard::Z || kpEvent->buttonState != Button::UP)
	{
		return;
	}

	if (gpRenderer->getShader() == gpNoDepthTestShader)
	{
		gpRenderer->setShader(gpStandardShader);
	}
	else
	{
		gpRenderer->setShader(gpNoDepthTestShader);
	}
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int _iCommandShow)
{
	// Unsorted scene
	SimpleTree* pRoot = new SimpleTree;
	GazEngine::addWorldRepresentation(pRoot);
	SimpleTree* pCameraNode = new SimpleTree;
	pRoot->addChild(pCameraNode);

	// Window
    WinApiEngine* pWindowEngine = new WinApiEngine(L"3D World", instance, _iCommandShow);
	pWindowEngine->setWidth(1024);
	pWindowEngine->setHeight(768);
    pWindowEngine->createWindow();
	GazEngine::addEngine(pWindowEngine);

	// Physics
	//GazEngine::addEngine(new BulletEngine(Vector3(0.0f, 0.0f, 0.0f), 1.0f / 60.0f));
	//PhysicsFactory::setInstance(new BulletPhysicsFactory);

	PhysXEngine* pPhysicsEngine = new PhysXEngine(Vector3(0.0f, 0.0f, 0.0f), 1.0f / 60.0f);
	GazEngine::addEngine(pPhysicsEngine);
	PhysicsFactory::setInstance(new PhysXPhysicsFactory(pPhysicsEngine->getPhysics(), pPhysicsEngine->getCooking()));

	// Manual Depth Sorting
	CDepthSortingEngine* pDepthSortingEngine = new CDepthSortingEngine(pRoot, pCameraNode);
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
	gpRenderer->setShader(gpNoDepthTestShader);
	pRenderingEngine->addRenderer(gpRenderer);

	Camera* pCamera = new Direct3D10Camera;
	pCamera->setNode(pCameraNode);
	getTranslation3(pCameraNode->getTransformation()).Y() = 11.0f;
	pCamera->setPerspective(0.25f * Math::PI, 4.0f / 3.0f);
	pRenderingEngine->setCamera(pCamera);

	Light* pLight = new Direct3D10Light("theOnly");
	pLight->setAmbientComponent(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	pLight->setDiffuseComponent(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	pLight->setSpecularComponent(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	pLight->setTranslation(Vector3(0.0f, 100.0f, 0.0f));
	pRenderingEngine->addLight(pLight);

	GazEngine::addEngine(pRenderingEngine);
	ModelFactory::setInstance(new Direct3D10ModelFactory(*pRenderingEngine->getDevice()));

	// Flying Camera
	Entity* pCameraEntity = new Entity;
	Body::Material material;
	material.density = 0.5f;
	material.friction = 0.5f;
	material.restitution = 0.5f;
	Body* pCameraBody = PhysicsFactory::getInstance()->createBody(material, new Sphere(Vector2(), 0.2f), pCameraNode->getTransformation());
	pCameraBody->setMass(1.0f);
	//static_cast<BulletBody*>(pCameraBody)->getBody()->setDamping(0.95f, 0.95f);
	//static_cast<BulletBody*>(pCameraBody)->setNode(pCameraNode);
	static_cast<physx::PxRigidDynamic*>(static_cast<PhysXBody*>(pCameraBody)->getActor())->setAngularDamping(0.95f);
	static_cast<physx::PxRigidDynamic*>(static_cast<PhysXBody*>(pCameraBody)->getActor())->setLinearDamping(0.95f);
	static_cast<PhysXBody*>(pCameraBody)->setNode(pCameraNode);
	pCameraEntity->addComponent(pCameraBody);
	pCameraBody->setEntity(pCameraEntity);
	GazEngine::addEntity(pCameraEntity);

	GazEngine::addEngine(new FlyingCameraEngine(pCamera, 25.0f, pCameraBody));

	// Scene
	CreateControlInfo(*pRoot);
	CreateWorld(*pRoot);
	CreateShapes(*pRoot);

	// Messaging
	Messages::registerRecipient(Events::KEYBOARD_BUTTON, ToggleDepthTest);

	// GO!!!
	GazEngine::setMaxFrameRate(60);
	GazEngine::play();

    return 0;
}
