/**
 *	@file GDE\Renderer\RendererImp.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright ? 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _RENDERERIMP_H__Included_98756839
#define _RENDERERIMP_H__Included_98756839

#include "GDE/Core/Implementer.h"
#include "GDE/Renderer/Primitives.h"

namespace GDE
{
class IRendererContext;
class CLibraryDescription;
class ITextureManagerImp;
class IFontManagerImp;
class IVertexDeclarationManagerImp;
class ITextureStageStateManagerImp;
class IMaterialManagerImp;
class IVertexBufferManagerImp;
class IIndexBufferManagerImp;
class ILightManagerImp;
class CFMat4x4;
class CTextureBlendState;
class IShaderManagerImp;

/**
 *	@ingroup Renderer
 *
 *	The interface for the concrete implementation of the CRenderer class.
 *	This version is for rendere 1.0.0 functionality.
 */
class IRendererImp_1_0_0 : public IImplementer
{
public:
	// these functions are for initialisation:
	virtual bool				SetContext( IRendererContext& Context ) = 0;
	virtual void				RunGameLoop( void ) = 0;

	// these functions are for asset management:
	virtual ITextureManagerImp*	GetTextureManager( void ) = 0;

	// these functions are for rendering:
	virtual bool				BeginScene( void ) = 0;
	virtual bool				EndScene( void ) = 0;
	virtual bool				ColourClear( UINT32 colClear ) = 0;

	virtual bool				DrawPrimitives( EPrimitiveType eType, UINT32 uPrimitiveCount, const void* pVertices, UINT32 uVertexStride ) = 0;
};

/**
 *	The default renderer implementation.
 *
	Needs to be a class not a typedef so it can be forward references.
 */
class IRendererImp : public IRendererImp_1_0_0
{

};

} // end namespace GDE
#endif //_RENDERERIMP_H__Included_98756839

