/**
 *	@file GDE\Input\StateChangeSubject.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "StateChangeSubject.h"
#include "InputDevice.h"

using namespace GDE;

CStateChangeSubject::CStateChangeSubject( CInputDevice& Device )
: CInputDevice::IStateChangeSubject( Device )
, m_Observers( m_Device.GetStateCount() )
, m_uCurrentNotification( 0xFFFFFFFF )
//, m_ObserversToAdd( 4 )
//, m_ObserversToRemove( 4 )
{	// empty
}

/**
 *	Add an observer for all states.
 *	@param pObserver	The observer to add.
 *	@return				Success if all states had the observer added OK. @see AddObserver.
 */
bool CStateChangeSubject::AddObserverForAll( IObserver* pObserver )
{
	bool bOK = true;
	for ( UINT32 i=0; i<m_Device.GetStateCount(); ++i )
	{
		bOK |= FinalAddObserver( pObserver, i );
	}
	return bOK;
}

/**
 *	Remove an observer for all states.
 *	@param pObserver	The observer to add.
 *	@return				Success if all states had the observer added OK. @see AddObserver.
 */
bool CStateChangeSubject::RemoveObserverForAll( IObserver* pObserver )
{
	bool bOK = true;
	for ( UINT32 i=0; i<m_Device.GetStateCount(); ++i )
	{
		bOK |= FinalRemoveObserver( pObserver, i );
	}
	return bOK;
}

/**
 *	Add an observer.
 *	@param pObserver	The observer to add.
 *	@param uMessage		The message to watch.
 *	@return				Success. Fails if the observer is already present for the state, or if the state is invalid.
 */
bool CStateChangeSubject::AddObserver( IObserver* pObserver, UINT32 uMessage )
{
	return FinalAddObserver( pObserver, uMessage );
}

/**
 *	Remove an observer.
 *	@param pObserver	The observer to remove.
 *	@param uMessage		The message to stop watching.
 *	@return				Success. Fails if the observer is not present for the state, or if the state is invalid.
 */
bool CStateChangeSubject::RemoveObserver( IObserver* pObserver, UINT32 uMessage )
{
	return FinalRemoveObserver( pObserver, uMessage );
}

/**
 *	Add an observer for a state.
 *	Note: this is protected and only used to implement the above.
 */
bool CStateChangeSubject::FinalAddObserver( IObserver* pObserver, UINT32 uMessage )
{
	assert_msg( uMessage < m_Device.GetStateCount(), "Message to watch %d is not allowed", uMessage );
	if ( uMessage >= m_Device.GetStateCount() )
	{
		return false;
	}
	// check it isn't in already:
	UINT32 uPos = GetObserverPos( pObserver, uMessage );
	if ( uPos < m_Device.GetStateCount() )
	{
		return false;	// it's already in the observer list
	}
	if ( uMessage != m_uCurrentNotification )
	{
		m_Observers[uMessage].push_back( pObserver );
	} else
	{
		m_ObserversToAdd.push_back( pObserver );
	}
	return true;
}

/**
 *	Remove an observer for a state.
 *	Note: this is protected and only used to implement the above.
 */
bool CStateChangeSubject::FinalRemoveObserver( IObserver* pObserver, UINT32 uMessage )
{
	assert_msg( uMessage < m_Device.GetStateCount(), "Message to watch %d is not allowed", uMessage );
	if ( uMessage >= m_Device.GetStateCount() )
	{
		return false;
	}
	UINT32 uPos = GetObserverPos( pObserver, uMessage );
	if ( uPos < m_Device.GetStateCount() )
	{
		if ( uMessage != m_uCurrentNotification )
		{
			m_Observers[uMessage].erase( m_Observers[uMessage].begin()+uPos );
		} else
		{
			m_ObserversToRemove.push_back( pObserver );
		}
		return true;
	}
	return false;	
}

/**
 *	@return the position in the observer array of the given observer or UINT32_MAX if not present.
 */
UINT32 CStateChangeSubject::GetObserverPos( IObserver* pObserver, UINT32 uMessage )
{
	for ( UINT32 i=0; i<m_Observers[uMessage].size(); ++i )
	{
		if ( pObserver == m_Observers[uMessage][i] )
		{
			return i;
		}
	}
	return UINT32_MAX;
}

/**
 *	Notify everyone waiting for the specified message.
 */
void CStateChangeSubject::Notify( UINT32 uMessage )
{
	for ( UINT32 i=0; i<m_Observers[uMessage].size(); ++i )
	{
		m_Observers[uMessage][i]->NotifyChange( this, uMessage );
	}
}

/**
 *	@return true if there is an observer for the given state.
 */
bool CStateChangeSubject::StateIsObserved( UINT32 uMessage ) const
{
	return m_Observers[uMessage].size() > 0;
}

void CStateChangeSubject::SetNotifying( UINT32 uMessage )
{
	UINT32 uOld = m_uCurrentNotification;
	m_uCurrentNotification = uMessage; 
	if ( m_uCurrentNotification != 0xFFFFFFFF  )
	{
		for ( UINT32 i=0; i<m_ObserversToAdd.size(); i++ )
		{
			FinalAddObserver( m_ObserversToAdd[i], uOld );
		}
		for ( UINT32 i=0; i<m_ObserversToRemove.size(); i++ )
		{
			FinalRemoveObserver( m_ObserversToRemove[i], uOld );
		}
		m_ObserversToAdd.erase(m_ObserversToAdd.begin(), m_ObserversToAdd.end());
		m_ObserversToRemove.erase(m_ObserversToRemove.begin(), m_ObserversToRemove.end());
	}
}
