/**
 *	@file GDE\Tests\TestMandelbrot\KeyboardObserver.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "KeyboardObserver.h"

using namespace GDE;

CKeyboardObserver::CKeyboardObserver( bool& bValueToChange, bool bValueToSet)
: m_bValueToChange( bValueToChange )
, m_bValueToSet( bValueToSet )
{
	m_bValueToChange = false;
}

CKeyboardObserver::~CKeyboardObserver( void )
{
}

void CKeyboardObserver::NotifyChange( ISubject* pSubject, GDE::UINT32 uMessage )
{
	UNUSED_VAR( uMessage );
	UNUSED_VAR( pSubject );
	m_bValueToChange = m_bValueToSet;
}

CKeyboardBoolToggle::~CKeyboardBoolToggle( void )
{
	// empty
}

void CKeyboardBoolToggle::Init( bool &bValueToToggle )
{
	m_pbValue = &bValueToToggle;
}

void CKeyboardBoolToggle::NotifyChange( ISubject* pSubject, GDE::UINT32 uMessage )
{
	UNUSED_VAR( uMessage );
	UNUSED_VAR( pSubject );
	*m_pbValue = !*m_pbValue;
}

void	CKeyboardUINT32Set::Init( GDE::UINT32& uValueToSet, GDE::UINT32 uValue )
{
	m_puValue = &uValueToSet;
	m_uValue = uValue;
}

CKeyboardUINT32Set::~CKeyboardUINT32Set( void )
{
	// empty
}

void CKeyboardUINT32Set::NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage)
{
	UNUSED_VAR( uMessage );
	UNUSED_VAR( pSubject );
	*m_puValue = m_uValue;
}
