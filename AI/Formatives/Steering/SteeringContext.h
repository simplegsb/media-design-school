/**
 *	@file AI_Steering\SteeringContext.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _STEERINGCONTEXT_H__Included_214923253
#define _STEERINGCONTEXT_H__Included_214923253

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
class CTree;
class CChangeSpeedOfTimeObserver;
class CWall;
class CCircle;
class CCat;

/**
 *	The steering context, defining the application.
 */
class CSteeringContext : public GDE::IRendererContext
{
protected:
	CSteeringContext();	// it's a singleton..
public:
	static CSteeringContext& GetInstance( void );
	~CSteeringContext();
	bool Initialise( 
		GDE::CRenderer& Renderer, 
		GDE::CInputDevice& InputMgr, 
		GDE::CFontManager& FontManager,
		GDE::CDataParserManager& DataParserManager );
	void ReleaseResources( void );
	void CreateAndRegisterKey( const char* strKey, GDE::IInputChangeSubject& Subject, GDE::IObserver& Observer );

	virtual bool FrameUpdate( void );

	GDE::CRenderer*	GetRenderer( void ) { return m_pRenderer; }
	void DrawCircle( const CCircle& Circle, UINT32 uColour );
	void DrawFilledCircle( const CCircle& Circle, UINT32 uColour );
protected:
	CSteeringContext& operator=(const CSteeringContext& Other);	// non implemented assignment, fixes a warning.

	GDE::CRenderer*				m_pRenderer;			//!< The renderer used.
	GDE::CInputDevice*			m_pInputMgr;			//!< The input manager.
	GDE::CDataParserManager*	m_pParserManager;		//!< The data parser manager.

	CChangeSpeedOfTimeObserver*	m_pPitchUpSpeedObsrv;	//!< Observer to slow down time.
	CChangeSpeedOfTimeObserver*	m_pPitchDownSpeedObsrv;	//!< Observer to speed up time.

	GDE::CClock*				m_pClock;				//!< The game clock (can be sped up/slowed down).
	GDE::CClock*				m_pFrameTimer;			//!< The frame timer, always at 'real' speed.
	GDE::CFont*					m_pFont;				//!< The current font.
	CWall*						m_pWalls;				//!< Array of walls in the scene.
	UINT32						m_uWallCount;			//!< How many walls are there in the scene?
	CMouse*						m_pMouse;				//!< The mouse representation.
	CTree*						m_pTrees;				//!< Array of trees in the scene.
	UINT32						m_uTreeCount;			//!< Number of trees in the scene.
	CCat*						m_pCats;				//!< Array of cats in the scene.
	UINT32						m_uCatCount;			//!< Number of cats in the scene.

	void IntersectionUpdate( void );
};

#endif //_STEERINGCONTEXT_H__Included_214923253

