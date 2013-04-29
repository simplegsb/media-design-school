/**
 *	@file GDE\Core\DebuggerLog.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/

#include "Core.h"
#include "DebuggerLog.h"

#ifndef __GNUC__ // not on gcc, instead we just printf..
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#else
# include <stdio.h>
#endif

using namespace GDE;

/**
 *	Implement the Meyers Singleton pattern.
 *
 *	@return The one and only instance of this.
 */
CDebuggerLog& CDebuggerLog::GetInstance( void )
{
	static CDebuggerLog s_Instance;
	return s_Instance;
}

/**
 *	@return the unique description of this implementation.
 */
const CLibraryDescription& CDebuggerLog::GetDesc( void ) const
{
#ifndef __GNUC__ // not on gcc, instead we just printf..
# define _LIB_DESC_		"Visual Studio Debugger Log"
#else
# define _LIB_DESC_		"TTY (stdout) Debugger Log"
#endif
	static const CLibraryDescription Desc = CREATE_LIBRARY_DESCRIPTION(
		'L', 'O', 'G', 'R',
		_LIB_DESC_,
		1,0,0);		// version
	return Desc;
}

/**
 *	This function prints out data to the log output.
 *
 *	@returns Success.
 */
bool CDebuggerLog::Write( const char* strMessage )
{
	if ( NULL == strMessage )
	{
		return false;
	}
#ifndef __GNUC__ // not on gcc, instead we just printf..
	OutputDebugStringA( strMessage );
#else
	printf( strMessage );
#endif
	return true;
}

/**
 *	This function will reset or clear the contents of the log.
 *	NOTE: currently unimplemented.
 *
 *	@returns Success.
 */
bool CDebuggerLog::Reset( void )
{
	return false;
}
