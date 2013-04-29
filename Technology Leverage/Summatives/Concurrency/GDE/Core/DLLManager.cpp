/**
 *	@file GDE\Core\DLLManager.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/

#include "Core.h"
#ifdef _SUPPORT_DLLS

#include "DLLManager.h"
#include "DLLWrapper.h"


#include <map>
#include <string>
#include <cctype>		// for tolower
#include <algorithm>	// for transform

using namespace GDE;

typedef std::map<std::string,CDLLWrapper*> DLLContainer;
static DLLContainer gs_DLLContainer;	//!< Container for all the DLL's

/**
 *	Implement the singleton access.
 */
CDLLManager& CDLLManager::GetInstance( void )
{
	static CDLLManager sManager;
	return sManager;
}

/**
 *	@param strFileName	The filename for the DLL.
 *	@return A DLL wrapper for the given DLL, or NULL if it cannot be initialised. The refcount for the DLL will have already been incremented on return.
 */
CDLLWrapper* CDLLManager::GetDLL( const char* strFileName )
{
	std::string strFN = strFileName;
	std::transform(strFN.begin(),
				   strFN.end(),
				   strFN.begin(),
				   std::tolower );
	//char strFN[MAX_PATH];
	//const char* pSrc = strFileName;
	//char* pDst = strFN;
	//do
	//{
	//	*pDst++ = *pSrc++;
	//} while( *pSrc != 0 )
	// now we have a lowercase string lets see it's in the map.
	CDLLWrapper* pWrapper = gs_DLLContainer[strFN];
	if ( pWrapper )
	{
		pWrapper->AddRef();
	} else
	{
		pWrapper = new CDLLWrapper( strFileName );
		if ( false == pWrapper->DLLExists() )
		{
			pWrapper->Release(); // will delete it...
			pWrapper = NULL;
		} else
		{
			gs_DLLContainer[strFN] = pWrapper;
		}
	}
	return pWrapper;
}

/**
 *	Protected function only to be called on final Release of the DLL, removes it.
 */
bool CDLLManager::ReleaseDLL( CDLLWrapper* pDLL )
{
	std::string strFN = pDLL->GetFileName();
	std::transform(strFN.begin(), 
				   strFN.end(),
				   strFN.begin(),
				   std::tolower );

	gs_DLLContainer.erase(strFN);
	delete pDLL;
	return true;
}

#endif // #ifdef _SUPPORT_DLLS


