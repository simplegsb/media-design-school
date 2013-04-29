/**
 *	@file GDE\GDE\Input\CMousePosObserver.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "MousePosObserver.h"


#include "GDE/Input/InputDevice.h"
#include "GDE/Input/InputChangeSubject.h"
#include "GDE/Input/InputStates.h"


using namespace GDE;

CMousePosObserver::CMousePosObserver( void )
: m_vPosition( SFVec2( 0.0f, 0.0f ) )
, m_vLastPosition( SFVec2( 0.0f, 0.0f ) )
{	// empty.
}

CMousePosObserver::~CMousePosObserver( void )
{	// empty
}

/**
 *	Message passed to say the button being watched has changed in state.
 */
void CMousePosObserver::NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage)
{
	const GDE::CInputDevice& Device = reinterpret_cast<GDE::IInputChangeSubject*>(pSubject)->GetDevice();	
	const GDE::SInputState& State = Device.GetState( uMessage );
	m_vPosition = SFVec2( State.m_fValue, State.m_fValue2 );
}

/**
 *	Register this observer to listen for changes.
 *
 *	@param Device	The input device to listen on
 *	@return			Success.
 */
bool CMousePosObserver::Register( CInputDevice& Device )
{
	// firstly get the code:
	UINT32 uMessage;
	if ( Device.GetInputCode( "MouseXY", uMessage ) )
	{
		return Device.GetChangeSubject().AddObserver( this, uMessage );
	}
	return false;
}

/**
 *	Update the button for a given frame.
 *	Used to update to detect if the button is held.
 */
void CMousePosObserver::FrameUpdate( FLOAT32 fTimeValue )
{
	// TODO update a countdown, to detect gestures?
	UNUSED_VAR( fTimeValue );
	m_vLastPosition = m_vPosition;
}

/**
 *	@return the difference in mouse position, last frame to this frame.
 */
CFVec2		CMousePosObserver::GetDelta( void ) const
{
	return m_vPosition-m_vLastPosition;
}

/**
 *	@return	The current mouse position.
 */
CFVec2Arg	CMousePosObserver::GetPosition( void ) const
{
	return m_vPosition;
}

/**
 *	@return The mouse position on the last frame
 */
CFVec2Arg	CMousePosObserver::GetLastPosition( void ) const
{
	return m_vLastPosition;
}

