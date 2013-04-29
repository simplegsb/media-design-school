/**
 *	@file GDE\Renderer\SpriteManager.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "SpriteManager.h"
#include "SpriteManagerImp.h"
#include "Sprite.h"

using namespace GDE;

bool CSpriteManager::LoadDLL( const char* strDLLName, CRenderer* pRenderer )
{
	if ( LoadDLL( strDLLName ) )
	{
		if ( m_pImplementer->SetRenderer( pRenderer ) )
		{
			m_pRenderer = pRenderer;
			return true;
		} else 
		{
			assert_now( "Unable to bind sprite DLL \"%s\" to the renderer", strDLLName );
			ReleaseDLLImplementer();
		}
	}
	return false;
}

/**
 *	Create a sprite.
 *
 *	@param ResId			The resourceID for the sprite.
 *	@param pTexture			The texture to use for the sprite.
 *	@param sTextureRect		The rectangle of the texture the sprite uses - in texels.
 *	@return					The sprite (on success). Refcount will be one!
 */
CSprite* CSpriteManager::CreateSprite( const CResourceId& ResId, CTexture* pTexture, const SFRect& sTextureRect )
{
	CSprite* pResource = FindResouceById( ResId );
	if ( pResource )
	{
		return pResource;
	}
	if ( NULL != m_pImplementer )
	{
		ISpriteImp * pNakedResource = m_pImplementer->CreateSprite( pTexture, sTextureRect );
		if ( pNakedResource )
		{
			++m_uResourceCount;
			m_pResourceList = new CSprite( pNakedResource, ResId, /*reinterpret_cast<TResourceBridge::TManagerBase*>*/(this), m_pResourceList );
			return m_pResourceList;
		}
	}
	return NULL;	// no Implementer present.
}
