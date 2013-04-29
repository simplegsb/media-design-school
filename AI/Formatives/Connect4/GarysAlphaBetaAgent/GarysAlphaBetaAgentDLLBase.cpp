#include "GDE/Core/Core.h"
#include "GDE/Core/DLLInterface.h"

#include "GarysAlphaBetaAgent.h"

const GDE::CLibraryDescription g_LibDesc = CREATE_LIBRARY_DESCRIPTION(
		'C','N','C','4',
		"An Agent that employs a min-max algorithm with alpha-beta pruning.",
		1,0,0);		// version

const GDE::CLibraryDescription* g_pLibDesc = &g_LibDesc; // to be externed

/**
 *	This function allows the dll to return an instance of the requested interface.
 */
__declspec(dllexport) void* __cdecl DLL_GetWrapperBase( int iDLL_ID )
{
	switch( iDLL_ID )
	{
	case CREATE_DLL_ID( 'C','N','C','4' ):
		{
			GarysAlphaBetaAgent* pAgent = new GarysAlphaBetaAgent;
			return pAgent;
			break;
		}
	default:
		return NULL;
	}
}

/**
 *	This function shuts down the dll, deleting the interface.
 */
__declspec(dllexport) bool __cdecl DLL_ShutdownWrapperBase( int iDLL_ID, void* pBase )
{
	switch( iDLL_ID )
	{
	case CREATE_DLL_ID( 'C','N','C','4' ):
		{
			GarysAlphaBetaAgent* pAgent = reinterpret_cast<GarysAlphaBetaAgent*>(pBase);
			delete pAgent;
			return true;
		}
		break;
	default:
		return false;
		break;
	}
}

