/**
 *	@file GDE\Core\ConsoleLog.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/

#include "Core.h"
#include "ConsoleLog.h"
#include "DLLInterface.h"

#include <stdio.h>

using namespace GDE;

/**
 *	Implement the Meyers Singleton pattern.
 *
 *	@return The one and only instance of this.
 */
CConsoleLog& CConsoleLog::GetInstance( void )
{
	static CConsoleLog s_Instance;
	return s_Instance;
}

/**
 *	@return the unique description of this implementation.
 */
const CLibraryDescription&	CConsoleLog::GetDesc( void ) const
{
	static const CLibraryDescription Desc = CREATE_LIBRARY_DESCRIPTION(
		'L', 'O', 'G', 'R',
		"Console Log",
		1,0,0);		// version
	return Desc;
}

/**
 *	This function prints out data to the log output.
 *
 *	@returns Success.
 */
bool CConsoleLog::Write( const char* strMessage )
{
	if ( NULL == strMessage )
	{
		return false;
	}
	printf( strMessage );
	return true;
}

/**
 *	This function will reset or clear the contents of the log.
 *	NOTE: currently unimplemented, in theory it should run a cls or similar.
 *
 *	@returns Success.
 */
bool CConsoleLog::Reset( void )
{
	return false;
}
