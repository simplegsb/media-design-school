/**
 *	@file QuadTree\QuadTreeMain.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "SteeringContext.h"

#include "GDE/Renderer/Renderer.h"
#include "GDE/Input/InputDevice.h"
#include "GDE/DataParser/DataParserManager.h"
#include "GDE/Core/DLLLoader.h"
#include "GDE/Renderer/FontManager.h"

#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


#ifdef _DEBUG
const char* g_strDllName = "HGERenderer_D.dll";
#else
const char* g_strDllName = "HGERenderer.dll";
#endif

/**
 *	Simply initialise and kick off the game (application) loop..
 */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{	
	GDE::CRenderer TheRenderer;
	if ( false == TheRenderer.LoadDLL( g_strDllName ) )
	{
		printf( "Unable to load %s\n", g_strDllName );
		assert_now( "Unable to load %s\n", g_strDllName );
		return 0;
	}
	GDE::CInputDevice TheInput;
	if ( false == TheInput.LoadDLL( g_strDllName ) )
	{
		printf( "Unable to load input from %s\n", g_strDllName );
		assert_now( "Unable to load input from %s\n", g_strDllName );
		return 1;
	}

	GDE::CDataParserManager DataManager;
	if ( false == DataManager.LoadDLL( g_strDllName ) )
	{
		assert_now( "Error loading parsing dll \"%s\"", g_strDllName );
		return -1;
	}

	GDE::CFontManager FontMgr;
	if ( false == FontMgr.LoadDLL( g_strDllName, &TheRenderer ) )
	{
		assert_now( "Error loading font dll \"%s\"", g_strDllName );
		return -1;	
	}


	if ( false == CSteeringContext::GetInstance().Initialise(
			TheRenderer, 
			TheInput, 
			FontMgr,
			DataManager ) )
	{
		assert_now( "Error initialising the steering app" );
		return -1;
	}

	TheRenderer.RunGameLoop();
	CSteeringContext::GetInstance().ReleaseResources();
	return 0;
}

