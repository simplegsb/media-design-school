/**
 *	@file GDE\Core\Assert.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Core.h"

#include "AssertQuery.h"
#include "WindowsAssert.h" // for the default handler.

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

using namespace GDE;
#define __SHOW_FUNCTION_NAME

#ifdef _PS2_
namespace GDE
{
	void DebugBreakFunc( void )
	{
		// This is in a file, as it helps the debugger find out where it should be.
		// to see the assert message look in the output console (for eclipse).
		// to resume, right click on the nop line and select "Resume at Line"
		asm __volatile__("break 1");
		asm __volatile__("nop"); // resume at this line.
	}
}
#endif


//#ifdef _64BIT
//_CompileAssert( sizeof( UINTPTR ) == 64/8 );
//#else
//_CompileAssert( sizeof( UINTPTR ) == 32/8 );
//#endif

#ifdef _DEBUG
#include "Logger.h"

CAssertHandler::CAssertHandler( IAssertQuery* pQuery )
: TSuperClass( pQuery )
{ // empty
}

/**
 *	Meyers Singleton.
 */
CAssertHandler& CAssertHandler::GetInstance( void )
{
	static CAssertHandler sHandler( &CWindowsAssertQuery::GetInstance() );
	return sHandler;
}

// returns if we want to break.
bool CAssertHandler::Assert( 
		const char* strMsg, UINT32 uLine, const char* strFileName, 
		const char* strFunction, bool& bSkip )
{
#ifdef __SHOW_FUNCTION_NAME
	CLogger::GetDebugPrintLog().PrintF( "%s(%d):<%s> ASSERT FAILED: %s\n", strFileName, uLine, strFunction, strMsg );
#else
	CLogger::GetDebugPrintLog().PrintF( "%s(%d):ASSERT FAILED: %s\n", strFileName, uLine, strMsg );
#endif

	if ( bSkip )
	{
		return false;	// never break - no need to ask.
	}

#ifdef _PS2_
	// TODO: a PS2 implementer to handle this?
	if ( bSkip )
	{
		return false;	// already decided to skip.
	}
	// This is in a file, as it helps the debugger find out where it should be.
	// to see the assert message look in the output console (for eclipse).
	// to resume, right click on the nop line and select "Resume at Line"
	// if you never want to assert on this particular assert again, change the
	// bSkip variable to true (this can be done in the variables window in eclipse).
	asm __volatile__("break 1");
	asm __volatile__("nop"); // resume at this line.
	return false;	// no need to break - we already did!
#else
	// TODO: Improve this
	m_pImplementer->AskAssert( strFileName, uLine, strFunction, strMsg );
	bSkip = m_pImplementer->ShouldAlwaysIgnore();
	return m_pImplementer->ShouldBreak();
#endif
}

// returns if we want to break.
bool CAssertHandler::MessageAssert(
	const char* strExpression,
	UINT32 uLine, const char* strFileName, const char* strFunction,
	bool& bSkip,
	const char* strMessageFmt,
	...
)
{
	// TODO: make this thread safe.
	const static UINT32 uMESSAGE_SIZE = 1024;
	static char strMessage[uMESSAGE_SIZE];

	int iChars = _snprintf_s( strMessage, uMESSAGE_SIZE, uMESSAGE_SIZE,
			"\nexpression:	%s\n"
			"message:		", strExpression );

	va_list ap;
	va_start(ap, strMessageFmt);
	iChars += vsnprintf_s(strMessage+iChars, uMESSAGE_SIZE-iChars, uMESSAGE_SIZE-iChars, strMessageFmt, ap);
	va_end(ap);

	return Assert( strMessage, uLine, strFileName, strFunction, bSkip );
}


#endif // _DEBUG
