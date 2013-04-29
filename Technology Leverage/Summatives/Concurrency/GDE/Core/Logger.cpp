/**
 *	@file GDE\Core\Logger.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/

#include "Core.h"
#include "Logger.h"
#include "Log.h"
#include "DebuggerLog.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

using namespace GDE;

/**
 *	@return The default log for the DbgPrint macro, send output to the debugger.
 */
CLogger& CLogger::GetDebugPrintLog( void )
{
	static CLogger sLog( &CDebuggerLog::GetInstance() );
	return sLog;
}

/**
 *	@return The default log for the DbgSPrint macro, this one does nothing as it is not provided an log instance.
 */
CLogger& CLogger::GetDebugSPrintLog( void )
{
	static CLogger sLog( NULL );
	return sLog;

}


/**
 *	Construct the logger.
 *
 *	@param pLog				The log implementation to send messages too.
 *	@param uBufferSize		The size of the text buffer used internally.
 */
CLogger::CLogger( ILog* pLog, UINT32 uBufferSize )
: TSuperClass()
, m_uBufferSize( uBufferSize )
{
	m_pImplementer = pLog;
	m_pBuffer = new char[uBufferSize];
	m_pBuffer[0] = 0;
}

/**
 *	Shutdown the logger.
 */
CLogger::~CLogger( void )
{
	delete[] m_pBuffer;
}

/**
 *	Print to the log using printf style formatting.
 *	
 *	@param strFormat	The printf style format to print.
 *	@return				The number of characters written or -1 if this is not implemented.
 */
int CLogger::PrintF( const char* strFormat, ... )
{
	if ( NULL == m_pImplementer )
	{
		return -1;
	}

	va_list ap;
	va_start(ap, strFormat);
	int iChars = vsnprintf_s(m_pBuffer, m_uBufferSize, m_uBufferSize, strFormat, ap);
	va_end(ap);

	m_pImplementer->Write( m_pBuffer );
	m_pBuffer[0] = 0;
	return iChars;
}

/**
 *	Print to the log using printf style formatting, 
 *	first adding in the line number and source file that produced the log message.
 *	
 *	@param uLineNum		The source code line number that this came from.
 *	@param strFileName	The source code file name this came from.
 *	@param strFormat	The printf style format to print.
 *	@return				The number of characters written or -1 if this is not implemented.
 */
int	CLogger::PrintF( UINT32 uLineNum, const char* strFileName, const char* strFormat, ... )
{
	int iChars = sprintf_s( m_pBuffer, m_uBufferSize, "%s(%d) : ", strFileName, uLineNum );
	va_list ap;
	va_start(ap, strFormat);
	iChars += vsnprintf_s(m_pBuffer+iChars, m_uBufferSize-iChars, m_uBufferSize-iChars, strFormat, ap);
	va_end(ap);

	m_pImplementer->Write( m_pBuffer );
	m_pBuffer[0] = 0;
	return iChars;
}

/**
 *	@return The current implementer for the log.
 */
ILog*	CLogger::GetImplementer( void )
{
	return m_pImplementer;
}

/**
 *	@param pILog	The new implementer for this log.
 */
void	CLogger::SetImplementer( ILog* pILog )
{
	m_pImplementer = pILog;
}
