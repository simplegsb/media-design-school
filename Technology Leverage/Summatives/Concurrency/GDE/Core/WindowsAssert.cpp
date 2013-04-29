/**
 *	@file GDE\Core\WindowsAssert.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Core.h"
#include "WindowsAssert.h"

#include <stdio.h>
#ifndef __GNUC__
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#endif

#define __SHOW_FUNCTION_NAME

namespace GDE
{

// TODO: make this thread safe please...
const static UINT32 uMESSAGE_SIZE = 1024;

CWindowsAssertQuery& CWindowsAssertQuery::GetInstance( void )
{
	static CWindowsAssertQuery sInstance;
	return sInstance;
}

CWindowsAssertQuery::CWindowsAssertQuery( void )
: m_bBreak( false )
, m_bAlwaysSkip( false )
{	// empty
}

const CLibraryDescription&	CWindowsAssertQuery::GetDesc( void ) const
{
	static const CLibraryDescription Desc = CREATE_LIBRARY_DESCRIPTION (
		'A','S','R','T',
		"Windows message box assert",
		1,0,0);		// version
	return Desc;
}

void CWindowsAssertQuery::AskAssert( const char* strFileName, UINT32 uLine, const char* strFunction, const char* strMessage )
{
#ifndef __GNUC__
	m_bBreak = false;
	m_bAlwaysSkip = false;

	static char AssertMessage[uMESSAGE_SIZE];
# ifdef __SHOW_FUNCTION_NAME
	_snprintf_s( AssertMessage, uMESSAGE_SIZE, uMESSAGE_SIZE, "%s(%d,%s) %s\n  Yes to break, No to ignore, Cancel to ignore always", strFileName, uLine, strFunction, strMessage );
# else
	_snprintf_s( AssertMessage, uMESSAGE_SIZE, uMESSAGE_SIZE, "%s(%d) %s\n  Yes to break, No to ignore, Cancel to ignore always", File, uLine, strMessage );
# endif
	
	int iRes = ::MessageBoxA( NULL, AssertMessage, "Assert Failed", MB_YESNOCANCEL | MB_ICONERROR );
	switch( iRes )
	{
	case IDYES:
		m_bBreak = true;
		break;
	case IDNO:
		// ignore once
		break;
	case IDCANCEL:
		m_bAlwaysSkip = true;
		break;
	}
#else
	// just break.
	m_bBreak = true;
	m_bAlwaysSkip = false;
#endif
}

} // end namespace GDE
