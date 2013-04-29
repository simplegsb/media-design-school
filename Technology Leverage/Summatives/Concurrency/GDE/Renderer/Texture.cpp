/**
 *	@file GDE\Renderer\Texture.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "Texture.h"
#include "TextureImp.h"
#include "TextureManager.h"

using namespace GDE;

/**
 *	Apply the texture to be rendered on subsequent primitives.
 *	@param uTextureStage	The texture stage to apply to.
 */
bool CTexture::Apply( UINT32 uTextureStage )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->Apply( uTextureStage );
}

/**
 *	@return The width, in texels, of the texture or zero on error.
 */
UINT32 CTexture::GetWidth( void ) const
{
	if ( NULL == m_pImplementer )
	{
		return 0;
	}
	return m_pImplementer->GetWidth();
}

/**
 *	@return The height, in texels, of the texture or zero on error.
 */
UINT32 CTexture::GetHeight( void ) const
{
	if ( NULL == m_pImplementer )
	{
		return 0;
	}
	return m_pImplementer->GetHeight();
}

/**
 *	Lock the texels as UINT32 colour values in an array. All locked textures must be subsequently
 *	unlocked. @see UnlockTexels.
 *
 *	The first value in the array is the top left texel, subsequent values move to the right.
 *	Each row starts at pTexels[uPitch*uRow];
 *
 *	@param pTexels		Filled in with a pointer to the texels.
 *	@param uPitch		Filled in with the pitch (offset between subsequent rows of texels).
 *	@param bReadOnly	Are the texels read only? Otherwise changes will be updated to the texture.
 *	@param pRegion		Optional sub region to lock.
 *	@return				Success. Some textures (compressed for example) may not support locking.
 */
bool	CTexture::LockTexels( UINT32* &pTexels, UINT32& uPitch, bool bReadOnly, const SIRect* pRegion )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->LockTexels( pTexels, uPitch, bReadOnly, pRegion );
}

/**
 *	Unlock the texture's texels.
 *	If the texture was not locked read only then this will update the texture with the new values filled in.\
 *	@return Success.
 */
bool	CTexture::UnlockTexels( void )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->UnlockTexels();
}
