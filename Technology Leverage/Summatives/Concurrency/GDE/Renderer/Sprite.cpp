/**
 *	@file GDE\Renderer\Sprite.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "Sprite.h"
#include "SpriteImp.h"

namespace GDE
{

/**
 *	Render the sprite with the hotspot at the given coordinates.
 *	@see SetHotSpot.
 *	@param fX	The screen X coordinate to render at.
 *	@param fY	The screen Y coordinate to render at.
 *
 *	@return Success.
 */
bool CSprite::Render( FLOAT32 fX, FLOAT32 fY )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->Render( fX, fY );
}

/**
 *	Render the sprite with the hotspot at the given coordinates.
 *	@see SetHotSpot.
 *	@param fX		The screen X coordinate to render at.
 *	@param fY		The screen Y coordinate to render at.
 *	@param fRot		Clockwise rotation in radians around the hotspot.
 *	@param fXScale	Horizontal scaling to be applied.
 *	@param fYScale	Vertical scaling to be applied.
 *
 *	@return Success.
 */
bool CSprite::RenderEx( FLOAT32 fX, FLOAT32 fY, FLOAT32 fRot, FLOAT32 fXScale, FLOAT32 fYScale )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->RenderEx( fX, fY, fRot, fXScale, fYScale );
}

/**
 *	@param colSprite	The colour to set the sprite tint too.
 *	@return Success.
 */
bool CSprite::SetColour( UINT32 colSprite )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->SetColour( colSprite );
}

/**
 *	@return The current colour of the sprite tint or zero if not implemented.
 */
UINT32 CSprite::GetColour( void ) const
{
	if ( m_pImplementer )
	{
		return m_pImplementer->GetColour();
	}
	return 0;
}

/**
 *	@param pTexture The texture to set for the sprite page.
 *	@return Success.
 */
bool CSprite::SetTexture( CTexture* pTexture )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->SetTexture( pTexture );
}

/**
 *	@return The current sprite texture or NULL if not implemented.
 */
CTexture* CSprite::GetTexture( void ) const
{
	if ( m_pImplementer )
	{
		return m_pImplementer->GetTexture();
	}
	return NULL;
}

/**
 *	Set the texture rectangle for the sprite to use from the current texture page.
 *
 *	@param Rect	The rectangle to use.
 *	@return Success.
 */
bool CSprite::SetTextureRect( const SFRect& Rect )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->SetTextureRect( Rect );
}

/**
 *	Get the texture rectangle the sprite uses from the current texture page.
 *
 *	@param Rect	Filled with the rectangle in use.
 *	@return Success.
 */
bool CSprite::GetTextureRect( SFRect& Rect ) const
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->GetTextureRect( Rect );
}

/**
 *	Set the hot spot offset for the sprite.
 *	@param fX The horizontal offset into the texture rect.
 *	@param fY The vertical offset into the texture rect.
 *	@return Success.
 */
bool CSprite::SetHotSpot( FLOAT32 fX, FLOAT32 fY )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->SetHotSpot( fX, fY );
}
/**
 *	Get the hot spot offset in use for the sprite.
 *	@param fX Filled with the horizontal offset into the texture rect.
 *	@param fY Filled with the vertical offset into the texture rect.
 *	@return Success.
 */
bool CSprite::GetHotSpot( FLOAT32& fX, FLOAT32& fY ) const
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->GetHotSpot( fX, fY );
}

/**
 *	@return The width of the sprite in pixels or zero if not implemented.
 */
FLOAT32 CSprite::GetWidth( void ) const
{
	if ( m_pImplementer )
	{
		return m_pImplementer->GetWidth();
	}
	return 0.0f;
}

/**
 *	@return The height of the sprite in pixels or zero if not implemented.
 */
FLOAT32 CSprite::GetHeight( void ) const
{
	if ( m_pImplementer )
	{
		return m_pImplementer->GetHeight();
	}
	return 0.0f;
}

} // end namespace GDE
