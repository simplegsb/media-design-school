/**
 *	@file GDE\GDE\Input\ButtonObserver.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _BUTTONOBSERVER_H__Included_1565379451
#define _BUTTONOBSERVER_H__Included_1565379451

#include "GDE/Core/Observer.h"

namespace GDE
{

class CInputDevice;

/**
 *	@ingroup Input
 *
 *	The button observer listens for button messages and can then be polled for the current state
 *	of the button.
 */
class CButtonObserver : public GDE::IObserver
{
public:
	CButtonObserver(void);

	// Implement Observer interface:
	virtual ~CButtonObserver(void);
	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage);

	bool RegisterButton( CInputDevice& Device, const char* strCode );

	void FrameUpdate( FLOAT32 fTimeValue );

	bool WasPressed( void ) const;
	bool IsDown( void ) const;
	bool WasReleased( void ) const;
protected:
	CButtonObserver& operator=(const CButtonObserver& Other);	// non implemented assignment, fixes a warning.

	bool	m_bDown;		//!< Is the button currently down.
	bool	m_bWasDown;		//!< Was the button down at the last FrameUpdate?
};

}	// end namespace GDE

#endif //_BUTTONOBSERVER_H__Included_1565379451

