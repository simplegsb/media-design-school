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
#include <gazengine/gdi/rendering/GDIRenderer.h>
#include <gazengine/gdi/rendering/GDIRenderingEngine.h>
#include <gazengine/input/MouseButtonEvent.h>
#include <gazengine/Messages.h>
#include <gazengine/physics/SimpleBody.h>
#include <gazengine/physics/SimplePhysicsEngine.h>
#include <gazengine/physics/SpringConstraint.h>
#include <gazengine/physics/VerletIntegrator.h>
#include <gazengine/winapi/WinApiEngine.h>

#include "FloorConstraint.h"
#include "WindowConstraint.h"

using namespace std;

const float BLOB_ELASTICITY = 500.0f;

const float BLOB_POINT_MASSES = 5.0f; // This has no effect right now...

const float FLOOR_LEVEL = 400.0f;

const Vector2 GRAVITY = Vector2(0.0f, 10.0f);

Entity* g_pBlob = NULL;

Entity* CreateBlob(const Vector2& position)
{
	Entity* pBlob = new Entity;

	//CFloorConstraint* pHardConstraint = new CFloorConstraint(FLOOR_LEVEL);
	CWindowConstraint* pHardConstraint = new CWindowConstraint;
	pHardConstraint->setEntity(pBlob);

	Body* pBodyCenter = new SimpleBody(Body::Material(), NULL, position);
	pBodyCenter->setDynamic(true);
	pBodyCenter->setMass(BLOB_POINT_MASSES);
	pBlob->addComponent(pBodyCenter);
	pBodyCenter->setEntity(pBlob);
	pHardConstraint->AddBody(pBodyCenter);

	Body* pBodyInner0 = new SimpleBody(Body::Material(), NULL, position + Vector2(0.0f, -50.0f));
	pBodyInner0->setDynamic(true);
	pBodyInner0->setMass(BLOB_POINT_MASSES);
	pBlob->addComponent(pBodyInner0);
	pBodyInner0->setEntity(pBlob);
	pHardConstraint->AddBody(pBodyInner0);
	Body* pBodyInner1 = new SimpleBody(Body::Material(), NULL, position + Vector2(35.0f, -35.0f));
	pBodyInner1->setMass(BLOB_POINT_MASSES);
	pBodyInner1->setDynamic(true);
	pBlob->addComponent(pBodyInner1);
	pBodyInner1->setEntity(pBlob);
	pHardConstraint->AddBody(pBodyInner1);
	Body* pBodyInner2 = new SimpleBody(Body::Material(), NULL, position + Vector2(50.0f, 0.0f));
	pBodyInner2->setMass(BLOB_POINT_MASSES);
	pBodyInner2->setDynamic(true);
	pBlob->addComponent(pBodyInner2);
	pBodyInner2->setEntity(pBlob);
	pHardConstraint->AddBody(pBodyInner2);
	Body* pBodyInner3 = new SimpleBody(Body::Material(), NULL, position + Vector2(35.0f, 35.0f));
	pBodyInner3->setMass(BLOB_POINT_MASSES);
	pBodyInner3->setDynamic(true);
	pBlob->addComponent(pBodyInner3);
	pBodyInner3->setEntity(pBlob);
	pHardConstraint->AddBody(pBodyInner3);
	Body* pBodyInner4 = new SimpleBody(Body::Material(), NULL, position + Vector2(0.0f, 50.0f));
	pBodyInner4->setMass(BLOB_POINT_MASSES);
	pBodyInner4->setDynamic(true);
	pBlob->addComponent(pBodyInner4);
	pBodyInner4->setEntity(pBlob);
	pHardConstraint->AddBody(pBodyInner4);
	Body* pBodyInner5 = new SimpleBody(Body::Material(), NULL, position + Vector2(-35.0f, 35.0f));
	pBodyInner5->setMass(BLOB_POINT_MASSES);
	pBodyInner5->setDynamic(true);
	pBlob->addComponent(pBodyInner5);
	pBodyInner5->setEntity(pBlob);
	pHardConstraint->AddBody(pBodyInner5);
	Body* pBodyInner6 = new SimpleBody(Body::Material(), NULL, position + Vector2(-50.0f, 0.0f));
	pBodyInner6->setMass(BLOB_POINT_MASSES);
	pBodyInner6->setDynamic(true);
	pBlob->addComponent(pBodyInner6);
	pBodyInner6->setEntity(pBlob);
	pHardConstraint->AddBody(pBodyInner6);
	Body* pBodyInner7 = new SimpleBody(Body::Material(), NULL, position + Vector2(-35.0f, -35.0f));
	pBodyInner7->setMass(BLOB_POINT_MASSES);
	pBodyInner7->setDynamic(true);
	pBlob->addComponent(pBodyInner7);
	pBodyInner7->setEntity(pBlob);
	pHardConstraint->AddBody(pBodyInner7);

	SpringConstraint* pSpringCenterToInner0 = new SpringConstraint(BLOB_ELASTICITY, pBodyCenter, pBodyInner0);
	pSpringCenterToInner0->setDebugRendered(true);
	pBlob->addComponent(pSpringCenterToInner0);
	pSpringCenterToInner0->setEntity(pBlob);
	SpringConstraint* pSpringCenterToInner1 = new SpringConstraint(BLOB_ELASTICITY, pBodyCenter, pBodyInner1);
	pSpringCenterToInner1->setDebugRendered(true);
	pBlob->addComponent(pSpringCenterToInner1);
	pSpringCenterToInner1->setEntity(pBlob);
	SpringConstraint* pSpringCenterToInner2 = new SpringConstraint(BLOB_ELASTICITY, pBodyCenter, pBodyInner2);
	pSpringCenterToInner2->setDebugRendered(true);
	pBlob->addComponent(pSpringCenterToInner2);
	pSpringCenterToInner2->setEntity(pBlob);
	SpringConstraint* pSpringCenterToInner3 = new SpringConstraint(BLOB_ELASTICITY, pBodyCenter, pBodyInner3);
	pSpringCenterToInner3->setDebugRendered(true);
	pBlob->addComponent(pSpringCenterToInner3);
	pSpringCenterToInner3->setEntity(pBlob);
	SpringConstraint* pSpringCenterToInner4 = new SpringConstraint(BLOB_ELASTICITY, pBodyCenter, pBodyInner4);
	pSpringCenterToInner4->setDebugRendered(true);
	pBlob->addComponent(pSpringCenterToInner4);
	pSpringCenterToInner4->setEntity(pBlob);
	SpringConstraint* pSpringCenterToInner5 = new SpringConstraint(BLOB_ELASTICITY, pBodyCenter, pBodyInner5);
	pSpringCenterToInner5->setDebugRendered(true);
	pBlob->addComponent(pSpringCenterToInner5);
	pSpringCenterToInner5->setEntity(pBlob);
	SpringConstraint* pSpringCenterToInner6 = new SpringConstraint(BLOB_ELASTICITY, pBodyCenter, pBodyInner6);
	pSpringCenterToInner6->setDebugRendered(true);
	pBlob->addComponent(pSpringCenterToInner6);
	pSpringCenterToInner6->setEntity(pBlob);
	SpringConstraint* pSpringCenterToInner7 = new SpringConstraint(BLOB_ELASTICITY, pBodyCenter, pBodyInner7);
	pSpringCenterToInner7->setDebugRendered(true);
	pBlob->addComponent(pSpringCenterToInner7);
	pSpringCenterToInner7->setEntity(pBlob);

	SpringConstraint* pSpringInner0ToInner1 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner0, pBodyInner1);
	pSpringInner0ToInner1->setDebugRendered(true);
	pBlob->addComponent(pSpringInner0ToInner1);
	pSpringInner0ToInner1->setEntity(pBlob);
	SpringConstraint* pSpringInner1ToInner2 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner1, pBodyInner2);
	pSpringInner1ToInner2->setDebugRendered(true);
	pBlob->addComponent(pSpringInner1ToInner2);
	pSpringInner1ToInner2->setEntity(pBlob);
	SpringConstraint* pSpringInner2ToInner3 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner2, pBodyInner3);
	pSpringInner2ToInner3->setDebugRendered(true);
	pBlob->addComponent(pSpringInner2ToInner3);
	pSpringInner2ToInner3->setEntity(pBlob);
	SpringConstraint* pSpringInner3ToInner4 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner3, pBodyInner4);
	pSpringInner3ToInner4->setDebugRendered(true);
	pBlob->addComponent(pSpringInner3ToInner4);
	pSpringInner3ToInner4->setEntity(pBlob);
	SpringConstraint* pSpringInner4ToInner5 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner4, pBodyInner5);
	pSpringInner4ToInner5->setDebugRendered(true);
	pBlob->addComponent(pSpringInner4ToInner5);
	pSpringInner4ToInner5->setEntity(pBlob);
	SpringConstraint* pSpringInner5ToInner6 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner5, pBodyInner6);
	pSpringInner5ToInner6->setDebugRendered(true);
	pBlob->addComponent(pSpringInner5ToInner6);
	pSpringInner5ToInner6->setEntity(pBlob);
	SpringConstraint* pSpringInner6ToInner7 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner6, pBodyInner7);
	pSpringInner6ToInner7->setDebugRendered(true);
	pBlob->addComponent(pSpringInner6ToInner7);
	pSpringInner6ToInner7->setEntity(pBlob);
	SpringConstraint* pSpringInner7ToInner0 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner7, pBodyInner0);
	pSpringInner7ToInner0->setDebugRendered(true);
	pBlob->addComponent(pSpringInner7ToInner0);
	pSpringInner7ToInner0->setEntity(pBlob);

	Body* pBodyOuter0 = new SimpleBody(Body::Material(), NULL, position + Vector2(0.0f, -60.0f));
	pBodyOuter0->setMass(BLOB_POINT_MASSES);
	pBodyOuter0->setDynamic(true);
	pBlob->addComponent(pBodyOuter0);
	pBodyOuter0->setEntity(pBlob);
	pHardConstraint->AddBody(pBodyOuter0);
	Body* pBodyOuter1 = new SimpleBody(Body::Material(), NULL, position + Vector2(42.0f, -42.0f));
	pBodyOuter1->setMass(BLOB_POINT_MASSES);
	pBodyOuter1->setDynamic(true);
	pBlob->addComponent(pBodyOuter1);
	pBodyOuter1->setEntity(pBlob);
	pHardConstraint->AddBody(pBodyOuter1);
	Body* pBodyOuter2 = new SimpleBody(Body::Material(), NULL, position + Vector2(60.0f, 0.0f));
	pBodyOuter2->setMass(BLOB_POINT_MASSES);
	pBodyOuter2->setDynamic(true);
	pBlob->addComponent(pBodyOuter2);
	pBodyOuter2->setEntity(pBlob);
	pHardConstraint->AddBody(pBodyOuter2);
	Body* pBodyOuter3 = new SimpleBody(Body::Material(), NULL, position + Vector2(42.0f, 42.0f));
	pBodyOuter3->setMass(BLOB_POINT_MASSES);
	pBodyOuter3->setDynamic(true);
	pBlob->addComponent(pBodyOuter3);
	pBodyOuter3->setEntity(pBlob);
	pHardConstraint->AddBody(pBodyOuter3);
	Body* pBodyOuter4 = new SimpleBody(Body::Material(), NULL, position + Vector2(0.0f, 60.0f));
	pBodyOuter4->setMass(BLOB_POINT_MASSES);
	pBodyOuter4->setDynamic(true);
	pBlob->addComponent(pBodyOuter4);
	pBodyOuter4->setEntity(pBlob);
	pHardConstraint->AddBody(pBodyOuter4);
	Body* pBodyOuter5 = new SimpleBody(Body::Material(), NULL, position + Vector2(-42.0f, 42.0f));
	pBodyOuter5->setMass(BLOB_POINT_MASSES);
	pBodyOuter5->setDynamic(true);
	pBlob->addComponent(pBodyOuter5);
	pBodyOuter5->setEntity(pBlob);
	pHardConstraint->AddBody(pBodyOuter5);
	Body* pBodyOuter6 = new SimpleBody(Body::Material(), NULL, position + Vector2(-60.0f, 0.0f));
	pBodyOuter6->setMass(BLOB_POINT_MASSES);
	pBodyOuter6->setDynamic(true);
	pBlob->addComponent(pBodyOuter6);
	pBodyOuter6->setEntity(pBlob);
	pHardConstraint->AddBody(pBodyOuter6);
	Body* pBodyOuter7 = new SimpleBody(Body::Material(), NULL, position + Vector2(-42.0f, -42.0f));
	pBodyOuter7->setMass(BLOB_POINT_MASSES);
	pBodyOuter7->setDynamic(true);
	pBlob->addComponent(pBodyOuter7);
	pBodyOuter7->setEntity(pBlob);
	pHardConstraint->AddBody(pBodyOuter7);

	SpringConstraint* pSpringInner0ToOuter0 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner0, pBodyOuter0);
	pSpringInner0ToOuter0->setDebugRendered(true);
	pBlob->addComponent(pSpringInner0ToOuter0);
	pSpringInner0ToOuter0->setEntity(pBlob);
	SpringConstraint* pSpringInner1ToOuter1 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner1, pBodyOuter1);
	pSpringInner1ToOuter1->setDebugRendered(true);
	pBlob->addComponent(pSpringInner1ToOuter1);
	pSpringInner1ToOuter1->setEntity(pBlob);
	SpringConstraint* pSpringInner2ToOuter2 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner2, pBodyOuter2);
	pSpringInner2ToOuter2->setDebugRendered(true);
	pBlob->addComponent(pSpringInner2ToOuter2);
	pSpringInner2ToOuter2->setEntity(pBlob);
	SpringConstraint* pSpringInner3ToOuter3 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner3, pBodyOuter3);
	pSpringInner3ToOuter3->setDebugRendered(true);
	pBlob->addComponent(pSpringInner3ToOuter3);
	pSpringInner3ToOuter3->setEntity(pBlob);
	SpringConstraint* pSpringInner4ToOuter4 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner4, pBodyOuter4);
	pSpringInner4ToOuter4->setDebugRendered(true);
	pBlob->addComponent(pSpringInner4ToOuter4);
	pSpringInner4ToOuter4->setEntity(pBlob);
	SpringConstraint* pSpringInner5ToOuter5 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner5, pBodyOuter5);
	pSpringInner5ToOuter5->setDebugRendered(true);
	pBlob->addComponent(pSpringInner5ToOuter5);
	pSpringInner5ToOuter5->setEntity(pBlob);
	SpringConstraint* pSpringInner6ToOuter6 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner6, pBodyOuter6);
	pSpringInner6ToOuter6->setDebugRendered(true);
	pBlob->addComponent(pSpringInner6ToOuter6);
	pSpringInner6ToOuter6->setEntity(pBlob);
	SpringConstraint* pSpringInner7ToOuter7 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner7, pBodyOuter7);
	pSpringInner7ToOuter7->setDebugRendered(true);
	pBlob->addComponent(pSpringInner7ToOuter7);
	pSpringInner7ToOuter7->setEntity(pBlob);

	SpringConstraint* pSpringInner0ToOuter1 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner0, pBodyOuter1);
	pSpringInner0ToOuter1->setDebugRendered(true);
	pBlob->addComponent(pSpringInner0ToOuter1);
	pSpringInner0ToOuter1->setEntity(pBlob);
	SpringConstraint* pSpringInner1ToOuter2 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner1, pBodyOuter2);
	pSpringInner1ToOuter2->setDebugRendered(true);
	pBlob->addComponent(pSpringInner1ToOuter2);
	pSpringInner1ToOuter2->setEntity(pBlob);
	SpringConstraint* pSpringInner2ToOuter3 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner2, pBodyOuter3);
	pSpringInner2ToOuter3->setDebugRendered(true);
	pBlob->addComponent(pSpringInner2ToOuter3);
	pSpringInner2ToOuter3->setEntity(pBlob);
	SpringConstraint* pSpringInner3ToOuter4 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner3, pBodyOuter4);
	pSpringInner3ToOuter4->setDebugRendered(true);
	pBlob->addComponent(pSpringInner3ToOuter4);
	pSpringInner3ToOuter4->setEntity(pBlob);
	SpringConstraint* pSpringInner4ToOuter5 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner4, pBodyOuter5);
	pSpringInner4ToOuter5->setDebugRendered(true);
	pBlob->addComponent(pSpringInner4ToOuter5);
	pSpringInner4ToOuter5->setEntity(pBlob);
	SpringConstraint* pSpringInner5ToOuter6 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner5, pBodyOuter6);
	pSpringInner5ToOuter6->setDebugRendered(true);
	pBlob->addComponent(pSpringInner5ToOuter6);
	pSpringInner5ToOuter6->setEntity(pBlob);
	SpringConstraint* pSpringInner6ToOuter7 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner6, pBodyOuter7);
	pSpringInner6ToOuter7->setDebugRendered(true);
	pBlob->addComponent(pSpringInner6ToOuter7);
	pSpringInner6ToOuter7->setEntity(pBlob);
	SpringConstraint* pSpringInner7ToOuter0 = new SpringConstraint(BLOB_ELASTICITY, pBodyInner7, pBodyOuter0);
	pSpringInner7ToOuter0->setDebugRendered(true);
	pBlob->addComponent(pSpringInner7ToOuter0);
	pSpringInner7ToOuter0->setEntity(pBlob);

	SpringConstraint* pSpringOuter0ToOuter1 = new SpringConstraint(BLOB_ELASTICITY, pBodyOuter0, pBodyOuter1);
	pSpringOuter0ToOuter1->setDebugRendered(true);
	pBlob->addComponent(pSpringOuter0ToOuter1);
	pSpringOuter0ToOuter1->setEntity(pBlob);
	SpringConstraint* pSpringOuter1ToOuter2 = new SpringConstraint(BLOB_ELASTICITY, pBodyOuter1, pBodyOuter2);
	pSpringOuter1ToOuter2->setDebugRendered(true);
	pBlob->addComponent(pSpringOuter1ToOuter2);
	pSpringOuter1ToOuter2->setEntity(pBlob);
	SpringConstraint* pSpringOuter2ToOuter3 = new SpringConstraint(BLOB_ELASTICITY, pBodyOuter2, pBodyOuter3);
	pSpringOuter2ToOuter3->setDebugRendered(true);
	pBlob->addComponent(pSpringOuter2ToOuter3);
	pSpringOuter2ToOuter3->setEntity(pBlob);
	SpringConstraint* pSpringOuter3ToOuter4 = new SpringConstraint(BLOB_ELASTICITY, pBodyOuter3, pBodyOuter4);
	pSpringOuter3ToOuter4->setDebugRendered(true);
	pBlob->addComponent(pSpringOuter3ToOuter4);
	pSpringOuter3ToOuter4->setEntity(pBlob);
	SpringConstraint* pSpringOuter4ToOuter5 = new SpringConstraint(BLOB_ELASTICITY, pBodyOuter4, pBodyOuter5);
	pSpringOuter4ToOuter5->setDebugRendered(true);
	pBlob->addComponent(pSpringOuter4ToOuter5);
	pSpringOuter4ToOuter5->setEntity(pBlob);
	SpringConstraint* pSpringOuter5ToOuter6 = new SpringConstraint(BLOB_ELASTICITY, pBodyOuter5, pBodyOuter6);
	pSpringOuter5ToOuter6->setDebugRendered(true);
	pBlob->addComponent(pSpringOuter5ToOuter6);
	pSpringOuter5ToOuter6->setEntity(pBlob);
	SpringConstraint* pSpringOuter6ToOuter7 = new SpringConstraint(BLOB_ELASTICITY, pBodyOuter6, pBodyOuter7);
	pSpringOuter6ToOuter7->setDebugRendered(true);
	pBlob->addComponent(pSpringOuter6ToOuter7);
	pSpringOuter6ToOuter7->setEntity(pBlob);
	SpringConstraint* pSpringOuter7ToOuter0 = new SpringConstraint(BLOB_ELASTICITY, pBodyOuter7, pBodyOuter0);
	pSpringOuter7ToOuter0->setDebugRendered(true);
	pBlob->addComponent(pSpringOuter7ToOuter0);
	pSpringOuter7ToOuter0->setEntity(pBlob);

	// Add the hard constraint last!
	pBlob->addComponent(pHardConstraint);

	return pBlob;
}

void OnMouseClick(const void* message)
{
	const MouseButtonEvent* pEvent = static_cast<const MouseButtonEvent*>(message);

	if (pEvent->button == Mouse::LEFT && pEvent->buttonState == Button::UP)
	{
		vector<Body*> bodies = g_pBlob->getComponents<Body>();
		Vector2 mousePosition(pEvent->x, pEvent->y);
		Vector2 force = mousePosition - bodies[0]->getPosition();
		force.normalise();
		force *= 5000.0f;

		for (unsigned int uiIndex = 0; uiIndex < bodies.size(); uiIndex++)
		{
			bodies[uiIndex]->applyForce(force, bodies[uiIndex]->getPosition());
		}
	}
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int iCommandShow)
{
    WinApiEngine* pWindowEngine = new WinApiEngine(L"Day 008.2", instance, iCommandShow);
    pWindowEngine->createWindow();
	GazEngine::addEngine(pWindowEngine);

	SimplePhysicsEngine* pPhysicsEngine = new SimplePhysicsEngine(GRAVITY);
	pPhysicsEngine->setIntegrator(new VerletIntegrator(0.99f));
	GazEngine::addEngine(pPhysicsEngine);

	GDIRenderingEngine* pRenderingEngine = new GDIRenderingEngine(pWindowEngine->getWindow());
	pRenderingEngine->addRenderer(new GDIRenderer);
	GazEngine::addEngine(pRenderingEngine);

	g_pBlob = CreateBlob(Vector2(400.0f, 300.0f));
	GazEngine::addEntity(g_pBlob);

	Messages::registerRecipient(Events::MOUSE_BUTTON, OnMouseClick);

	GazEngine::setMaxFrameRate(60);
	GazEngine::play();

    return 0;
}
