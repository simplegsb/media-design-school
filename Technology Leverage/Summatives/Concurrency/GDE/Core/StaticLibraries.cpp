/**
 *	@file GDE\GDE\Core\StaticLibraries.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "StaticLibraries.h"
#include "DLLInterface.h"

#include <string.h>

using namespace GDE;

// the registrar, used to create.
CStaticLibraryRegistrar::CStaticLibraryRegistrar( 
	CStaticLibraryManager::cbCreateLib* pCreateFN, 
	CStaticLibraryManager::cbKillLib* pKillFN,
	const char* strName, char cID_0, char cID_1, char cID_2, char cID_3 )
{
	CStaticLibraryManager::GetInstance().RegisterLibrary(
		pCreateFN,
		pKillFN,
		strName,
		CREATE_DLL_ID( cID_0, cID_1, cID_2, cID_3 ) );
}

/**
 *	Singleton instance access.
 */
CStaticLibraryManager& CStaticLibraryManager::GetInstance( void )
{
	static CStaticLibraryManager Man;
	return Man;
}

CStaticLibraryManager::CStaticLibraryManager( void )
: m_pLibraries( NULL )
{	// empty
}

CStaticLibraryManager::~CStaticLibraryManager( void )
{
	while ( NULL != m_pLibraries )
	{
		SLibEntry* pNext = m_pLibraries->m_pNext;
		delete m_pLibraries;
		m_pLibraries = pNext;
	}
}


bool	CStaticLibraryManager::RegisterLibrary( cbCreateLib* pCreateFN, 
												cbKillLib* pKillFN,
												const char* strName, int iDLL_ID )
{
	if ( NULL != GetLibrary( strName, iDLL_ID ) )
	{
		return false; // can't register two libraries for the same thing.
	}
	SLibEntry* pNewEntry = new SLibEntry();
	pNewEntry->m_pCreateFN = pCreateFN;
	pNewEntry->m_pKillFN = pKillFN;
	pNewEntry->m_strName = strName;
	pNewEntry->m_iDLL_ID = iDLL_ID;
	pNewEntry->m_pNext = m_pLibraries;
	m_pLibraries = pNewEntry;

	return true;
}

/**
 *	@return the entry for the library mathching the parameters or NULL.
 */
CStaticLibraryManager::SLibEntry*		CStaticLibraryManager::GetLibrary( const char* strName, int iDLL_ID )
{
	for ( SLibEntry* pEntry = m_pLibraries; NULL != pEntry; pEntry = pEntry->m_pNext )
	{
		if ( iDLL_ID == pEntry->m_iDLL_ID &&
			_stricmp( pEntry->m_strName, strName ) == 0 )
		{
			return pEntry;
		}
	}
	return NULL;
}

/**
 *	@return a pointer to the library created.
 */
void*	CStaticLibraryManager::GetInterface( const char* strName,  int iDLL_ID, CStaticLibraryManager::cbKillLib*& pKillLib )
{
	SLibEntry* pEntry = GetLibrary( strName, iDLL_ID );
	if ( pEntry )
	{
		void* pRes = pEntry->m_pCreateFN();
		if ( pRes )
		{
			pKillLib = pEntry->m_pKillFN;
		} else
		{
			assert_now( "Unable to create a static registered library" );
		}
		return pRes;
	} 
	return NULL;
}
