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
#include <gazengine/GazEngine.h>
#include <gazengine/gdi/model/GDIMesh.h>
#include <gazengine/gdi/rendering/GDIRenderer.h>
#include <gazengine/gdi/rendering/GDIRenderingEngine.h>
#include <gazengine/winapi/WinApiEngine.h>

#include "SpringyUIEngine.h"

using namespace std;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int iCommandShow)
{
    WinApiEngine* pWindowEngine = new WinApiEngine(L"Day 008.1", instance, iCommandShow);
    pWindowEngine->createWindow();
	GazEngine::addEngine(pWindowEngine);

	GDIRenderingEngine* pRenderingEngine = new GDIRenderingEngine(pWindowEngine->getWindow());
	pRenderingEngine->addRenderer(new GDIRenderer);
	GazEngine::addEngine(pRenderingEngine);

	GazEngine::addEngine(new CSpringyUIEngine);

	Entity* pMenuItem1 = new Entity;
	vector<Vector2> vertices1;
	vertices1.reserve(4);
	vertices1.push_back(Vector2(150.0f, 250.0f));
	vertices1.push_back(Vector2(350.0f, 250.0f));
	vertices1.push_back(Vector2(150.0f, 350.0f));
	vertices1.push_back(Vector2(350.0f, 350.0f));
	GDIMesh* pMenuItem1Model = new GDIMesh(vertices1, RGB(0, 255, 0));
	pMenuItem1->addComponent(pMenuItem1Model);
	pMenuItem1Model->setEntity(pMenuItem1);
	GazEngine::addEntity(pMenuItem1);

	Entity* pMenuItem2 = new Entity;
	vector<Vector2> vertices2;
	vertices2.reserve(4);
	vertices2.push_back(Vector2(450.0f, 250.0f));
	vertices2.push_back(Vector2(650.0f, 250.0f));
	vertices2.push_back(Vector2(450.0f, 350.0f));
	vertices2.push_back(Vector2(650.0f, 350.0f));
	GDIMesh* pMenuItem1Mode2 = new GDIMesh(vertices2, RGB(0, 255, 0));
	pMenuItem2->addComponent(pMenuItem1Mode2);
	pMenuItem1Mode2->setEntity(pMenuItem2);
	GazEngine::addEntity(pMenuItem2);

	GazEngine::setMaxFrameRate(60);
	GazEngine::play();

    return 0;
}
