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

#include <gazengine/engine/DebugEngine.h>
#include <gazengine/Events.h>
#include <gazengine/GazEngine.h>
#include <gazengine/input/KeyboardButtonEvent.h>
#include <gazengine/input/MouseButtonEvent.h>
#include <gazengine/math/Math.h>
#include <gazengine/math/MathFunctions.h>
#include <gazengine/Messages.h>
#include <gazengine/physics/PhysXEngine.h>
#include <gazengine/physics/PhysXPhysicsFactory.h>
#include <gazengine/rendering/FreeGLUTRenderer.h>
#include <gazengine/rendering/OpenGLRenderingEngine.h>
#include <gazengine/rendering/SimpleOpenGLRenderer.h>
#include <gazengine/scene/OpenGLCamera.h>
#include <gazengine/scene/OpenGLLight.h>
#include <gazengine/windowing/FreeGLUTEngine.h>

#include "Scene.h"
#include "SimulationEventCallback.h"

using namespace physx;

PxFilterFlags FilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
						   PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags,
						   const void* constantBlock, PxU32 constantBlockSize);
void FireBall(const void* message);
void Reset(const void* message);
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

const float MAX_BALL_VELOCITY_FACTOR = 10.0f;
const Vector3 BALL_VELOCITY = Vector3(0.0f, 0.0f, -100.0f);
const float BALL_VELOCITY_MULTIPLIER = 10.0f;

const float BLOCK_SIZE = 5.0f;

const float MOUSE_FIRE_ANGLE_FACTOR = 0.4f;

const float TARGET_FPS = 60.0f;

float gfBallStartTime = 0.0f;
DebugEngine* gpDebugEngine = NULL;
SimpleTree* gpDebugNode = NULL;
SimpleTree* gpRoot = NULL;

using namespace physx;

PxFilterFlags FilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
						   PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags,
						   const void* /*constantBlock*/, PxU32 /*constantBlockSize*/)
{
	// let triggers through
	if(PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}

	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	// trigger the contact callback for pairs (A,B) where
	// the filtermask of A contains the ID of B and vice versa.
	if((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
	{
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_THRESHOLD_FORCE_FOUND;
	}

	return PxFilterFlag::eDEFAULT;
}

void FireBall(const void* message)
{
	const MouseButtonEvent* pEvent = static_cast<const MouseButtonEvent*>(message);
	if (pEvent->button != Mouse::LEFT)
	{
		return;
	}

	if (pEvent->buttonState == Button::DOWN)
	{
		gfBallStartTime = GazEngine::getTotalTime();
	}

	if (pEvent->buttonState == Button::UP)
	{
		Entity* pBall = CreateBall(BLOCK_SIZE, *gpRoot);
		float fVelocityFactor = (GazEngine::getTotalTime() - gfBallStartTime) * BALL_VELOCITY_MULTIPLIER;
		fVelocityFactor = min(fVelocityFactor, MAX_BALL_VELOCITY_FACTOR);

		pBall->getSingleComponent<PhysXBody>()->setLinearVelocity(BALL_VELOCITY * fVelocityFactor +
			Vector3((pEvent->x - 400.0f) * MOUSE_FIRE_ANGLE_FACTOR,
			(pEvent->y - 300.0f) * -MOUSE_FIRE_ANGLE_FACTOR, 0.0f));
	}
}

void Reset(const void* message)
{
	const KeyboardButtonEvent* pEvent = static_cast<const KeyboardButtonEvent*>(message);
	if (pEvent->button != Keyboard::R || pEvent->buttonState != Button::UP)
	{
		return;
	}

	GazEngine::removeAllEntities();
	gpDebugEngine->restart();
	gpDebugNode->setModel(gpDebugEngine->getDebugText());

	CreateFloor(*gpRoot);
	CreateWall(BLOCK_SIZE, *gpRoot);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Debugging / Free GLUT
	gpDebugEngine = new DebugEngine(new FreeGLUTEngine("Brick Wall"));
	GazEngine::addEngine(gpDebugEngine);

	// PhysX
	PhysXEngine* pPhysicsEngine = new PhysXEngine(Vector3(0.0f, -10.0f, 0.0f), 1.0f / TARGET_FPS);
	pPhysicsEngine->setSimulationFilterShader(FilterShader);
	PhysicsFactory::setInstance(new PhysXPhysicsFactory(pPhysicsEngine->getPhysics()));
	GazEngine::addEngine(pPhysicsEngine);

	// Open GL
	OpenGLRenderingEngine* pRenderingEngine = new OpenGLRenderingEngine;
	pRenderingEngine->addRenderer(new FreeGLUTRenderer(new SimpleOpenGLRenderer));
	pRenderingEngine->setClearingColour(Vector4(0.5f, 1.0f, 1.0f, 1.0f)); // Sky
	GazEngine::addEngine(pRenderingEngine);

	Camera* pCamera = new OpenGLCamera;
	pCamera->setPerspective(25.0f * Math::PI, 4.0f / 3.0f); // 4:3 aspect ratio
	pRenderingEngine->setCamera(pCamera);

	pRenderingEngine->addLight(new OpenGLLight("theOnlyLight"));

	// Scene
	gpRoot = new SimpleTree;
	GazEngine::addWorldRepresentation(gpRoot);
	CreateFloor(*gpRoot);
	CreateWall(BLOCK_SIZE, *gpRoot);
	pCamera->setNode(gpRoot);
	pPhysicsEngine->setSimulationEventCallback(new SimulationEventCallback(gpRoot));
	pRenderingEngine->setTree(gpRoot);

	// Debugging output
	gpDebugNode = new SimpleTree;
	getTranslation3(gpDebugNode->getTransformation()).X() = -1.0f;
	getTranslation3(gpDebugNode->getTransformation()).Y() = 0.75f;
	getTranslation3(gpDebugNode->getTransformation()).Z() = -1.0f;
	gpDebugNode->setModel(gpDebugEngine->getDebugText());
	gpRoot->addChild(gpDebugNode);

	// Messaging
	Messages::registerRecipient(Events::KEYBOARD_BUTTON, Reset);
	Messages::registerRecipient(Events::MOUSE_BUTTON, FireBall);

	// GO!!!
	GazEngine::setMaxFrameRate(static_cast<unsigned short>(TARGET_FPS));
	GazEngine::play();

    return 0;
}
