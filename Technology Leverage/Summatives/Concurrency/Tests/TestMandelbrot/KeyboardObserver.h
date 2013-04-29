/**
 *	@file GDE\Tests\TestMandelbrot\KeyboardObserver.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _KEYBOARDOBSERVER_H__Included_1987389589
#define _KEYBOARDOBSERVER_H__Included_1987389589

#include "GDE/Core/Observer.h"

/**
 *	Observe a keyboard press and then set a bool to the value given.
 */
class CKeyboardObserver : public GDE::IObserver
{
public:
	CKeyboardObserver( bool& bValueToChange, bool bValueToSet );
	virtual ~CKeyboardObserver( void );

	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage);

protected:
	CKeyboardObserver& operator=(const CKeyboardObserver& Other);	// non implemented assignment, fixes a warning.
	bool& m_bValueToChange;
	bool m_bValueToSet;
};

/**
 *	On the event happening, toggle a bool.
 */
class CKeyboardBoolToggle :  public GDE::IObserver
{
public:
	void	Init( bool& bValueToToggle );
	virtual ~CKeyboardBoolToggle( void );
	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage);
protected:
	bool*	m_pbValue;	//!< The value to toggle.
};

/**
 *	On the event happening, set a UINT32.
 */
class CKeyboardUINT32Set :  public GDE::IObserver
{
public:
	void	Init( GDE::UINT32& uValueToSet, GDE::UINT32 uValue );
	virtual ~CKeyboardUINT32Set( void );
	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage);
protected:
	GDE::UINT32*	m_puValue;	//!< The value to be set.
	GDE::UINT32		m_uValue;	//!< The value to set.
};


#endif //_KEYBOARDOBSERVER_H__Included_1987389589

