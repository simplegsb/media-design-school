/**
 *	@file GDE\Renderer\FontManager.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "FontManager.h"
#include "FontManagerImp.h"

using namespace GDE;

bool CFontManager::LoadDLL( const char* strDLLName, CRenderer* pRenderer )
{
	if ( LoadDLL( strDLLName ) )
	{
		if ( m_pImplementer->SetRenderer( pRenderer ) )
		{
			return true;
		} else 
		{
			assert_now( "Unable to bind font DLL \"%s\" to the renderer", strDLLName );
			ReleaseDLLImplementer();
		}
	}
	return false;
}
