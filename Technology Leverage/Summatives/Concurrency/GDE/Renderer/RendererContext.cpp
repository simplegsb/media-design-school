/**
 *	@file GDE\Renderer\RendererContext.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "RendererContext.h"

using namespace GDE;

IRendererContext::IRendererContext( void )
: m_uDispWidth( 800 )
, m_uDispHeight( 600 )
, m_uDispDepth( 32 )
, m_bWindowed( true )
, m_strWindowTitle( "Graduate Diploma of Games Development" )
, m_bCreateZBuffer( false )
, m_bInFocus( false )
{	// empty, just setting up defaults, above.
}

/**
 *	@return The width of the display in pixels.
 */
UINT32			IRendererContext::GetDisplayWidth( void ) const
{
	return m_uDispWidth;
}

/**
 *	@return The height of the display in pixels.
 */
UINT32			IRendererContext::GetDisplayHeight( void ) const
{
	return m_uDispHeight;
}

/**
 *	@return The depth of the display in bits
 */
UINT32			IRendererContext::GetDisplayDepth( void ) const
{
	return m_uDispHeight;	
}

/**
 *	@return Is the display windowed?
 */
bool			IRendererContext::IsWindowed( void ) const
{
	return m_bWindowed;
}

/**
 *	@return The system title of this application (used for the window/task bar tab titles under windows).
 */
const char*		IRendererContext::GetWindowTitle( void ) const
{
	return m_strWindowTitle;
}

/**
 *	@return If the context should automatically create a Z buffer
 */
bool			IRendererContext::CreateZBuffer( void ) const
{
	return m_bCreateZBuffer;
}

/**
 *	Notification from the renderer that the app has lost or gained focus.
 *	Overload this to provide application specific behaviour.
 *	@param bNowInFocus	Is the application now in focus.
 */
void			IRendererContext::NotifyFocusChange( bool bNowInFocus )
{
	m_bInFocus = bNowInFocus;
}

