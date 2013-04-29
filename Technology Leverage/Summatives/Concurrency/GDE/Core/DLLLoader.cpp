/**
 *	@file GDE\Core\DLLLoader.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Core.h"
#include "Logger.h"

#include "DLLLoader.h"
#include "DLLManager.h"
#include "DLLWrapper.h"
#include "StaticLibraries.h"

using namespace GDE;

CDLLLoader::CDLLLoader( void )
: m_pStaticLibDeleter( NULL )
#ifdef _SUPPORT_DLLS
, m_pDLL( NULL )
#endif
, m_pDLLInstance( NULL )
{	// empty
}

CDLLLoader::~CDLLLoader( void )
{
	assert_msg( NULL == m_pDLLInstance, "The DLL for this was not shut down" );
#ifdef _SUPPORT_DLLS
	assert_msg( NULL == m_pDLL, "The DLL for this was not shut down" );
#endif
}

/**
 *	Shutdown this DLL interface.
 *	@param iDLL_ID	The id that the interface was created with.
 *	@return success.
 */
bool CDLLLoader::ShutdownDLL( int iDLL_ID )
{
	if ( m_pStaticLibDeleter )
	{
		assert( m_pDLLInstance );
		m_pStaticLibDeleter( m_pDLLInstance );
		m_pDLLInstance = NULL;
		m_pStaticLibDeleter = NULL;
		return true;
	}

#ifdef _SUPPORT_DLLS
	if ( m_pDLL )
	{
		assert( m_pDLLInstance );
		m_pDLL->FreeDLLInterface( iDLL_ID, m_pDLLInstance );

		m_pDLLInstance = NULL;
		// TODO: use smart pointers to store the DLLs?
		m_pDLL->Release();
		m_pDLL = NULL;
		return true;
	} else
#endif
	{
		assert( NULL == m_pDLLInstance );
		return false;
	}
}

/**
 *	@param DLLName	The filename of the DLL to load.
 *	@param iDLL_ID	The id of the interface to get.
 *	@return			true on success.
 */
bool CDLLLoader::LoadDLLInterface( const char* DLLName, int iDLL_ID )
{
	assert( m_pDLLInstance == NULL );

	// check if there is a static library version registered:
	m_pDLLInstance = CStaticLibraryManager::GetInstance().GetInterface( DLLName, iDLL_ID, m_pStaticLibDeleter );
	if ( m_pDLLInstance )
	{
		// nice, created statically.
		return true;
	}

#ifdef _SUPPORT_DLLS
	m_pDLL = CDLLManager::GetInstance().GetDLL( DLLName );
	assert_msg( m_pDLL, "Unable to load DLL from file %s", DLLName );
	if ( NULL == m_pDLL )
	{
		return false;
	}
	if ( false == m_pDLL->GetDLLInterface( iDLL_ID, m_pDLLInstance ) )
	{
		char strDLLId[5];
		DLLIdToString( iDLL_ID, strDLLId );
		assert_now( "Unable to get DLL interface %s from file %s", strDLLId, DLLName );
		m_pDLL->Release();
		return false;
	} 
#endif
	return true;
}

