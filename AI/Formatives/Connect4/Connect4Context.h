/**
 *	@file Connect4\Connect4Context.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2012 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _CONNECT4CONTEXT_H__Included_2123735
#define _CONNECT4CONTEXT_H__Included_2123735


#include "GDE/Renderer/RendererContext.h"

// forward reference GDE members:
namespace GDE
{
	class CClock;
	class CRenderer;
	class CInputDevice;
	class IInputChangeSubject;
	class IObserver;
	class CDataParserManager;
	class CFontManager;
	class CFont;
};

// forward reference steering members:
class CMouse;
class CCircle;
class CGameBoard;
class CPlayerAgent;

/**
 *	The steering context, defining the application.
 */
class CConnect4Context : public GDE::IRendererContext
{
protected:
	CConnect4Context();	// it's a singleton..
public:
	static CConnect4Context& GetInstance( void );
	~CConnect4Context();
	bool Initialise( 
		GDE::CRenderer& Renderer, 
		GDE::CInputDevice& InputMgr, 
		GDE::CFontManager& FontManager,
		GDE::CDataParserManager& DataParserManager,
		const char* strININame );
	void ReleaseResources( void );
	void CreateAndRegisterKey( const char* strKey, GDE::IInputChangeSubject& Subject, GDE::IObserver& Observer );

	virtual bool FrameUpdate( void );

	GDE::CRenderer*	GetRenderer( void ) { return m_pRenderer; }
	const CMouse*	GetMouse( void ) const { return m_pMouse; }
	void DrawCircle( const CCircle& Circle, UINT32 uColour );
	void DrawFilledCircle( const CCircle& Circle, UINT32 uColour );
	int GetReturnVal( void ) const;
protected:
	CConnect4Context& operator=(const CConnect4Context& Other);	// non implemented assignment, fixes a warning.

	GDE::CRenderer*				m_pRenderer;			//!< The renderer used.
	GDE::CInputDevice*			m_pInputMgr;			//!< The input manager.
	GDE::CDataParserManager*	m_pParserManager;		//!< The data parser manager.

	GDE::CClock*				m_pFrameTimer;			//!< The frame timer, always at 'real' speed.
	GDE::CClock*				m_pPlayerTimer;			//!< A timer to time the last turn.
	GDE::CFont*					m_pFont;				//!< The current font.
	CMouse*						m_pMouse;				//!< The mouse.
	CGameBoard*					m_pBoard;				//!< The game board.
	CPlayerAgent*				m_pPlayers[2];			//!< The players.
	UINT32						m_uPlayerNumber;		//!< The current player number
	UINT32						m_uLastTurnEvaluations;	//!< Number of evaluations made for the last turn.
	UINT32						m_uPlayerScores[2];		//!< The current score of the players.
	bool						m_bUnitTesting;			//!< Is this running a unit test?
	bool						m_bTestPassed;			//!< Did the unit test pass?
	bool						m_bTestCompleted;		//!< Has the test completed?
	bool						m_bExitAfterTest;		//!< Quit after the unit test completed?
	bool*						m_pbUnitTestAllowedRes;	//!< Array of columns that are allowed results of the unit test.
};


#endif //_CONNECT4CONTEXT_H__Included_2123735

