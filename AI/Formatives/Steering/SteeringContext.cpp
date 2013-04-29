/**
 *	@file AI_Steering\SteeringContext.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "SteeringContext.h"
#include "FVector2.h"
#include "Wall.h"
#include "Circle.h"
#include "Mouse.h"
#include "Tree.h"
#include "Cat.h"
#include "Blackboard.h"

#include "GDE/Core/GameTimer.h"
#include "GDE/Renderer/Renderer.h"
#include "GDE/Input/InputDevice.h"
#include "GDE/Input/InputChangeSubject.h"
#include "GDE/Renderer/FontManager.h"
#include "GDE/Renderer/Font.h"
#include "GDE/Renderer/Vertex.h"
#include "GDE/Core/Logger.h"
#include "GDE/DataParser/DataParserManager.h"
#include "GDE/DataParser/DataParser.h"
#include "GDE/Core/Observer.h"
// for untextured:
#include "GDE/Renderer/TextureManager.h"
#include "GDE/Renderer/Texture.h"

#include <stdio.h> // for debug string info printing...
#include <math.h> // for sin/cos
/**
 *	Simple, tiny class to allow the speed of time to be changed.
 */
class CChangeSpeedOfTimeObserver : public GDE::IObserver
{
public:
	CChangeSpeedOfTimeObserver( bool bSpeedUp, GDE::CClock& Clock )
		: m_bSpeedUp( bSpeedUp )
		, m_Clock( Clock )
	{	// empty
	}
	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
	{
		uMessage; pSubject; // unused parameters.
		if ( m_bSpeedUp )
		{
			FLOAT32 fSpeedOfTime = m_Clock.GetSpeedOfTime();
			fSpeedOfTime = (fSpeedOfTime<0.95f)?fSpeedOfTime+0.05f:fSpeedOfTime;
			m_Clock.SetSpeedOfTime( fSpeedOfTime );
		} else
		{
			FLOAT32 fSpeedOfTime = m_Clock.GetSpeedOfTime();
			fSpeedOfTime = (fSpeedOfTime>0.05f)?fSpeedOfTime-0.05f:fSpeedOfTime;
			m_Clock.SetSpeedOfTime( fSpeedOfTime );
		}
	}
protected:
	CChangeSpeedOfTimeObserver& operator=(const CChangeSpeedOfTimeObserver& Other);	// non implemented assignment, fixes a warning.
	
	bool			m_bSpeedUp;
	GDE::CClock&	m_Clock;
};


/**
 *	Singleton get instance.
 */
CSteeringContext& CSteeringContext::GetInstance( void )
{
	static CSteeringContext Context;
	return Context;
}

/**
 *	Construct an empty context, ready to be initialised.
 *	Note: this is a singleton, so this is protected.
 */
CSteeringContext::CSteeringContext( void )
: m_pRenderer( NULL )
, m_pInputMgr( NULL )
, m_pParserManager( NULL )
, m_pPitchUpSpeedObsrv( NULL )
, m_pPitchDownSpeedObsrv( NULL )
, m_pClock( NULL )
, m_pFrameTimer( NULL )
, m_pFont( NULL )
, m_pWalls( NULL )
, m_uWallCount( 0 )
, m_pMouse( NULL )
, m_pTrees( NULL )
, m_uTreeCount( 0 )
{	// initialisation, empty.
}

/**
 *	Initialise the context and create all resources it requires.
 *
 *	@return true on success.
 */
bool CSteeringContext::Initialise( 
	GDE::CRenderer& Renderer, 
	GDE::CInputDevice& InputMgr,
	GDE::CFontManager& FontManager,
	GDE::CDataParserManager& DataParserMan )
{
	//m_bWindowed = false;
	//m_uDispWidth = 1024;
	//m_uDispHeight = 768;
	//m_uDispWidth = 1280;
	//m_uDispHeight = 1024;
	m_strWindowTitle = "MDS GD230 - AI Steering";

	if ( false == Renderer.SetContext( *this ) )
	{
		assert_now( "error creating context" );
		return false;
	}

	m_pRenderer = &Renderer;
	m_pInputMgr = &InputMgr;
	m_pParserManager = &DataParserMan;

	m_pClock = new GDE::CClock();
	m_pFrameTimer = new GDE::CClock();

	m_pPitchUpSpeedObsrv = new CChangeSpeedOfTimeObserver( true, *m_pClock );
	m_pPitchDownSpeedObsrv = new CChangeSpeedOfTimeObserver( false, *m_pClock );

	CreateAndRegisterKey( "KeyF4", m_pInputMgr->GetDownSubject(), *m_pPitchDownSpeedObsrv );
	CreateAndRegisterKey( "KeyF5", m_pInputMgr->GetDownSubject(), *m_pPitchUpSpeedObsrv );		

	m_pMouse = new CMouse;
	CreateAndRegisterKey( "MouseXY", m_pInputMgr->GetChangeSubject(), *m_pMouse );
	CreateAndRegisterKey( "MouseXY", m_pInputMgr->GetChangeSubject(), CBlackboard::GetInstance() );

	m_pFont = FontManager.LoadResource( GDE::CResourceId( "CourierNew.fnt" ) );
	if ( NULL == m_pFont )
	{
		return false;
	}
	m_pFont->SetColour( 0xFF000000 );

	// now load in the ini file:
	GDE::CDataParser* pParser = m_pParserManager->LoadResource( GDE::CResourceId( "Steering.ini" ) );
	if ( NULL == pParser )
	{
		assert_now( "Unable to load/parse ini file" );
		return false;
	}

	// load walls:
	if ( false == pParser->GetValue( m_uWallCount, "Main", "Walls" ) )
	{
		assert_now( "INI: no wall count" );
		pParser->Release();
		return false;
	}

	m_pWalls = new CWall[m_uWallCount];
	FLOAT32 fX0, fY0, fX1, fY1;
	char strItemName[10];
	for ( UINT32 i=0; i<m_uWallCount; i++ )
	{
		sprintf_s( strItemName, "Wall%d", i );
		if ( false == pParser->GetValue( fX0, strItemName, "X0" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "X0" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->GetValue( fY0, strItemName, "Y0" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "Y0" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->GetValue( fX1, strItemName, "X1" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "X1" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->GetValue( fY1, strItemName, "Y1" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "Y1" );
			pParser->Release();
			return false;
		}

		m_pWalls[i].Initialise( SFVec2( fX0, fY0 ), SFVec2( fX1, fY1 ) );

		CCircle repulsionZone0;
		repulsionZone0.Initialise(SFVec2(fX0, fY0), 0.0f);
		CBlackboard::GetInstance().AddRepulsionZone(repulsionZone0);
		CCircle repulsionZone1;
		repulsionZone1.Initialise(SFVec2(fX1, fY1), 0.0f);
		CBlackboard::GetInstance().AddRepulsionZone(repulsionZone1);
		CBlackboard::GetInstance().AddWall(m_pWalls[i]);
	}

	// load trees:
	if ( false == pParser->GetValue( m_uTreeCount, "Main", "Trees" ) )
	{
		assert_now( "INI: no tree count" );
		pParser->Release();
		return false;
	}

	m_pTrees = new CTree[m_uTreeCount];
	for ( UINT32 i=0; i<m_uTreeCount; i++ )
	{
		sprintf_s( strItemName, "Tree%d", i );
		if ( false == pParser->GetValue( fX0, strItemName, "X" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "X" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->GetValue( fY0, strItemName, "Y" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "Y" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->GetValue( fX1, strItemName, "R" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "R" );
			pParser->Release();
			return false;
		}

		m_pTrees[i].Initialise( SFVec2( fX0, fY0 ), fX1 );

		CCircle repulsionZone;
		repulsionZone.Initialise(SFVec2(fX0, fY0), fX1);
		CBlackboard::GetInstance().AddRepulsionZone(repulsionZone);
	}

	// load cats:
	if ( false == pParser->GetValue( m_uCatCount, "Main", "Cats" ) )
	{
		assert_now( "INI: no cat count" );
		pParser->Release();
		return false;
	}

	FLOAT32 fMaxFrameRate;
	if( pParser->GetValue( fMaxFrameRate, "Main", "MaxFrameRate" ) )
	{
		m_pFrameTimer->SetMinFrameInterval( 1.0f/fMaxFrameRate );
	}

	m_pCats = new CCat[m_uCatCount];
	for ( UINT32 i=0; i<m_uCatCount; i++ )
	{
		sprintf_s( strItemName, "Cat%d", i );
		if ( false == pParser->GetValue( fX0, strItemName, "X" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "X" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->GetValue( fY0, strItemName, "Y" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "Y" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->GetValue( fX1, strItemName, "Direction" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "Direction" );
			pParser->Release();
			return false;
		}

		m_pCats[i].Initialise( SFVec2( fX0, fY0 ), fX1 );

		CBlackboard::GetInstance().AddCat(m_pCats[i]);
	}

	pParser->Release();
	return true;
}

CSteeringContext::~CSteeringContext( void )
{
	if ( m_pPitchUpSpeedObsrv )
	{
		delete m_pPitchUpSpeedObsrv;
	}
	if ( m_pPitchDownSpeedObsrv )
	{
		delete m_pPitchDownSpeedObsrv;
	}
	if ( m_pClock )
	{
		delete m_pClock;
	}
	if ( m_pFrameTimer )
	{
		delete m_pFrameTimer;
	}
	if ( m_pWalls )
	{
		delete[] m_pWalls;
	}
	if ( m_pMouse )
	{
		delete m_pMouse;
	}
	if ( m_pTrees )
	{
		delete[] m_pTrees;
	}
	if ( m_pCats )
	{
		delete[] m_pCats;
	}
}


void CSteeringContext::CreateAndRegisterKey( const char* strKey, GDE::IInputChangeSubject& Subject, GDE::IObserver& Observer )
{
	// now register the observers
	GDE::UINT32 uCode;
	if ( m_pInputMgr->GetInputCode( strKey, uCode ) )
	{
		Subject.AddObserver( (GDE::IObserver*)&Observer, uCode );
	} else
	{
		assert_now( "Unable to bind input for key \"%s\"", strKey );
	}
}

bool CSteeringContext::FrameUpdate( void )
{
	const static UINT32 colCollide = 0xFFFF0000;
	const static UINT32 colNonCollide = 0xFF00FF00;
	const static CFVec2 fv2ScreenExtents = SFVec2( (FLOAT32)GetDisplayWidth(), (FLOAT32)GetDisplayHeight() );

	m_pFrameTimer->FrameUpdate();
	m_pClock->FrameUpdate();
	const FLOAT32 fTimeInterval = m_pClock->GetFrameInterval(); // allows pausing.
	m_pInputMgr->Update( fTimeInterval );

	// do all required updates here.
	for ( UINT32 i=0; i<m_uCatCount; i++ )
	{
		m_pCats[i].Update( fTimeInterval );
	}

	IntersectionUpdate();

	// now for all the rendering,
	m_pRenderer->BeginScene();
	m_pRenderer->ColourClear( 0xFFFFFFFF );

	for ( UINT32 i=0; i<m_uTreeCount; i++ )
	{
		m_pTrees[i].DrawCanopy();
	}

	for ( UINT32 i=0; i<m_uWallCount; i++ )
	{
		m_pWalls[i].Draw();
	}

	for ( UINT32 i=0; i<m_uTreeCount; i++ )
	{
		m_pTrees[i].DrawTrunk();
	}

	for ( UINT32 i=0; i< m_uCatCount; i++ )
	{
		m_pCats[i].Draw();
	}

	m_pMouse->Draw();

//	if ( m_bDrawInfoText )
	{
		char InfoText[256];
		const FLOAT32 fFrameRate = 1.0f/m_pFrameTimer->GetFrameInterval();
		const UINT32 uFrameRate = UINT32(fFrameRate);
		const UINT32 uFrameDecimal = UINT32(fFrameRate*100.0f)%100;
		sprintf_s( InfoText, "Framerate %05d.%02d", uFrameRate, uFrameDecimal );
		m_pFont->Render( 10, 10, GDE::FJ_Left, InfoText );
	}

	m_pRenderer->EndScene();
	return true;
}

void CSteeringContext::IntersectionUpdate()
{
	for (UINT32 uiIndex = 0; uiIndex < m_uWallCount; uiIndex++)
	{
		m_pWalls[uiIndex].ResetIntersection();
	}

	for (UINT32 uiIndex = 0; uiIndex < m_uTreeCount; uiIndex++)
	{
		m_pTrees[uiIndex].ResetIntersection();
	}

	for (UINT32 uiIndex = 0; uiIndex < m_uCatCount; uiIndex++)
	{
		m_pCats[uiIndex].ResetIntersection();
	}

	for (UINT32 uiCatIndex = 0; uiCatIndex < m_uCatCount; uiCatIndex++)
	{
		CCircle bounds;
		m_pCats[uiCatIndex].GetBoundingCircle(bounds);

		for (UINT32 uiWallIndex = 0; uiWallIndex < m_uWallCount; uiWallIndex++)
		{
			if (m_pWalls[uiWallIndex].Intersects(bounds))
			{
				m_pWalls[uiWallIndex].SetIntersectedByCat();
			}
		}

		for (UINT32 uiTreeIndex = 0; uiTreeIndex < m_uTreeCount; uiTreeIndex++)
		{
			if (m_pTrees[uiTreeIndex].Intersects(bounds))
			{
				m_pTrees[uiTreeIndex].SetIntersectedByCat();
			}
		}

		for (UINT32 uiOtherCatIndex = 0; uiOtherCatIndex < m_uCatCount; uiOtherCatIndex++)
		{
			if (uiOtherCatIndex == uiCatIndex)
			{
				continue;
			}

			if (m_pCats[uiOtherCatIndex].Intersects(bounds))
			{
				m_pCats[uiOtherCatIndex].SetIntersectedByCat();
			}
		}
	}
}

void CSteeringContext::ReleaseResources( void )
{
	m_pFont->Release();
}

/**
 *	@param Circle	The circle to draw.
 *	@param uColour	The colour to draw with.
 */
void CSteeringContext::DrawCircle( const CCircle& Circle, UINT32 uColour )
{
	// how many line segments?
	const FLOAT32 fRadius = Circle.Radius();
	const FLOAT32 fCircumference = 2.0f * fPI * fRadius;
	
	const static UINT32 uMAX_SEGMENTS = 20;
	UINT32 uSegments = UINT32( fCircumference*0.2f );
	if ( uSegments>uMAX_SEGMENTS )
	{
		uSegments = uMAX_SEGMENTS;
	}

	// TODO: optimise with line strip rather than list.
	GDE::SVertex Verts[uMAX_SEGMENTS*2+2];
	const FLOAT32 fAngleStep = 2.0f*fPI/uSegments;
	FLOAT32 fCurAngle;
	Verts[0].m_fX = fRadius + Circle.Centre().X();
	Verts[0].m_fY = Circle.Centre().Y();;
	Verts[0].m_fZ = 0.5f;
	Verts[0].m_uColour = uColour;
	Verts[uSegments*2-1] = Verts[0];
	for ( UINT32 i=1; i<uSegments;i++ )
	{
		fCurAngle = fAngleStep * i;
		Verts[i*2-1].m_fX = cosf( fCurAngle ) * fRadius + Circle.Centre().X();
		Verts[i*2-1].m_fY = sinf( fCurAngle ) * fRadius + Circle.Centre().Y();
		Verts[i*2-1].m_fZ = 0.5f;
		Verts[i*2-1].m_uColour = uColour;
		Verts[i*2] = Verts[i*2-1];
	}

	// untextured:
	m_pRenderer->GetTextureManager().GetNULLTexture()->Apply( 0 );
	m_pRenderer->DrawPrimitives( GDE::PT_LineList, uSegments, Verts );
}

/**
 *	@param Circle	The circle to draw.
 *	@param uColour	The colour to draw with.
 */
void CSteeringContext::DrawFilledCircle( const CCircle& Circle, UINT32 uColour )
{
	// how many line segments?
	const static FLOAT32 fPI = 3.141592653589793f;
	const FLOAT32 fRadius = Circle.Radius();
	const FLOAT32 fCircumference = 2.0f * fPI * fRadius;
	
	const static UINT32 uMAX_SEGMENTS = 20;
	UINT32 uSegments = UINT32( fCircumference*0.2f );
	if ( uSegments>uMAX_SEGMENTS )
	{
		uSegments = uMAX_SEGMENTS;
	}
	if ( uSegments < 3 )
	{
		uSegments=3;
	}

	// TODO: optimise with line strip rather than list.
	GDE::SVertex Verts[uMAX_SEGMENTS*3];
	const FLOAT32 fAngleStep = 2.0f*fPI/uSegments;
	FLOAT32 fCurAngle;
	Verts[0].m_fX = Circle.Centre().X();
	Verts[0].m_fY = Circle.Centre().Y();;
	Verts[0].m_fZ = 0.5f;
	Verts[0].m_uColour = uColour;
	Verts[1].m_fX = fRadius + Circle.Centre().X();
	Verts[1].m_fY = Circle.Centre().Y();;
	Verts[1].m_fZ = 0.5f;
	Verts[1].m_uColour = uColour;
	Verts[uSegments*3-1] = Verts[1];
	for ( UINT32 i=1; i<uSegments;i++ )
	{
		fCurAngle = fAngleStep * i;
		Verts[i*3-1].m_fX = cosf( fCurAngle ) * fRadius + Circle.Centre().X();
		Verts[i*3-1].m_fY = sinf( fCurAngle ) * fRadius + Circle.Centre().Y();
		Verts[i*3-1].m_fZ = 0.5f;
		Verts[i*3-1].m_uColour = uColour;
		Verts[i*3+1] = Verts[i*3-1];
		Verts[i*3] = Verts[0];
	}
	// untextured:
	m_pRenderer->GetTextureManager().GetNULLTexture()->Apply( 0 );
	m_pRenderer->DrawPrimitives( GDE::PT_TriangleList, uSegments, Verts );
}


