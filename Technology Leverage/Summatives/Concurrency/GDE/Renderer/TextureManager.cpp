/**
 *	@file GDE\Renderer\TextureManager.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "TextureManager.h"
#include "TextureManagerImp.h"
#include "Texture.h"

using namespace GDE;

/**
 *	Create the texture manager, and initialise the NULL texture.
 */
CTextureManager::CTextureManager( ITextureManagerImp* pImp )
: TParent( pImp )
, m_pNULLTexture( NULL )
{
	if ( pImp )
	{
		ITextureImp* pNakedResource = NULL;
		// nasty cast ensures that we can use this inline as all Implementers must be of the TIResourceManagerImp type.
		if ( m_pImplementer->GetNULLTexture( pNakedResource ) )
		{
			CResourceId ResId( "?NULL_Texture" );
			++m_uResourceCount;
			m_pResourceList = new CTexture( pNakedResource, ResId, this, m_pResourceList );
			m_pNULLTexture = m_pResourceList;
		}
	}
}

CTextureManager::~CTextureManager( void )
{
	if ( m_pNULLTexture )
	{
		m_pNULLTexture->Release();
		m_pNULLTexture = NULL;
	}
}

CTexture* CTextureManager::GetNULLTexture( void )
{
	return m_pNULLTexture;
}

