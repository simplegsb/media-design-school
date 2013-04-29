/**
 *	@file GDE\Core\ResourceId.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Core.h"
#include "ResourceId.h"

#include <string.h>

using namespace GDE;

CResourceId::CResourceId( void )
: m_strFileName( NULL )
{
}

CResourceId::CResourceId( const char* strFileName )
	: m_strFileName( NULL )
{
	Init( strFileName );
}

CResourceId::CResourceId( const CResourceId& Other )
	: m_strFileName( NULL )
{
	Init( Other.m_strFileName );
}


CResourceId::~CResourceId( void )
{
	if ( m_strFileName )
	{
		delete[] m_strFileName;
		m_strFileName = NULL;
	}
}

void CResourceId::Init(  const char* strFileName )
{
	if ( m_strFileName )
	{
		delete[] m_strFileName;
	}

	m_strFileName = NULL;

	if ( strFileName )
	{
		const size_t iStrLen = strlen(strFileName);
		char* pFileName = new char[iStrLen+1];
		m_strFileName = pFileName;
		while( (*pFileName++=*strFileName++)!= 0 )
		{
			// loop
		}
	}
}

/**
 *	Check to see if the resource Id's are equivalent.	
 */
bool CResourceId::operator == (const CResourceId& Other ) const
{
	return _stricmp( m_strFileName, Other.m_strFileName ) == 0;
}

/**
 *	@return the filename for the given resouce.
 */
const char* CResourceId::GetFileName( void ) const
{
	return m_strFileName; 
}
