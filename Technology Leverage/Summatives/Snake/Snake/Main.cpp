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
// $Date: 2013-01-23 09:45:25 +1300 (Wed, 23 Jan 2013) $
// $Rev: 1110 $
//
#include <ctime>

#include "SnakeClientEngine.h"
#include "SnakeEvents.h"

// Must be after SnakeClientEngine.h because it includes winsock.h and this includes windows.h.
#include "ServerBrowser.h"

#include <gazengine/GazEngine.h>
#include <gazengine/gdi/rendering/GDIRenderer.h>
#include <gazengine/gdi/rendering/GDIRenderingEngine.h>
#include <gazengine/winapi/WinApiEngine.h>

#ifdef _DEBUG
#include <vld.h>
#endif

void OnStartGame(const void* _kpMessage);
int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int iCommandShow);

WinApiEngine* g_pWindowEngine = NULL;

void OnStartGame(const void*)
{
	g_pWindowEngine->grabFocus();
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int iCommandShow)
{
    g_pWindowEngine = new WinApiEngine(L"Snake", instance, iCommandShow);
    g_pWindowEngine->createWindow();
	g_pWindowEngine->setDialog(ServerBrowser::Create(instance));
	GazEngine::addEngine(g_pWindowEngine);

	GazEngine::addEngine(new CSnakeClientEngine);

	GDIRenderingEngine* renderingEngine = new GDIRenderingEngine(g_pWindowEngine->getWindow());
	renderingEngine->addRenderer(new GDIRenderer);
	GazEngine::addEngine(renderingEngine);

	Messages::registerRecipient(SnakeClientEvents::GAME_STARTED, OnStartGame);

	GazEngine::setMaxFrameRate(30);
	GazEngine::play();

	ServerBrowser::Dispose();

    return 0;
}
