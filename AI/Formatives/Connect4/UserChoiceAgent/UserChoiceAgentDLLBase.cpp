/**
 *	@file Connect4\UserChoiceAgent\UserChoiceAgentDLLBase.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2012 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "GDE/Core/DLLInterface.h"
#include "UserChoiceAgent.h"

const GDE::CLibraryDescription g_LibDesc = CREATE_LIBRARY_DESCRIPTION(
		'C','N','C','4',
		"Connect4 agent that allows the user to pick moves",
		1,0,0);		// version

const GDE::CLibraryDescription* g_pLibDesc = &g_LibDesc; // to be externed

/**
 *	This function allows the dll to return an instance of the requested interface..
 */
__declspec(dllexport) void* __cdecl DLL_GetWrapperBase( int iDLL_ID )
{
	switch( iDLL_ID )
	{
	case CREATE_DLL_ID( 'C','N','C','4' ):
		{
			CUserChoiceAgent* pAgent = new CUserChoiceAgent;
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
			CUserChoiceAgent* pAgent = reinterpret_cast<CUserChoiceAgent*>(pBase);
			delete pAgent;
			return true;
		}
		break;
	default:
		return false;
		break;
	}
}

