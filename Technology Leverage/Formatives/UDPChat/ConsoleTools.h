/**
 *	@file ConsoleTools.h
 *
 *	Copyright (c) Media Design School. 2009 - 2012
 *
 *	@author Jon Booth
 *
 *	@brief	Some simple tools to get input from the user on a windows console.
 *
 */

#ifndef _CONSOLE_TOOLS_H__Included_8769461133411857
#define _CONSOLE_TOOLS_H__Included_8769461133411857

#include <stdio.h>
#include <ctype.h>
#include <conio.h>

inline char* GetLineFromConsole( char* pBuffer, int iNumChars )
{
	fgets( pBuffer, iNumChars, stdin );
	char* pBufferEnd = pBuffer+iNumChars;
	// remove any trailing \n:
	for ( char* pBuf = pBuffer; pBuf < pBufferEnd && *pBuf!=0; pBuf++ )
	{
		if ( *pBuf == '\n' )
		{
			*pBuf = 0;
			return pBuffer;
		}
	}
	// OK, so we need to strip the rest out up to the newline now as we were cut off:
	char Clip[2] = { 1,0 };
	while ( Clip[0] != '\n' )
	{
		fgets( Clip, 2, stdin );		
	}
	return pBuffer;
}

template <size_t iNumChars> char* GetLineFromConsole( char (&pBuffer)[iNumChars] )
{
	return GetLineFromConsole( pBuffer, (int)iNumChars );
}

/**
 *	Ask the user a question, accepting a number of answers, if nothing acceptable is entered then
 *	ask them again.
 *
 *	@author Jon Booth
 *	@param Question			The question to be asked.
 *	@param Accepted			String of characters to accept.
 *	@param bCaseSensitive	Are the accepted answers case sensitive?
 *	@return					The character of the accepted command entered by the user.
 */
inline char QueryOption( const char* Question, const char* Accepted, bool bCaseSensitive = false )
{
	for (;;) // ever
	{
		printf( Question );
		char Str[2];
		GetLineFromConsole( Str );
		char res = Str[0];
		const char* pTestAccepted = Accepted;
		if ( false == bCaseSensitive )
		{
			res = (char)tolower( res );	
		}
		while ( *pTestAccepted != 0 )
		{
			char cTest = *pTestAccepted;
			if ( false == bCaseSensitive )
			{
				cTest = (char)tolower( cTest );
			}
			if ( res == cTest )
			{
				return *pTestAccepted;
			}
			pTestAccepted++;
		}

		printf( "\nSorry %c is not an acceptable answer, please try again.\n", res );
	}
}

/**
 *	Go through a string and resolve any backspaces, deleting the
 *	requisite characters.
 *	Will also clean out any tabs as we don't much like them when
 *	formatting output.
 *
 *	@author Jon Booth
 *
 *	@param	pBuffer	String to have backspaces resolved.
 *	@return A pointer to the start of pBuffer.
 */
inline char* CollapseBackspacesAndCleanInput( char* pBuffer )
{
	// remove all backspaces and tabs:
	char* pDest = pBuffer;
	for ( char* pTest = pBuffer; *pTest!=0; pTest++ )
	{
		if ( *pTest == '\t' )
		{
			*pTest = ' ';
		}
		if ( *pTest == '\b' )
		{
			if ( pDest > pBuffer )
			{
				pDest--;
			}
		} else
		{
			if ( pDest != pTest )
			{
				*pDest = *pTest;
			}
			pDest++;
		}
	}
	*pDest = 0;

}

/**
 *	Input text into a buffer without stalling in a Win32 console window.
 *	sadly not cross platform but only the cursor pos nonsense would need porting.
 *
 *	@author Jon Booth
 */
class CInputLineBuffer
{
public:
	/**
	 *	Constructor.
	 *
	 *	Creates the buffer and sets the string to be empty.
	 *
	 *	@author Jon Booth
	 *	@param uBufferSize	The maximum size of the input buffer.
	 *						No more characters than this will be parsed.
	 */
	CInputLineBuffer( unsigned int uBufferSize )
		: m_uBufferSize( uBufferSize )
		, m_uBufferPos( 0 )
		, m_pBuffer( new char[uBufferSize] )
	{	
		m_pBuffer[0] = 0;
	}
	
	/**
	 *	Deconstructor.
	 *	Cleans up and frees all memory.
	 *
	 *	@author Jon Booth
	 */
	~CInputLineBuffer( void )
	{
		delete[] m_pBuffer;
		m_pBuffer = NULL;
		m_uBufferSize = m_uBufferPos = 0;
	}

	/**
	 *	Clear the current buffered string and set it to empty.
	 *
	 *	@author Jon Booth
	 */
	void ClearString( void )
	{
		m_pBuffer[0] = 0;
		m_uBufferPos = 0;
	}

	/**
	 *	Update the contents of this line buffer.
	 *	Will update the view at the top of the window too.
	 *	@see PrintToScreenTop
	 *
	 *	@author Jon Booth
	 *
	 *	@return true if a newline was encountered.
	 */
	bool Update( void )
	{
		while ( _kbhit() )
		{
			char cNext = (char)_getch();
			if ( cNext == '\t' )
			{
				continue; // we don't like tabs so ignore them.
			}
			if ( cNext == '\b' )
			{
				if ( m_uBufferPos )
				{
					m_uBufferPos--;
					m_pBuffer[m_uBufferPos] = 0;
					PrintToScreenTop();
				}
				continue; // no good for the rest now we've deleted a character.
			}
			if ( cNext == '\n' || cNext == '\r' )
			{
				return true;
			}
			if ( m_uBufferSize > m_uBufferPos+1 )
			{
				m_pBuffer[m_uBufferPos] = cNext; 
				m_uBufferPos++;
				m_pBuffer[m_uBufferPos] = 0;
				PrintToScreenTop();
			}
		}
		return false; // no newline found.
	}

	/**
	 *	Prints the current contenst of the buffer to the top of the
	 *	screen (terminal window).
	 *
	 *	@author Jon Booth
	 *
	 */
	void PrintToScreenTop( void )
	{
		CONSOLE_SCREEN_BUFFER_INFO BufInfo;
		GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &BufInfo );

		COORD coord = { 0, 0 };
		SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), coord );
		printf( m_pBuffer );
		const int iExtraToClearLine = BufInfo.dwSize.X-(m_uBufferPos%BufInfo.dwSize.X);
		for ( int i=0; i<iExtraToClearLine; i++ )
		{
			printf( " " );
		}
		for ( int i=0; i<BufInfo.dwSize.X; i++ )
		{
			printf( "-" );
		}
		SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), BufInfo.dwCursorPosition );
	}

	/**
	 *	Get the current contents of the buffer as entered by the user.
	 *
	 *	@author Jon Booth
	 *
	 *	@return The current string contents.
	 */
	const char* GetString( void ) const { return m_pBuffer; }

protected:
	unsigned int	m_uBufferSize;	//!< The total size of the buffer.
	unsigned int	m_uBufferPos;	//!< The position of the next char in the buffer to be entered by the user.
	char*			m_pBuffer;		//!< The buffer contents.
};

/**
 *	@author Jon Booth
 *
 *	@param uDefault, default port number to use if none entered.  If zero do not allow an empty port.
 *	@return a port number from the user.
 */
unsigned short QueryPortNumber( unsigned short uDefault = 0 )
{
	int iPort = 0;
	for (;;)
	{
		char Port[128];
		printf( "Enter port number: " );
		GetLineFromConsole( Port );
		iPort = atoi( Port );
		if ( iPort != 0 )
		{
			return (unsigned short)iPort;
		}
		if ( uDefault != 0 )
		{
			printf( "Using default %d\n", uDefault );
			return uDefault;
		}
		printf( "Didn't understand that port number\n" );
	}
}

#endif // ifndef _CONSOLE_TOOLS_H__Included_8769461133411857

