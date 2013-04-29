/**
 *	@file GDE\Renderer\SpriteAnimManager.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "SpriteAnimManager.h"
#include "SpriteAnimManagerImp.h"
#include "SpriteAnim.h"

using namespace GDE;

bool CSpriteAnimManager::LoadDLL( const char* strDLLName, CSpriteManager* pSpriteManager )
{
	if ( LoadDLL( strDLLName ) )
	{
		if ( m_pImplementer->SetSpriteManager( pSpriteManager ) )
		{
			return true;
		} else 
		{
			assert_now( "Unable to bind sprite animation DLL \"%s\" to the sprite manager passed in", strDLLName );
			ReleaseDLLImplementer();
		}
	}
	return false;
}

/**
 *	Create a sprite animation.
 *	After the first frame, subsequent frames must come to the left in the texure.
 *	When there is no more space available to the left of the previous frame
 *	the next frame starts directly below and to the very left of the texture page.
 *
 *	@param ResId			The resourceID for the sprite.
 *	@param uFrameCount		The number of frames in the animation.
 *	@param fFrameRate		The number of frames per second to play the animation at.
 *	@param pTexture			The texture to use for the sprite.
 *	@param sTextureRect		The rectangle of the texture the sprite uses - in texels, for the first frame.
 *	@return					The sprite animation (on success). Refcount will be one!
 */
CSpriteAnim* CSpriteAnimManager::CreateAnim( 
			const CResourceId&	ResId, 
			UINT32				uFrameCount,
			FLOAT32				fFrameRate,
			CTexture*			pTexture,
			const SFRect&		sTextureRect )
{
	CSpriteAnim* pResource = FindResouceById( ResId );
	if ( pResource )
	{
		return pResource;
	}
	if ( NULL != m_pImplementer )
	{
		ISpriteAnimImp * pNakedResource = m_pImplementer->CreateAnim( 
			uFrameCount,
			fFrameRate,
			pTexture, 
			sTextureRect );
		if ( pNakedResource )
		{
			++m_uResourceCount;
			m_pResourceList = new CSpriteAnim( pNakedResource, ResId, /*reinterpret_cast<TResourceBridge::TManagerBase*>*/(this), m_pResourceList );
			return m_pResourceList;
		}
	}
	return NULL;	// no Implementer present.
}
