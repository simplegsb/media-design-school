/**
 *	@file Connect4\Mouse.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2012 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/

#include "Common.h"
#include "Mouse.h"
#include "Connect4Context.h"
#include "Circle.h"

#include "GDE/Input/InputDevice.h"
#include "GDE/Input/InputChangeSubject.h"
#include "GDE/Input/InputStates.h"

CMouse::CMouse( void )
: m_vPosition( SFVec2( 0.0f, 0.0f ) )
, m_fVisualRadius( 5.0f )
, m_bWasClicked( false )
{	// empty
}
/**
 *	Update the mouse position based on the input device.
 *	Requires to be listening to a mouseXY state.
 */
void CMouse::NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
{
	GDE::CInputDevice& Device = reinterpret_cast<GDE::IInputChangeSubject*>(pSubject)->GetDevice();	
	if ( reinterpret_cast<GDE::ISubject*>(&Device.GetChangeSubject()) == pSubject )
	{
		const GDE::SInputState& State = Device.GetState( uMessage );
		m_vPosition.X( State.m_fValue );
		m_vPosition.Y( State.m_fValue2 );
	} else
	{
		m_bWasClicked = true;
	}

}

/**
 *	Must be called before the input manager is updated to clear click status.
 */
void CMouse::FrameUpdate( void )
{
	m_bWasClicked = false;
}

/**
 *	Draw the mouse
 *	TODO: better drawing!
 */
void CMouse::Draw( void )
{
	CCircle Rep;
	GetBoundingCircle( Rep );
	CConnect4Context::GetInstance().DrawCircle( Rep, 0xFF00FF00 );
}

void CMouse::GetBoundingCircle( CCircle& Bounds ) const
{
	Bounds.Initialise( m_vPosition, m_fVisualRadius );
}

void CMouse::GetPosition(CFVec2 &vPos) const
{
	vPos = m_vPosition;
}

bool CMouse::WasClicked( void ) const
{
	return m_bWasClicked;
}

