/**
 *	@file GDE\GDE\Input\ButtonObserver.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "ButtonObserver.h"


#include "GDE/Input/InputDevice.h"
#include "GDE/Input/InputChangeSubject.h"
#include "GDE/Input/InputStates.h"


using namespace GDE;

CButtonObserver::CButtonObserver( void )
: m_bDown( false )
, m_bWasDown( false )
{	// empty.
}

CButtonObserver::~CButtonObserver( void )
{	// empty
}

/**
 *	Message passed to say the button being watched has changed in state.
 */
void CButtonObserver::NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage)
{
	const GDE::CInputDevice& Device = reinterpret_cast<GDE::IInputChangeSubject*>(pSubject)->GetDevice();	
	const GDE::SInputState& State = Device.GetState( uMessage );
	m_bDown = State.m_bButtonDown;
}

/**
 *	Register this observer to listen for changes.
 *
 *	@param Device	The input device to listen on
 *	@param strCode	The message code to listen for.
 *	@return			Success.
 */
bool CButtonObserver::RegisterButton( CInputDevice& Device, const char* strCode )
{
	// firstly get the code:
	UINT32 uMessage;
	if ( Device.GetInputCode( strCode, uMessage ) )
	{
		return Device.GetChangeSubject().AddObserver( this, uMessage );
	}
	return false;
}

/**
 *	Update the button for a given frame.
 *	Used to update to detect if the button is held.
 */
void CButtonObserver::FrameUpdate( FLOAT32 fTimeValue )
{
	// TODO update a countdown, to detect double clicks etc
	UNUSED_VAR( fTimeValue );
	m_bWasDown = m_bDown;
}


/**
 *	@return true if the button was pressed on the last frame.
 */
bool CButtonObserver::WasPressed( void ) const
{
	return m_bDown && false == m_bWasDown;
}

/**
 *	@return true if the button is currently down.
 */
bool CButtonObserver::IsDown( void ) const
{
	return m_bDown;
}

/**
 *	@return true if the button was released on the last frame.
 */
bool CButtonObserver::WasReleased( void ) const
{
	return m_bWasDown && false == m_bDown;
}

