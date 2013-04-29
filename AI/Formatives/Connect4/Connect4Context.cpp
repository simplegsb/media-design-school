/**
 *	@file Connect4\Connect4Context.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2012 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/

#include "Common.h"
#include "Connect4Context.h"
#include "FVector2.h"
#include "Mouse.h"
#include "GameBoard.h"
#include "Circle.h"
#include "PlayerAgent.h"

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

const static UINT32 MAX_PATH = 260;
/**
 *	Singleton get instance.
 */
CConnect4Context& CConnect4Context::GetInstance( void )
{
	static CConnect4Context Context;
	return Context;
}

/**
 *	Construct an empty context, ready to be initialised.
 *	Note: this is a singleton, so this is protected.
 */
CConnect4Context::CConnect4Context( void )
: m_pRenderer( NULL )
, m_pInputMgr( NULL )
, m_pParserManager( NULL )
, m_pFrameTimer( NULL )
, m_pPlayerTimer( NULL )
, m_pFont( NULL )
, m_pMouse( NULL )
, m_pBoard( NULL )
, m_uPlayerNumber( 0 )
, m_uLastTurnEvaluations( 0 )
, m_bUnitTesting( false )
, m_bTestCompleted( false )
, m_bExitAfterTest( false )
, m_pbUnitTestAllowedRes( false )
{
	// create the board:
	m_pBoard = new CGameBoard();
	// create the players
	m_pPlayers[0] = new CPlayerAgent();
	m_pPlayers[1] = new CPlayerAgent();
	m_uPlayerScores[0] = 0;
	m_uPlayerScores[1] = 0;
}

/**
 *	Initialise the context and create all resources it requires.
 *
 *	@return true on success.
 */
bool CConnect4Context::Initialise( 
	GDE::CRenderer& Renderer, 
	GDE::CInputDevice& InputMgr,
	GDE::CFontManager& FontManager,
	GDE::CDataParserManager& DataParserMan,
	const char* strININame )
{
	//m_bWindowed = false;
	//m_uDispWidth = 1024;
	//m_uDispHeight = 768;
	//m_uDispWidth = 1280;
	//m_uDispHeight = 1024;
	m_strWindowTitle = "MDS GD230 - AI Connect4";

	if ( false == Renderer.SetContext( *this ) )
	{
		assert_now( "error creating context" );
		return false;
	}

	m_pRenderer = &Renderer;
	m_pInputMgr = &InputMgr;
	m_pParserManager = &DataParserMan;

	m_pFrameTimer = new GDE::CClock();
	m_pPlayerTimer = new GDE::CClock();
	m_pPlayerTimer->FrameUpdate();

	m_pMouse = new CMouse;
	CreateAndRegisterKey( "MouseXY", m_pInputMgr->GetChangeSubject(), *m_pMouse );
	CreateAndRegisterKey( "MouseLButton", m_pInputMgr->GetDownSubject(), *m_pMouse );

	m_pFont = FontManager.LoadResource( GDE::CResourceId( "CourierNew.fnt" ) );
	if ( NULL == m_pFont )
	{
		return false;
	}
	m_pFont->SetColour( 0xFF00FFFF );

	UINT32 uBoardWidth = 8;
	UINT32 uBoardHeight = 6;
	SINT32 iPlayer1Depth = 3;
	SINT32 iPlayer2Depth = 3;

	char PlayerOneDLL[MAX_PATH];
	char PlayerTwoDLL[MAX_PATH];
	sprintf_s( PlayerOneDLL, "UserChoiceAgent" );
	sprintf_s( PlayerTwoDLL, "UserChoiceAgent" );

	// now load in the ini file:
	GDE::CDataParser* pParser = m_pParserManager->LoadResource( GDE::CResourceId( strININame ) );
	if ( NULL == pParser )
	{
		assert_now( "Unable to load/parse ini file \"%s\"", strININame );
		return false;
	}

	if ( false == pParser->GetValue( uBoardWidth, "Board", "Width" ) )
	{
		assert_now( "INI: no board width" );
		pParser->Release();
		return false;
	}

	if ( false == pParser->GetValue( uBoardHeight, "Board", "Height" ) )
	{
		assert_now( "INI: no board height" );
		pParser->Release();
		return false;
	}

	if ( false == pParser->GetValue( iPlayer1Depth, "Red", "MaxDepth" ) )
	{
		assert_now( "INI: no player one (red) Depth" );
		pParser->Release();
		return false;
	}

	if ( false == pParser->GetString( PlayerOneDLL, MAX_PATH, "Red", "DLL" ) )
	{
		assert_now( "INI: no player one (red) dll" );
		pParser->Release();
		return false;
	}

	if ( false == pParser->GetValue( iPlayer2Depth, "Yellow", "MaxDepth" ) )
	{
		assert_now( "INI: no player one (yellow) Depth" );
		pParser->Release();
		return false;
	}

	if ( false == pParser->GetString( PlayerTwoDLL, MAX_PATH, "Yellow", "DLL" ) )
	{
		assert_now( "INI: no player two (yellow) dll" );
		pParser->Release();
		return false;
	}

	char strUnitTestName[MAX_PATH];
	if ( pParser->GetString( strUnitTestName, MAX_PATH, "UnitTest", "Name" ) )
	{
		m_bUnitTesting = true;
		UINT32 uDummy;
		if ( pParser->GetValue( uDummy, "UnitTest", "Exit" ) )
		{
			m_bExitAfterTest = uDummy!=0;
		}
	}

	m_pBoard->Init( uBoardWidth, uBoardHeight );
	m_pBoard->InitCloneSystem();

	char DLLFileName[MAX_PATH];
#ifdef _DEBUG
	const char* strDLLFormat = "%s_D.dll";
#else
	const char* strDLLFormat = "%s.dll";
#endif
	sprintf_s( DLLFileName, strDLLFormat, PlayerOneDLL );
	if ( false == m_pPlayers[0]->LoadDLL( DLLFileName ) )
	{
		assert_now( "Unable to load the first player (Red) dll: %s", DLLFileName );
		return false;
	}
	sprintf_s( DLLFileName, strDLLFormat, PlayerTwoDLL );
	if ( false == m_pPlayers[1]->LoadDLL( DLLFileName ) )
	{
		assert_now( "Unable to load the second player (Yellow) dll: %s", DLLFileName );
		return false;
	}

	m_pPlayers[0]->SetMaximumDepth( iPlayer1Depth );
	m_pPlayers[1]->SetMaximumDepth( iPlayer2Depth );

	CBoard::EBoardPos eNextPlayer = CBoard::PLAYER1;

	if ( m_bUnitTesting )
	{
		bool bReadARow = false;
		const static UINT32 uMAX_COLS = 100;
		char StringRow[uMAX_COLS];
		char RowName[10];
		for ( int y=0; y< m_pBoard->GetHeight(); y++ )
		{
			sprintf_s( RowName, "Row%d", y );
			if ( pParser->GetString( StringRow, uMAX_COLS, strUnitTestName, RowName ) )
			{
				bReadARow = true;
				for ( int x=0;x<m_pBoard->GetWidth(); x++ )
				{
					if ( StringRow[x] == 0 )
					{
						break;
					}
					if ( StringRow[x] == 'R' )
					{
						m_pBoard->SetBoardPos( x, y, CBoard::PLAYER1 );
						continue;
					}
					if ( StringRow[x] == 'Y' )
					{
						m_pBoard->SetBoardPos( x, y, CBoard::PLAYER2 );
						continue;
					}
					if ( StringRow[x] != ' ' && StringRow[x] != '0' )
					{
						assert_now( "Error parsing a board character (%d,d) is '%c'", x,y, StringRow[x] );
					}
				}
			}
		}
		// loaded in the board
		assert( bReadARow );

		// now load in allowed positions:
		if ( pParser->GetString( StringRow, uMAX_COLS, strUnitTestName, "Expected" ) )
		{
			m_pbUnitTestAllowedRes = new bool[m_pBoard->GetWidth()];
			for ( int x=0;x<m_pBoard->GetWidth(); x++ )
			{
				m_pbUnitTestAllowedRes[x] = false;
				if ( StringRow[x] == 0 )
				{
					for ( ;x<m_pBoard->GetWidth(); x++ )
					{
						m_pbUnitTestAllowedRes[x] = false;
					}
					break;
				}
				if ( StringRow[x] == 'X' )
				{
					m_pbUnitTestAllowedRes[x] = true;
					continue;
				}
				if ( StringRow[x] != '0' )
				{
					assert_now( "Error parsing result character (%d) is '%c'", x, StringRow[x] );
				}
			}
		} else
		{
			assert_now( "Unable to find unit test results in \"%s\"\n", strUnitTestName );
		}
		if ( pParser->GetString( StringRow, uMAX_COLS, strUnitTestName, "Player" ) )
		{
			if ( _stricmp( StringRow, "Red" ) == 0 )
			{
				eNextPlayer = CBoard::PLAYER1;
				m_uPlayerNumber = 0;
			} else if ( _stricmp( StringRow, "Yellow" ) == 0 )
			{
				eNextPlayer = CBoard::PLAYER2;
				m_uPlayerNumber = 1;
			} else
			{
				assert_now( "Unknown player (%s) for unit test \"%s\"", StringRow, strUnitTestName );
			}
		}

	}

	pParser->Release();

	m_pPlayers[m_uPlayerNumber]->StartChooseNextColumn( m_pBoard, eNextPlayer );

	return true;
}

CConnect4Context::~CConnect4Context( void )
{
	if ( m_pFrameTimer )
	{
		delete m_pFrameTimer;
	}
	if ( m_pPlayerTimer )
	{
		delete m_pPlayerTimer;
	}
	if ( m_pMouse )
	{
		delete m_pMouse;
	}
	// these have to delete before the board, incase they are holding clones.
	delete m_pPlayers[0];
	delete m_pPlayers[1];
	if ( m_pBoard )
	{
		m_pBoard->ShutdownCloneSystem();
		delete m_pBoard;
	}
	if ( m_pbUnitTestAllowedRes )
	{
		delete[] m_pbUnitTestAllowedRes;
	}
}


void CConnect4Context::CreateAndRegisterKey( const char* strKey, GDE::IInputChangeSubject& Subject, GDE::IObserver& Observer )
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


bool CConnect4Context::FrameUpdate( void )
{
	const static UINT32 colCollide = 0xFFFF0000;
	const static UINT32 colNonCollide = 0xFF00FF00;
	const static CFVec2 fv2ScreenExtents = SFVec2( (FLOAT32)GetDisplayWidth(), (FLOAT32)GetDisplayHeight() );

	m_pFrameTimer->FrameUpdate();
	const FLOAT32 fTimeInterval = m_pFrameTimer->GetFrameInterval(); // allows pausing.
	m_pMouse->FrameUpdate();
	m_pInputMgr->Update( fTimeInterval );

	// do all required updates here.
	if (false ==  m_pBoard->IsComplete() && m_pFrameTimer->GetFrameNum()>5 && false == m_bTestCompleted )
	{
		// don't update for the first few frames, allowing the board to be drawn in
		// unit test mode.
		int iChosenColumn;
		if ( m_pPlayers[m_uPlayerNumber]->IsNextColumnChosen( iChosenColumn ) )
		{
			if ( m_bUnitTesting )
			{
				m_bTestCompleted = true;
				m_bTestPassed = m_pbUnitTestAllowedRes[iChosenColumn];
			}
			if ( m_pBoard->MakeMove( iChosenColumn, m_uPlayerNumber?CBoard::PLAYER2:CBoard::PLAYER1 ) )
			{
				m_pPlayerTimer->FrameUpdate();
				m_uLastTurnEvaluations = m_pBoard->GetEvaluationCount();
				m_uPlayerScores[0] = m_pBoard->EvaluateScore( CBoard::PLAYER1 );
				m_uPlayerScores[1] = m_pBoard->EvaluateScore( CBoard::PLAYER2 );
				m_pBoard->ResetEvaluationCount();
				if ( false == m_pBoard->IsComplete() )
				{
					m_uPlayerNumber++;
					m_uPlayerNumber&=1;
					m_pPlayers[m_uPlayerNumber]->StartChooseNextColumn( m_pBoard, m_uPlayerNumber?CBoard::PLAYER2:CBoard::PLAYER1 );
				}
			} else
			{
				assert_now( "Illegal move attemted!" );
			}
		}
	}

	// now for all the rendering,
	m_pRenderer->BeginScene();
	m_pRenderer->ColourClear( 0xFF00007F );

	m_pBoard->Draw();

	m_pMouse->Draw();

//	if ( m_bDrawInfoText )
	{
		char InfoText[256];
		const FLOAT32 fFrameRate = 1.0f/m_pFrameTimer->GetFrameInterval();
		const UINT32 uFrameRate = UINT32(fFrameRate);
		const UINT32 uFrameDecimal = UINT32(fFrameRate*100.0f)%100;

		if ( m_bUnitTesting )
		{
			if ( m_bTestCompleted )
			{
				if ( m_bTestPassed )
				{
					sprintf_s( InfoText, "UNIT TEST PASSED" );
				} else
				{
					sprintf_s( InfoText, "UNIT TEST FAILED" );
				}
			} else
			{
				sprintf_s( InfoText, "UNIT TESTING" );
			}
			m_pFont->Render( 10, 520, GDE::FJ_Left, InfoText );
		}

		sprintf_s(  InfoText, "Evaluations: Last %d, Current %d", m_uLastTurnEvaluations, m_pBoard->GetEvaluationCount() );
		m_pFont->Render( 10, 540, GDE::FJ_Left, InfoText );

		sprintf_s(  InfoText, "Last turn took %0.2f seconds", m_pPlayerTimer->GetFrameInterval() );
		m_pFont->Render( 10, 560, GDE::FJ_Left, InfoText );

		if ( m_pBoard->IsComplete() )
		{
			sprintf_s(  InfoText, "Framerate %05d.%02d : Red %d, Yellow %d : Game complete!", uFrameRate, uFrameDecimal,
				m_uPlayerScores[0], m_uPlayerScores[1] );
		} else
		{
			sprintf_s(  InfoText, "Framerate %05d.%02d : Red %d, Yellow %d : %s to play", uFrameRate, uFrameDecimal,
				m_uPlayerScores[0], m_uPlayerScores[1], m_uPlayerNumber==0?"Red":"Yellow" );
		}
		m_pFont->Render( 10, 580, GDE::FJ_Left, InfoText );
	}

	m_pRenderer->EndScene();
	if ( m_bExitAfterTest && m_bTestCompleted )
	{
		return false;
	}
	return true;
}

void CConnect4Context::ReleaseResources( void )
{
	m_pFont->Release();
}

/**
 *	@param Circle	The circle to draw.
 *	@param uColour	The colour to draw with.
 */
void CConnect4Context::DrawCircle( const CCircle& Circle, UINT32 uColour )
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
void CConnect4Context::DrawFilledCircle( const CCircle& Circle, UINT32 uColour )
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


int CConnect4Context::GetReturnVal( void ) const
{
	if ( false == m_bUnitTesting )
	{
		return 0;
	}
	if ( m_bTestPassed )
	{
		return 0;
	}
	return 1;
}
