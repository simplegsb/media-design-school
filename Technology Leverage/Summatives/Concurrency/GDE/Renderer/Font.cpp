/**
 *	@file GDE\Renderer\Font.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "Font.h"
#include "FontImp.h"
#include "FontManager.h"

using namespace GDE;

/**
 *	Constructor - only used by the manager, has to be overloaded though explicitly
 */
CFont::CFont( IFontImp* pImplementer, const CResourceId& Id, CFontManager* pManager, CFont* pNext )
: TParent( pImplementer, Id,  pManager, pNext )
{	// empty.
}

/**
 *	Draw the message at the specified place.
 *	Unknown characters are drawn as '?'.
 *	Line breaks ('\n') should be handled properly.
 *
 *	@param fX			Screen X coordinate.
 *	@param fY			Screen Y coordinate.
 *	@param eJustify		The type of justification to use.
 *	@param strMessage	The message to be drawn on the screen.
 */
bool CFont::Render( FLOAT32 fX, FLOAT32 fY, EFontJustification eJustify, const char* strMessage )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->Render( fX, fY, eJustify, strMessage );
}

/**
 *	@param colFont	The colour to set the font to for subsequent draw operations.
 */
bool CFont::SetColour( UINT32 colFont )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->SetColour( colFont );	
}

/**
 *	@return The current colour of the font.
 */
UINT32 CFont::GetColour( void ) const
{
	if ( m_pImplementer )
	{
		return m_pImplementer->GetColour();
	}
	assert_now( "No implementer present" );
	return 0;
}

/**
 *	@return The height of the font, in pixels.
 */
FLOAT32 CFont::GetHeight( void ) const
{
	if ( m_pImplementer )
	{
		return m_pImplementer->GetHeight();
	}
	assert_now( "No implementer present" );
	return 0;
}

/**
 *	@return The width of the string, in pixels.
 */
FLOAT32 CFont::GetStringWidth( const char* strMessage ) const
{
	if ( m_pImplementer )
	{
		return m_pImplementer->GetStringWidth( strMessage );
	}
	assert_now( "No implementer present" );
	return 0;
}
