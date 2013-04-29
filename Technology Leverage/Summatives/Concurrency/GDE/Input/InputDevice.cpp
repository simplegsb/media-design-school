/**
 *	@file GDE\Input\InputDevice.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "InputDevice.h"
#include "InputDeviceImp.h"
#include "StateChangeSubject.h"

namespace GDE
{
/**
 *	@defgroup Input Input Library
 *
 *	This library abstracts input systems using an event handling system
 *	and not polling. The design of the input system itself is for event management.
 *	If an implementation calls for polling, then it should be implemented on top
 *	of the event management system presented here, not replacing this.
 *
 *	The main class in the CInputDevice. This can map to either a keyboard, mouse
 *	or controller device quite simply.
 *
 *	It is conceviable that a GUI could also implement a CInput device where the
 *	GUI creates specific events related to options being selected.
 *
 *	Event handling is managed via an observer system.
 *
 *	g_DefaultKeyNames is simply for information, it is a list of the default
 *	key names for keys on the keyboard. Any input device, should, of course,
 *	be queried to see what messages it supports.
 */
} // end namespace GDE (for documentation purposes).

using namespace GDE;

const SInputDesc	CInputDevice::sm_InvalidDesc =
{
	IST_Count,	"NULL", "Invalid input state", 0.0f, 0.0f 
};

const SInputState	CInputDevice::sm_InvalidState =
{
	false, false, 0.0f
};


CInputDevice::CInputDevice( void )
:TSuperClass()
, m_pButtonDownSubject( NULL )
, m_pButtonUpSubject( NULL )
, m_pStateChangedSubject( NULL )
, m_uStateCount( 0 )
, m_uCurrentFrame( 0 )
, m_pStates( NULL )
, m_pDescriptions( NULL )
{	// empty
}

CInputDevice::~CInputDevice( void )
{
	if ( m_pStates )
	{
		delete[] m_pStates;
		m_pStates = NULL;
	}
	if ( m_pDescriptions )
	{
		delete[] m_pDescriptions;
		m_pDescriptions = NULL;
	}

	if ( m_pButtonDownSubject )
	{
		delete m_pButtonDownSubject;
	}
	if ( m_pButtonUpSubject )
	{
		delete m_pButtonUpSubject;
	}
	if ( m_pStateChangedSubject )
	{
		delete m_pStateChangedSubject;
	}
}


/** 
 *	Override parent to load the DLL and initialize this input manager based on it.
 */
bool CInputDevice::LoadDLL( const char* strDLLName )
{
	if ( TSuperClass::LoadDLL( strDLLName ) )
	{
		// now initialize this.
		m_uStateCount = m_pImplementer->GetInputCount();
		m_pStates = new SInputState[m_uStateCount*2];
		m_pDescriptions = new SInputDesc[m_uStateCount];

		// initialize all the states (to not having been watched).
		for ( UINT32 i=0; i<m_uStateCount*2; i++ )
		{
			m_pStates[i].m_bWatched = false;
			m_pStates[i].m_bButtonDown = false;
			m_pStates[i].m_fValue = 0.0f;
		}

		m_pImplementer->GetInputDescriptions( m_pDescriptions );

		m_pButtonDownSubject = new CStateChangeSubject( *this );
		m_pButtonUpSubject = new CStateChangeSubject( *this );
		m_pStateChangedSubject  = new CStateChangeSubject( *this );
		return true;
	}
	return false;
}		

/**
 *	Update the input device for the given time update.
 */
void CInputDevice::Update( FLOAT32 fTimeDelta )
{
	m_pImplementer->Update( fTimeDelta );

	// now we need to update where we store the states.
	++m_uCurrentFrame;
	m_uCurrentFrame&=1;

	// pull in the new, updated states:
	SInputState* pCurrentState = GetCurrentState();

	// setup which states are observed now:
	for ( UINT32 i=0; i<m_uStateCount; i++ )
	{
		pCurrentState[i].m_bWatched = 
			m_pButtonDownSubject->StateIsObserved(i) ||
			m_pButtonUpSubject->StateIsObserved(i) ||
			m_pStateChangedSubject->StateIsObserved(i);
	}

	m_pImplementer->GetInputStates( pCurrentState );

	// loop through all the states, updating as required:
	const SInputState* pPrevState = GetPreviousState();
	for ( UINT32 i=0; i<m_uStateCount; i++ )
	{
		if ( pCurrentState[i].m_bWatched && pPrevState[i].m_bWatched )
		{
			m_pButtonDownSubject->SetNotifying( i );
			m_pButtonUpSubject->SetNotifying( i );
			m_pStateChangedSubject->SetNotifying( i );
			// process this state:
			if ( m_pDescriptions[i].m_eType == IST_Button )
			{
				// deal with button observers.
				if ( pCurrentState[i].m_bButtonDown != pPrevState[i].m_bButtonDown )
				{
					if ( pCurrentState[i].m_bButtonDown )
					{
						m_pButtonDownSubject->Notify( i );
					} else
					{
						m_pButtonUpSubject->Notify( i );
					}
				}
			}
			// deal with other observers.
			if ( pCurrentState[i] != pPrevState[i] )
			{
				m_pStateChangedSubject->Notify( i );
			}
		}
	}
	m_pButtonDownSubject->SetNotifying( 0xFFFFFFFF );
	m_pButtonUpSubject->SetNotifying( 0xFFFFFFFF );
	m_pStateChangedSubject->SetNotifying( 0xFFFFFFFF );
}

/**
 *	Get the input code for a given state.
 *	@param strName	The name of the state to get the code for.
 *	@param uCode	On success filled in with the code for strName.
 *	@return			Success.
 */
bool CInputDevice::GetInputCode( const char* strName, GDE::UINT32& uCode ) const
{
	for ( UINT32 i=0; i<m_uStateCount; i++ )
	{
		if ( strcmp( strName, m_pDescriptions[i].m_strName ) == 0 )
		{
			uCode = i;
			return true;
		}
	}
	return false;
}

/**
 *	@return A observer subject to watch for down messages.
 */
CInputDevice::IStateChangeSubject& CInputDevice::GetDownSubject( void )
{
	return *m_pButtonDownSubject;
}

/**
 *	@return A observer subject to watch for up messages.
 */
CInputDevice::IStateChangeSubject& CInputDevice::GetUpSubject( void )
{
	return *m_pButtonUpSubject;
}

/**
 *	@return A observer subject to watch for change messages (could be up, down or analogue).
 */
CInputDevice::IStateChangeSubject& CInputDevice::GetChangeSubject( void )
{
	return *m_pStateChangedSubject;
}

/**
 *	Convenience to get the state array for the current frame.
 *	@return	The state array for the current frame.
 */
SInputState* CInputDevice::GetCurrentState( void )
{
	return m_pStates + (m_uCurrentFrame*m_uStateCount);
}

/**
 *	Convenience to get the state array for the previous frame.
 *	@return	The state array for the previous frame.
 */
SInputState* CInputDevice::GetPreviousState( void )
{
	return m_pStates + (((m_uCurrentFrame+1)&1)*m_uStateCount);
}

/**
 *	Convenience to get the state array for the current frame.
 *	@return	The state array for the current frame.
 */
const SInputState* CInputDevice::GetCurrentState( void ) const
{
	return m_pStates + (m_uCurrentFrame*m_uStateCount);
}

/**
 *	Convenience to get the state array for the previous frame.
 *	@return	The state array for the previous frame.
 */
const SInputState* CInputDevice::GetPreviousState( void ) const
{
	return m_pStates + (((m_uCurrentFrame+1)&1)*m_uStateCount);
}

/**
 *	@return The number of states this device has.
 */
UINT32 CInputDevice::GetStateCount( void ) const
{
	return m_uStateCount;
}

/**
 *	@return The current state of the given code or sm_InvalidState.
 *	@see sm_InvalidState;
 */
const SInputState&	CInputDevice::GetState( UINT32 uCode ) const
{
	if ( uCode >= m_uStateCount )
	{
		return sm_InvalidState;
	}
	return GetCurrentState()[uCode];
}

/**
 *	@return The previous state of the given code or sm_InvalidState.
 *	@see sm_InvalidState;
 */
const SInputState&	CInputDevice::GetLastState( UINT32 uCode ) const
{
	if ( uCode >= m_uStateCount )
	{
		return sm_InvalidState;
	}
	return GetPreviousState()[uCode];
}

/**
 *	@return The input state description for the given code, or sm_InvalidDesc.
 *	@see sm_InvalidDesc.
 */
const SInputDesc&	CInputDevice::GetInputDesc( UINT32 uCode ) const
{
	if ( uCode >= m_uStateCount )
	{
		return sm_InvalidDesc;
	}
	return m_pDescriptions[uCode];
}
