/**
 *	@file GDE\Core\DLLWrapper.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/

#include "Core.h"
#ifdef _SUPPORT_DLLS

#include "DLLWrapper.h"
#include "DLLManager.h"
#include "Logger.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


using namespace GDE;

/**
 *	@param DLLName	The filename of the DLL to load.
 *	If this fails to load the DLL then m_hModule is set to NULL.
 *	Created with a single reference.
 */
CDLLWrapper::CDLLWrapper( const char* DLLName )
: m_uRefCount( 1 )
, m_hModule( NULL )
, m_cbGetInterface( NULL )
, m_cbShutdownInterface( NULL )
{
	strcpy_s( m_strFileName, DLLName );
//	strncpy( m_strFileName, DLLName, uMAX_DLL_NAME-1 );
	m_hModule = LoadLibraryA( DLLName );
	if ( NULL == m_hModule )
	{
		DbgPrint( "ERROR:(%s) unable to load the dll\n", DLLName );
		assert_now( "ERROR:(%s) unable to load the dll\n", DLLName );
		return;
	}

	m_cbGetInterface = (DLL_GetInterface)GetProcAddress( reinterpret_cast<HMODULE>(m_hModule), "DLL_GetWrapperBase" );
	if ( NULL == m_cbGetInterface )
	{
		DbgPrint( "ERROR:(%s) unable to bind the dll get wrapper base function\n", DLLName );		
		FreeLibrary( reinterpret_cast<HMODULE>(m_hModule) );
		m_hModule = NULL;
		return;
	}
	m_cbShutdownInterface = (DLL_ShutdownInterface)GetProcAddress( reinterpret_cast<HMODULE>(m_hModule), "DLL_ShutdownWrapperBase" );
	if ( NULL == m_cbShutdownInterface )
	{
		DbgPrint( "ERROR:(%s) unable to bind the dll get wrapper base shutdown function\n", DLLName );		
		FreeLibrary( reinterpret_cast<HMODULE>(m_hModule) );
		m_hModule = NULL;
		return;
	}

}

/** 
 *	Destructor, remove and free any interface we have.
 */
CDLLWrapper::~CDLLWrapper( void )
{
	if ( NULL != m_hModule )
	{
		FreeLibrary( reinterpret_cast<HMODULE>(m_hModule) );
	}
	m_hModule = NULL;
	m_cbGetInterface = NULL;
	m_cbShutdownInterface = NULL;
}

/**
 *	Add a reference to this resource.
 */
void CDLLWrapper::AddRef( void )
{
	++m_uRefCount;
}

/**
 *	Release this resource.
 *
 *	@return true if this was the last reference count freed and so the resource is finished with.
 */
bool CDLLWrapper::Release( void )
{
	--m_uRefCount;
	if ( m_uRefCount == 0 )
	{
		CDLLManager::GetInstance().ReleaseDLL( this );	// note deletes this!
		return true;
	}
	return false;
}

/**
 *	@return the current reference count for the resource.
 */
GDE::UINT32 CDLLWrapper::GetRefCount( void ) const
{
	return m_uRefCount;
}

// Accessing a DLL interface

/**
 *	@return The filename for the given DLL.
 */
const char*	CDLLWrapper::GetFileName( void ) const\
{
	return m_strFileName;
}


/**
 *	@return Does the DLL exist?
 */
bool CDLLWrapper::DLLExists( void ) const
{
	return NULL != m_hModule;
}

/**
 *	Load a DLL interface from this DLL.
 *
 *	@param iDLL_ID		The id for the dll interface being sought.
 *	@param pInterface	On success filled in with a ppinter to the interface, else NULL.
 *	@return				Sucess.
 */
bool CDLLWrapper::GetDLLInterface( int iDLL_ID, void*& pInterface )
{
	pInterface = m_cbGetInterface( iDLL_ID );
	if ( NULL == pInterface )
	{
		char strDLLId[5];
		DLLIdToString( iDLL_ID, strDLLId );
		DbgPrint( "ERROR:(%s) unable to create the dll object %s\n", m_strFileName, strDLLId );
		return false;
	}
	return true;
}

/**
 *	Close a given DLL interface.
 *	
 *	@param iDLL_ID		The id for the interface to be shutdown.
 *	@param pInterface	The interface to be shutdown.
 */
bool CDLLWrapper::FreeDLLInterface( int iDLL_ID, void* pInterface )
{
	if ( pInterface )
	{
		return m_cbShutdownInterface( iDLL_ID, pInterface );
	}
	return false;
}


#endif // #ifdef _SUPPORT_DLLS


