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
#include <gazengine/gdi/rendering/GDIRenderer.h>
#include <gazengine/gdi/rendering/GDIRenderingEngine.h>
#include <gazengine/winapi/WinApiEngine.h>

#include "ClothEngine.h"

using namespace std;

const unsigned int GRID_HEIGHT = 20;
const unsigned int GRID_WIDTH = 20;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int iCommandShow)
{
    WinApiEngine* pWindowEngine = new WinApiEngine(L"Day 007.1", instance, iCommandShow);
    pWindowEngine->createWindow();
	GazEngine::addEngine(pWindowEngine);

	GDIRenderingEngine* pRenderingEngine = new GDIRenderingEngine(pWindowEngine->getWindow());
	pRenderingEngine->addRenderer(new GDIRenderer);
	GazEngine::addEngine(pRenderingEngine);

	CClothEngine* pClothEngine = new CClothEngine(new CCloth(GRID_HEIGHT, GRID_WIDTH));
	GazEngine::addEngine(pClothEngine);

	GazEngine::setMaxFrameRate(60);
	GazEngine::play();

    return 0;
}
