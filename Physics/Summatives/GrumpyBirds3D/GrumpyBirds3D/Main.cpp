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

#include <vld.h>

#include <gazengine/engine/DebugEngine.h>
#include <gazengine/GazEngine.h>
#include <gazengine/math/Math.h>
#include <gazengine/math/MathFunctions.h>
#include <gazengine/model/SimpleModelFactory.h>
#include <gazengine/physics/PhysXEngine.h>
#include <gazengine/physics/PhysXPhysicsFactory.h>
#include <gazengine/rendering/FreeGLUTRenderer.h>
#include <gazengine/rendering/OpenGLRenderingEngine.h>
#include <gazengine/rendering/SimpleOpenGLRenderer.h>
#include <gazengine/scene/OpenGLCamera.h>
#include <gazengine/scene/OpenGLLight.h>
#include <gazengine/windowing/FreeGLUTEngine.h>

#include "BirdEngine.h"
#include "FilterShader.h"
#include "GrumpyEngine.h"
#include "SimulationEventCallback.h"
#include "SlingshotEngine.h"

const unsigned short TARGET_FPS = 60;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// World
	SimpleTree* pWorld = new SimpleTree;
	GazEngine::addWorldRepresentation(pWorld);

	// Window
	GazEngine::addEngine(new FreeGLUTEngine("Grumpy Birds 3D", 1024, 768));

	// Gameplay
	GazEngine::addEngine(new CGrumpyEngine);
	GazEngine::addEngine(new CSlingshotEngine);
	GazEngine::addEngine(new CBirdEngine(10.0f, 5.0f));

	// Physics
	PhysXEngine* pPhysicsEngine = new PhysXEngine(Vector3(0.0f, -10.0f, 0.0f), 1.0f / TARGET_FPS);
	pPhysicsEngine->setSimulationEventCallback(new SimulationEventCallback);
	pPhysicsEngine->setSimulationFilterShader(FilterShader);
	PhysicsFactory::setInstance(new PhysXPhysicsFactory(pPhysicsEngine->getPhysics(), pPhysicsEngine->getCooking()));
	GazEngine::addEngine(pPhysicsEngine);

	// Rendering
	OpenGLRenderingEngine* pRenderingEngine = new OpenGLRenderingEngine;
	pRenderingEngine->addRenderer(new FreeGLUTRenderer(new SimpleOpenGLRenderer));
	pRenderingEngine->setClearingColour(Vector4(0.5f, 1.0f, 1.0f, 1.0f)); // Sky
	pRenderingEngine->setHeight(768);
	pRenderingEngine->setTree(pWorld);
	pRenderingEngine->setWidth(1024);
	ModelFactory::setInstance(new SimpleModelFactory);
	GazEngine::addEngine(pRenderingEngine);

	Camera* pCamera = new OpenGLCamera;
	pCamera->setPerspective(25.0f * Math::PI, 4.0f / 3.0f); // 4:3 aspect ratio
	pRenderingEngine->setCamera(pCamera);

	SimpleTree* pCameraNode = new SimpleTree;
	setTranslation(pCameraNode->getTransformation(), Vector3(0.0f, 0.0f, 10.0f));
	pCamera->setNode(pCameraNode);
	GazEngine::addToWorld(pCameraNode);

	pRenderingEngine->addLight(new OpenGLLight("theOnlyLight"));

	// Debugging
	//GazEngine::addEngine(new DebugEngine);

	// GO!!!
	GazEngine::setMaxFrameRate(TARGET_FPS);
	GazEngine::play();

	// Cleanup.
	delete ModelFactory::getInstance();
	delete PhysicsFactory::getInstance();

    return 0;
}
