/**
 *	@file GDE\Tests\TestMandelbrot\TestMandelbrot.cpp
 *	@brief:
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/

#include "GDE/Core/Core.h"
#include "GDE/Core/PerfTools.h"
#include "GDE/Renderer/Renderer.h"
#include "GDE/Renderer/Vertex.h"
#include "GDE/Renderer/FontManager.h"
#include "GDE/Renderer/Font.h"
#include "GDE/Core/GameTimer.h"

#include "GDE/Input/InputDevice.h"
#include "GDE/Input/InputChangeSubject.h"

#include "GDE/Renderer/Texture.h"
#include "GDE/Renderer/TextureManager.h"

#include "GDE/Core/DLLLoader.h"

#include "GDE/Core/Logger.h"

#include "TestMandelbrot.h"
#include "MandelbrotBuffer.h"
#include "KeyboardObserver.h"
#include "ThreadPoolDeluxe.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cmath>
#include <stdio.h>

#ifdef _DEBUG
const char* g_strDllName = "HGERenderer_D.dll";
const char* g_strInputDllName = "HGERenderer_D.dll"; 
const char* g_strFontDllName = "HGERenderer_D.dll";
#else
const char* g_strDllName = "HGERenderer.dll";
const char* g_strInputDllName = "HGERenderer.dll"; 
const char* g_strFontDllName = "HGERenderer.dll";
#endif

struct TMandelbrotPartialData
{
	CMandelbrotBuffer* mandelbrot;
	UINT32 task;
	UINT32 taskCount;
};

using namespace GDE;

void mandelbrotThreadFunction(void* _data)
{
	TMandelbrotPartialData* pMandelbrotPartialData = static_cast<TMandelbrotPartialData*>(_data);
	pMandelbrotPartialData->mandelbrot->FillPartial(pMandelbrotPartialData->taskCount, pMandelbrotPartialData->task);
	delete pMandelbrotPartialData;
}

/**
 *	Main windows application entry point.
 */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	CRenderer TheRenderer;
	if ( false == TheRenderer.LoadDLL( g_strDllName ) )
	{
		printf( "Unable to load %s\n", g_strDllName );
		assert_now( "Unable to load %s\n", g_strDllName );
		return 0;
	}

	GDE::CDataParserManager DataManager;
	if ( false == DataManager.LoadDLL( g_strDllName ) )
	{
		assert_now( "Error loading parsing dll \"%s\"", g_strDllName );
		return -1;
	}

	CMandelbrotContext RendererContext( TheRenderer, DataManager );
	TheRenderer.SetContext( RendererContext );
	RendererContext.Init();

	TheRenderer.RunGameLoop();
	RendererContext.Release();

	return 0;
}


/**
 *	Constructor, prepare the context.
 */
CMandelbrotContext::CMandelbrotContext( CRenderer& Renderer, CDataParserManager& ParserManager )
: m_Renderer( Renderer )
, m_pInputKeyboard( NULL )
, m_pFrameTimer( NULL )
, m_pFontMgr( NULL )
, m_pFont( NULL )
, m_pMandelbrotTexture( NULL )
, m_pMandelbrot( NULL )
, m_pParserManager( &ParserManager )
{	
	m_bCreateZBuffer = false;

	m_pFrameTimer = new GDE::CClock();

	m_pFontMgr = new GDE::CFontManager();
	if ( false == m_pFontMgr->LoadDLL( g_strFontDllName, &m_Renderer ) )
	{
		printf( "Unable to load %s\n", m_pFontMgr );
		assert_now( "Unable to load %s\n", m_pFontMgr );
	}

	m_pInputKeyboard = new GDE::CInputDevice();
	if ( false == m_pInputKeyboard->LoadDLL( g_strInputDllName ) )
	{
		printf( "Unable to load %s\n", g_strDllName );
		assert_now( "Unable to load %s\n", g_strDllName );
	}

	//Register keys:
	const char* strKeyCommands[] = 
	{
		"KeyEscape", //KC_Quit,		
	};
	_COMPILE_ASSERT( _ARRAY_SIZE( strKeyCommands ) == KC_Count );

	for ( GDE::UINT32 i=0; i<KC_Count; i++ )
	{
		m_bKeyDown[i] = m_bKeyWasDown[i] = false;
		m_KeyObserver[i].Init( m_bKeyDown[i] );
		CreateAndRegisterKey( 
			strKeyCommands[i], 
			m_pInputKeyboard->GetChangeSubject(),
			m_KeyObserver[i] );
	}
}

/**
 *	Initialise the context, called once the renderer has been created and initialised.
 */
void CMandelbrotContext::Init( void )
{
	// create a Mandelbrot scene.
//	m_Renderer.SetRenderStateB( RS_bAlphaBlendEnable, false );

	//TODO: tacky, really need a way to create blank images - not render tagets though!
	m_pMandelbrotTexture = m_Renderer.GetTextureManager().LoadResource(
		GDE::CResourceId( "MandelbrotTemplate.png" ) );
	assert_msg( NULL != m_pMandelbrotTexture, "Unable to open madelbrot template image." );

	m_pMandelbrot = new CMandelbrotBuffer(
		m_pMandelbrotTexture->GetWidth(), m_pMandelbrotTexture->GetHeight(),
		m_pMandelbrotTexture->GetWidth(), m_pMandelbrotTexture->GetHeight() );

	GDE::CDataParser* pParser = m_pParserManager->LoadResource( GDE::CResourceId( "Config.ini" ) );
	if ( NULL == pParser )
	{
		assert_now( "Unable to load/parse ini file" );
		m_uiTaskCount = 1;
		m_uiThreadCount = 1;
	}
	else
	{
		pParser->GetValue(m_uiTaskCount, "Tasks", "TaskCount");
		pParser->GetValue(m_uiThreadCount, "ThreadPool", "ThreadCount");
		pParser->Release();
	}

	if (m_uiTaskCount <= 0 || m_uiTaskCount > 1024)
	{
		GDE::CLogger::GetDebugPrintLog().PrintF("The task count provided was out of range (%i), using 1 instead.\n",
			m_uiTaskCount);
		m_uiTaskCount = 1;
	}
	if (m_uiThreadCount <= 0 || m_uiThreadCount > 64)
	{
		GDE::CLogger::GetDebugPrintLog().PrintF("The thread count provided was out of range (%i), using 1 instead.\n",
			m_uiThreadCount);
		m_uiThreadCount = 1;
	}

	// create the debug font
	m_pFont = m_pFontMgr->LoadResource( CResourceId( "CourierNew.fnt" ) );
}


/**
 *	Register's a key to the given event with the keyboard input.
 */
void CMandelbrotContext::CreateAndRegisterKey( 
		const char* strKey, 
		GDE::IInputChangeSubject& Subject, 
		IObserver& Observer )
{
	//Register the observers
	GDE::UINT32 uCode;
	if ( m_pInputKeyboard->GetInputCode( strKey, uCode ) )
	{
		Subject.AddObserver( (GDE::IObserver*)&Observer, uCode );
	} else
	{
		assert_now( "Unable to bind input for key \"%s\"", strKey );
	}
}

/**
 *	Destructor.
 */
CMandelbrotContext::~CMandelbrotContext( void )
{
	SafeDelete( m_pInputKeyboard );
	SafeDelete( m_pFrameTimer );
	SafeDelete( m_pFontMgr );
}

/**
 *	Release all the context's members, called before the renderer is shut down.
 */
void CMandelbrotContext::Release( void )
{
	SafeRelease( m_pFont );
	SafeRelease( m_pMandelbrotTexture );
	SafeDelete( m_pMandelbrot );
}


/**
 *	Update the keyboard states for the frame.
 */
void CMandelbrotContext::UpdateKeyStates( void )
{
	memcpy( m_bKeyWasDown, m_bKeyDown, sizeof( m_bKeyWasDown ) );
}


/**
 *	Implements the basic context requirements, update and then render the frame.
 */
bool CMandelbrotContext::FrameUpdate( void )
{
	CThreadPoolDeluxe threadPool(m_uiThreadCount);
	for (GDE::UINT32 uiIndex = 0; uiIndex < m_uiTaskCount; uiIndex++)
	{
		threadPool.QueueTask(CreateTask(uiIndex));
	}
	threadPool.Start();

	// Added for testing.
	//m_pMandelbrot->Clear();

	m_pFrameTimer->FrameUpdate();
	
	FLOAT32 fTimeInterval = m_pFrameTimer->GetFrameInterval(); // allows pausing.

//	if ( m_Renderer.IsActiveWindow() )
	{
		m_pInputKeyboard->Update( fTimeInterval );
	}
	UpdateKeyStates();

	m_Renderer.BeginScene();
	m_Renderer.ColourClear( 0xFF000000 );

	m_pMandelbrotTexture->Apply( 0 );
	const static SVertex Quads[] = 
	{
		// x,		y,		z,		colour,		u,	v
		{ 0.0f,	0.0f,	0.0f, 0xFFFFFFFF, 0.0f, 0.0f },
		{ FLOAT32(m_uDispWidth),	0.0f,	0.0f, 0xFFFFFFFF, 1.0f, 0.0f }, 
		{ FLOAT32(m_uDispWidth),	FLOAT32(m_uDispHeight),	0.0f, 0xFFFFFFFF, 1.0f, 1.0f }, 
		{ 0.0f,	FLOAT32(m_uDispHeight),	0.0f, 0xFFFFFFFF, 0.0f, 1.0f }, // end quad
	};

//	m_Renderer.SetRenderStateB( GDE::RS_bAlphaBlendEnable, false );
	m_Renderer.DrawPrimitives( PT_QuadList, _ARRAY_SIZE(Quads)/4, Quads );
	m_Renderer.GetTextureManager().GetNULLTexture()->Apply( 0 ); 

	char InfoText[256];
	const FLOAT32 fFrameRate = 1.0f/m_pFrameTimer->GetFrameInterval();
	const GDE::UINT32 uFrameRate = GDE::UINT32(fFrameRate);
	const GDE::UINT32 uFrameDecimal = GDE::UINT32(fFrameRate*100.0f)%100;
 
	threadPool.Join();

	sprintf_s(  InfoText, "Framerate: %05d.%02d\nMandlebrot calc time: %.5f",
		uFrameRate, uFrameDecimal, threadPool.GetTimeTaken() );
	m_pFont->Render( 10, 560, GDE::FJ_Left, InfoText );

	m_Renderer.EndScene();

	UpdateTexture();

	return m_bKeyDown[KC_Quit] == false;
}

/**
 *	Update the texture from the image created.
 */
void CMandelbrotContext::UpdateTexture( void )
{
	GDE::UINT32* pTexels = NULL;
	GDE::UINT32 uPitch;
	m_pMandelbrotTexture->LockTexels( pTexels, uPitch, false );

	// fast and nasty:
//	memcpy( pTexels, m_pMandelbrot->GetPixels(), uPitch* m_pMandelbrotTexture->GetHeight() );

	for ( GDE::UINT32 uRow = 0; uRow<m_pMandelbrotTexture->GetHeight(); uRow++ )
	{
		for ( GDE::UINT32 uCol = 0; uCol<m_pMandelbrotTexture->GetWidth(); uCol++ )
		{
			pTexels[uCol] = m_pMandelbrot->TexelAt( uCol, uRow );
		}
		pTexels += uPitch;
	}

	m_pMandelbrotTexture->UnlockTexels();
}

TTask CMandelbrotContext::CreateTask( GDE::UINT32 _uiTaskNumber )
{
	TMandelbrotPartialData* pMandelbrotPartialData = new TMandelbrotPartialData;
	pMandelbrotPartialData->task = _uiTaskNumber;
	pMandelbrotPartialData->taskCount = m_uiTaskCount;
	pMandelbrotPartialData->mandelbrot = m_pMandelbrot;

	TTask mandelbrotTask;
	mandelbrotTask.data = pMandelbrotPartialData;
	mandelbrotTask.runnable = &mandelbrotThreadFunction;

	return mandelbrotTask;
}
