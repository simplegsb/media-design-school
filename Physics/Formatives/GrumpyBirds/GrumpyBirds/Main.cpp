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
// $Date: 2013-01-29 11:58:25 +1300 (Tue, 29 Jan 2013) $
// $Rev: 1129 $
//
#include <gazengine/GazEngine.h>
#include <gazengine/gdi/rendering/GDIRenderer.h>
#include <gazengine/gdi/rendering/GDIRenderingEngine.h>
#include <gazengine/winapi/WinApiEngine.h>

#include "Box2DPhysicsEngine.h"
#include "GrumpyConstants.h"
#include "GrumpyFactory.h"
#include "GrumpyEngine.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int iCommandShow)
{
    WinApiEngine* pWindowEngine = new WinApiEngine(L"Grumpy Birds", instance, iCommandShow);
	pWindowEngine->setHeight(WINDOW_HEIGHT);
	pWindowEngine->setWidth(WINDOW_WIDTH);
    pWindowEngine->createWindow();
	pWindowEngine->init();
	GazEngine::addEngine(pWindowEngine);

	CBox2DGDIDrawer physicsDrawer(pWindowEngine->getWindow());
	physicsDrawer.SetFlags(b2Draw::e_shapeBit);
	CBox2DPhysicsEngine* pPhysicsEngine = new CBox2DPhysicsEngine(Vector2(0.0f, 7.0f), &physicsDrawer);
	//CBox2DPhysicsEngine* pPhysicsEngine = new CBox2DPhysicsEngine(Vector2(0.0f, 7.0f), NULL);
	GazEngine::addEngine(pPhysicsEngine);

	CGrumpyEngine* pGrumpyEngine = new CGrumpyEngine;
	pGrumpyEngine->addBird(GrumpyFactory::CreateBoostBird(BIRD_START_POSITION, BOOST_BIRD_RADIUS));
	pGrumpyEngine->addBird(GrumpyFactory::CreateTripleBird(BIRD_START_POSITION, TRIPLE_BIRD_RADIUS));
	pGrumpyEngine->addBird(GrumpyFactory::CreateBombBird(BIRD_START_POSITION, BOMB_BIRD_RADIUS));
	GazEngine::addEngine(pGrumpyEngine);

	//GDIRenderingEngine* pRenderingEngine = new GDIRenderingEngine(pWindowEngine->getWindow());
	//pRenderingEngine->addRenderer(new GDIRenderer);
	//GazEngine::addEngine(pRenderingEngine);

	GrumpyFactory::CreateMap();
	GazEngine::addEntity(GrumpyFactory::CreatePig(Vector2(555.0f, 200.0f), 20.0f));
	GazEngine::addEntity(GrumpyFactory::CreatePig(Vector2(635.0f, 150.0f), 20.0f));
	GazEngine::addEntity(GrumpyFactory::CreatePig(Vector2(825.0f, 120.0f), 20.0f));

	GazEngine::setMaxFrameRate(30);
	GazEngine::play();

    return 0;
}
