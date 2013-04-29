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
#include <windows.h>

#include <gazengine/Events.h>
#include <gazengine/GazEngine.h>
#include <gazengine/input/MouseButtonEvent.h>
#include <gazengine/math/Math.h>
#include <gazengine/math/MathFunctions.h>
#include <gazengine/Messages.h>
#include <gazengine/model/Plane.h>
#include <gazengine/model/shape/Cube.h>
#include <gazengine/model/shape/Sphere.h>
#include <gazengine/physics/BulletEngine.h>
#include <gazengine/physics/BulletPhysicsFactory.h>
#include <gazengine/physics/PhysXEngine.h>
#include <gazengine/physics/PhysXPhysicsFactory.h>
#include <gazengine/rendering/FreeGLUTRenderer.h>
#include <gazengine/rendering/OpenGLRenderingEngine.h>
#include <gazengine/rendering/SimpleOpenGLRenderer.h>
#include <gazengine/scene/OpenGLCamera.h>
#include <gazengine/scene/OpenGLLight.h>
#include <gazengine/windowing/FreeGLUTEngine.h>

using namespace physx;

void CreateFloor();
void CreateWall();
void CreateWallBlock(const Vector3& position);
void FireBall(const void* message);
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

const Vector4 BALL_COLOUR(1.0f, 0.0f, 0.0f, 1.0f);
const Vector3 BALL_POSITION = Vector3(0.0f, 0.0f, 0.0f);
const float BALL_SIZE = 5.0f;
const Vector3 BALL_VELOCITY = Vector3(0.0f, 0.0f, -100.0f);
const Vector4 BLOCK_COLOUR(0.0f, 0.0f, 1.0f, 1.0f);
const float BLOCK_SIZE = 5.0f;
const float DISTANCE_TO_WALL = 75.0f;
const Vector3 FLOOR_POSITION = Vector3(0.0f, -20.0f, 0.0f);

SimpleTree* gpRoot = NULL;
PxPhysics* gpPhysics = NULL;

void CreateFloor()
{
	Entity* pFloor = new Entity;

	Plane* pPlane = new Plane(Vector3(0.0f, 1.0f, 0.0f), FLOOR_POSITION);
	pFloor->addComponent(pPlane);
	pPlane->setEntity(pFloor);

	Body::Material material;
	material.density = 0.5f;
	material.friction = 0.5f;
	material.restitution = 0.5f;
	Body* pBody = PhysicsFactory::getInstance()->createBody(material, pPlane, FLOOR_POSITION, false);
	pFloor->addComponent(pBody);
	pBody->setEntity(pFloor);

	SimpleTree* pNode = new SimpleTree;
	pNode->setModel(pPlane);
	pBody->setNode(pNode);
	gpRoot->addChild(pNode);

	GazEngine::addEntity(pFloor);
}

void CreateWall()
{
	// Layer 1
	CreateWallBlock(Vector3(-18.0f, -10.0f, -DISTANCE_TO_WALL));
	CreateWallBlock(Vector3(-6.0f, -10.0f, -DISTANCE_TO_WALL));
	CreateWallBlock(Vector3(6.0f, -10.0f, -DISTANCE_TO_WALL));
	CreateWallBlock(Vector3(18.0f, -10.0f, -DISTANCE_TO_WALL));

	// Layer 2
	CreateWallBlock(Vector3(-12.0f, 0.0f, -DISTANCE_TO_WALL));
	CreateWallBlock(Vector3(0.0f, 0.0f, -DISTANCE_TO_WALL));
	CreateWallBlock(Vector3(12.0f, 0.0f, -DISTANCE_TO_WALL));

	// Layer 3
	CreateWallBlock(Vector3(-6.0f, 10.0f, -DISTANCE_TO_WALL));
	CreateWallBlock(Vector3(6.0f, 10.0f, -DISTANCE_TO_WALL));

	// Layer 4
	CreateWallBlock(Vector3(0.0f, 20.0f, -DISTANCE_TO_WALL));
}

void CreateWallBlock(const Vector3& position)
{
	Entity* pWallBlock = new Entity;

	// Layer 1
	Cube* pCube = new Cube(Vector2(), BLOCK_SIZE);
	pCube->setColour(BLOCK_COLOUR);
	pWallBlock->addComponent(pCube);
	pCube->setEntity(pWallBlock);

	Body::Material material;
	material.density = 0.5f;
	material.friction = 0.5f;
	material.restitution = 0.5f;
	Body* pBody = PhysicsFactory::getInstance()->createBody(material, pCube, position, true);
	pBody->setMass(1.0f);
	pWallBlock->addComponent(pBody);
	pBody->setEntity(pWallBlock);

	SimpleTree* pNode = new SimpleTree;
	setTranslation(pNode->getTransformation(), position);
	pNode->setModel(pCube);
	pBody->setNode(pNode);
	gpRoot->addChild(pNode);

	GazEngine::addEntity(pWallBlock);
}

void FireBall(const void* message)
{
	const MouseButtonEvent* pEvent = static_cast<const MouseButtonEvent*>(message);
	if (pEvent->button != Mouse::LEFT || pEvent->buttonState != Button::UP)
	{
		return;
	}

	Entity* pBall = new Entity;

	Sphere* pSphere = new Sphere(Vector2(), BLOCK_SIZE);
	pSphere->setColour(BALL_COLOUR);
	pSphere->setLevelOfDetail(10);
	pBall->addComponent(pSphere);
	pSphere->setEntity(pBall);

	Body::Material material;
	material.density = 0.5f;
	material.friction = 0.5f;
	material.restitution = 0.5f;
	Body* pBody = PhysicsFactory::getInstance()->createBody(material, pSphere, BALL_POSITION, true);
	pBody->setLinearVelocity(BALL_VELOCITY + Vector3((pEvent->x - 400.0f) * 0.2f, (pEvent->y - 300.0f) * -0.2f, 0.0f));
	pBody->setMass(1.0f);
	pBall->addComponent(pBody);
	pBody->setEntity(pBall);

	SimpleTree* pNode = new SimpleTree;
	setTranslation(pNode->getTransformation(), BALL_POSITION);
	pNode->setModel(pSphere);
	pBody->setNode(pNode);
	gpRoot->addChild(pNode);

	GazEngine::addEntity(pBall);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Free GLUT
	GazEngine::addEngine(new FreeGLUTEngine("Day 009.1"));

	// Open GL
	OpenGLRenderingEngine* pRenderingEngine = new OpenGLRenderingEngine;
	pRenderingEngine->addRenderer(new FreeGLUTRenderer(new SimpleOpenGLRenderer));
	GazEngine::addEngine(pRenderingEngine);

	Camera* pCamera = new OpenGLCamera;
	pCamera->setPerspective(25.0f * Math::PI, 4.0f / 3.0f); // 4:3 aspect ratio
	pRenderingEngine->setCamera(pCamera);

	pRenderingEngine->addLight(new OpenGLLight("theOnlyLight"));

	// Bullet
	GazEngine::addEngine(new BulletEngine(Vector3(0.0f, -10.0f, 0.0f)));
	PhysicsFactory::setInstance(new BulletPhysicsFactory);

	// PhysX
	//PhysXEngine* pPhysicsEngine = new PhysXEngine(Vector3(0.0f, -10.0f, 0.0f));
	//PhysicsFactory::setInstance(new PhysXPhysicsFactory(pPhysicsEngine->getPhysics()));
	//GazEngine::addEngine(pPhysicsEngine);

	// Scene
	gpRoot = new SimpleTree;
	CreateFloor();
	CreateWall();
	pCamera->setNode(gpRoot);
	pRenderingEngine->setTree(gpRoot);

	Messages::registerRecipient(Events::MOUSE_BUTTON, FireBall);

	// GO!!!
	GazEngine::setMaxFrameRate(60);
	GazEngine::play();

	delete gpRoot;

    return 0;
}
