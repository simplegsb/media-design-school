/**
 *	@file GDE\Renderer\Renderer.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "Renderer.h"
#include "RendererImp.h"
#include "TextureManager.h"
#include "FontManager.h"
#ifdef _PS2_
# include "GDE/Core/StaticLibraries.h"
#endif
namespace GDE
{

/**
 *	@defgroup Renderer Renderer Library
 *
 *	The renderer interface provides rendering services to the user.
 *	Typically a visual game will create a CRenderer and then give it
 *	a concrete implementation of IRendererContext that contains that
 *	game's logic.
 *
 */

// these functions are for initialisation:
CRenderer::CRenderer( void )
	: TSuperClass()
	, m_pTextureManager( NULL )
{	// Empty

#ifdef _PS2_
	// nasty hack - force the PS2 renderer to be registered.
	static bool sbDoPS2Renderer = true;
	if ( sbDoPS2Renderer )
	{
		sbDoPS2Renderer = false;
		extern void* CreatePS2Renderer( void );
		extern void DeletePS2Renderer( void* pRenderer );
		_REGISTER_STATIC_LIB( CreatePS2Renderer, DeletePS2Renderer, "PS2Renderer", 'R','N','D','R' );
	}
#endif
}

CRenderer::~CRenderer( void )
{
	delete m_pTextureManager;
}

bool CRenderer::LoadDLL( const char* strDLLName )
{
	if ( TSuperClass::LoadDLL( strDLLName ) )
	{
		return true;
	}
	return false;
}

void CRenderer::ReleaseDLLImplementer( void )
{
	TSuperClass::ReleaseDLLImplementer();
}

/**
 *	Set the renderer context and initialize the renderer.
 *
 *	@param Context	The context to use to render and define the renderer properties.
 *	@return success.
 */
bool CRenderer::SetContext( IRendererContext& Context )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	bool bOK = m_pImplementer->SetContext( Context );
	if ( bOK )
	{
		if ( m_pTextureManager )
		{
			delete m_pTextureManager;
		}
		// get a texture manager.
		ITextureManagerImp* pTMImp = m_pImplementer->GetTextureManager();
		m_pTextureManager = new CTextureManager( pTMImp ); 
	}
	return bOK;
}

/**
 *	Start the renderer running the main game loop.
 */
void CRenderer::RunGameLoop( void )
{
	if ( NULL == m_pImplementer )
	{
		assert_now( "There is no Implementer for the renderer bridge" );
		return;
	}
	m_pImplementer->RunGameLoop();
}

// these functions are for asset management:

// these functions are for rendering:
/**
 *	Start the renderer into a new scene.
 *
 *	@return success.
 */
bool CRenderer::BeginScene( void )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->BeginScene();
}

/**
 *	Finish the current scene.
 *
 *	@return success.
 */
bool CRenderer::EndScene( void )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->EndScene();
}

/**
 *	Clear the current scene.
 *
 *	@param colClear	The colour to clear to.
 *
 *	@return success.
 */
bool CRenderer::ColourClear( UINT32 colClear )
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->ColourClear( colClear );
}

/**
 *	Draw one or more primitives.
 *
 *	@param eType			The type of primitive to draw.
 *	@param uPrimitiveCount	The number of primitives to draw.
 *	@param pVertices		The vertices to be drawn.
 *	@param uVertexStride	The stride (in bytes) of the vertices to be drawn.
 */
bool CRenderer::DrawPrimitives(EPrimitiveType eType, UINT32 uPrimitiveCount, const void *pVertices, UINT32 uVertexStride)
{
	_BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer();
	return m_pImplementer->DrawPrimitives( eType, uPrimitiveCount, pVertices, uVertexStride );	
}

CTextureManager& CRenderer::GetTextureManager( void )
{
	return *m_pTextureManager;
}

} // end namespace GDE
