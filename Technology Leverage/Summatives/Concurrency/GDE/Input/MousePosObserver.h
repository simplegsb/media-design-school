/**
 *	@file GDE\GDE\Input\MousePosObserver.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _MOUSEPOSOBSERVER_H__Included_1628411425
#define _MOUSEPOSOBSERVER_H__Included_1628411425

#include "GDE/Core/Observer.h"
#include "GDE/Maths/FVector2.h"

namespace GDE
{

class CInputDevice;

/**
 *	@ingroup Input
 *
 *	The mouse observer watches the mouse position.
 */
class CMousePosObserver : public GDE::IObserver
{
public:
	CMousePosObserver(void);

	// Implement Observer interface:
	virtual ~CMousePosObserver(void);
	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage);

	bool Register( CInputDevice& Device );

	void FrameUpdate( FLOAT32 fTimeValue );

	CFVec2		GetDelta( void ) const;
	CFVec2Arg	GetPosition( void ) const;
	CFVec2Arg	GetLastPosition( void ) const;
protected:
	CMousePosObserver& operator=(const CMousePosObserver& Other);	// non implemented assignment, fixes a warning.
	
	CFVec2	m_vPosition;
	CFVec2	m_vLastPosition;
};

}	// end namespace GDE


#endif //_MOUSEPOSOBSERVER_H__Included_1628411425

