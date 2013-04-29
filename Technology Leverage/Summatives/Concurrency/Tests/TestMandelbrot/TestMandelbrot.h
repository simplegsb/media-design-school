/**
 *	@file GDE\Tests\TestMandelbrot\TestMandelbrot.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _TESTMESHCALCULATION_H__Included_1107682990
#define _TESTMESHCALCULATION_H__Included_1107682990

#include "KeyboardObserver.h"
#include "GDE/Renderer/RendererContext.h"

#include "GDE/DataParser/DataParserManager.h"
#include "GDE/DataParser/DataParser.h"

#include <vector>
#include "Task.h"

// forward references
class CMandelbrotBuffer;;
namespace GDE
{
	class CRenderer;
	class CTexture;
	class CInputDevice;
	class CClock;
	class IInputChangeSubject;
	class CFont;
	class CFontManager;
	class CTexture;
};

/** 
 *	The application context to contain the mesh calculation stuff.
 */
class CMandelbrotContext : public GDE::IRendererContext
{
public:
	CMandelbrotContext( GDE::CRenderer& Renderer, GDE::CDataParserManager& ParserManager );
	~CMandelbrotContext( void );

	void Init( void );
	void Release( void );

	virtual bool FrameUpdate( void );

protected:
	void UpdateTexture( void );
	CMandelbrotContext& operator=(const CMandelbrotContext& Other);	// non implemented assignment, fixes a warning.
	void CreateAndRegisterKey( const char* strKey, GDE::IInputChangeSubject& Subject, 
		GDE::IObserver& Observer );
	void UpdateKeyStates( void );
	void RenderBoxes( void );
	TTask CreateTask( GDE::UINT32 _uiTaskNumber );

	GDE::CRenderer&				m_Renderer;
	GDE::CInputDevice*			m_pInputKeyboard;
	GDE::CDataParserManager*	m_pParserManager;		//!< The data parser manager.

	// Keyboard controls.
	enum EKeyoardCommands 
	{
		KC_Quit,
		KC_Count			//!< Must come last, the number of keyboard commands to toggle bools.
	};
	CKeyboardBoolToggle			m_KeyObserver[KC_Count];
	bool						m_bKeyDown[KC_Count];
	bool						m_bKeyWasDown[KC_Count];

	GDE::CClock*				m_pFrameTimer;
	GDE::CFontManager*			m_pFontMgr;				//!< Font manager.
	GDE::CFont*					m_pFont;				//!< Font, used for debug info.
	GDE::CTexture*				m_pMandelbrotTexture;	//!< The texture for the mandelbrot.
	CMandelbrotBuffer*			m_pMandelbrot;			//!< The mandelbrot.

	std::vector<TTask>			m_tasks;
	GDE::UINT32                 m_uiTaskCount;
	GDE::UINT32                 m_uiThreadCount;
};


#endif //_TESTMESHCALCULATION_H__Included_1107682990

