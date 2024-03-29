/**
 *	@file Connect4\Mouse.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2004-2012 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/

#ifndef _MOUSE_H__Included_1578817441
#define _MOUSE_H__Included_1578817441

#include "FVector2.h"
#include "GDE/Core/Observer.h"

class CCircle;

class CMouse : public GDE::IObserver
{
public:
	CMouse( void );
	void Draw( void );
	void GetBoundingCircle( CCircle& Bounds ) const;
	void GetPosition( CFVec2& vPos ) const;
	bool WasClicked( void ) const;
	void FrameUpdate( void );

	// for the observer.
	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage );
protected:
	CFVec2	m_vPosition;
	FLOAT32	m_fVisualRadius;
	bool	m_bWasClicked;		//Was the mouse clocked.
	void operator = (const CMouse& Other); // remove warning, not implemented.
};

#endif //_MOUSE_H__Included_1578817441

