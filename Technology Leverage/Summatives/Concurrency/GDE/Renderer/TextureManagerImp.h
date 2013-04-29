/**
 *	@file GDE\Renderer\TextureManagerImp.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _TEXTUREMANAGERIMP_H__Included_354835106
#define _TEXTUREMANAGERIMP_H__Included_354835106

#include "GDE/Core/ResourceManagerImp.h"

namespace GDE
{
	class ITextureImp;

	/**
	 *	@ingroup Renderer
	 *
	 *	The texture manager Implementer.
	 */
	class ITextureManagerImp_1_0_0 : public TIResourceManagerImp<ITextureImp>
	{
	public:
		/**
		 *	@return A NULL texture implementation, which when bound to the device means untextured or to disable texturing.
		 */
		virtual bool GetNULLTexture( ITextureImp*& pNULLTexture ) = 0;

	};

	/**
	 *	The final (latest version) of the texture manger interface.
	 *	Is an inherited class rather than a typedef so it can be forward declared.
	 */
	class ITextureManagerImp : public ITextureManagerImp_1_0_0
	{
	};

}	// end namespace GDE

#endif //_TEXTUREMANAGERIMP_H__Included_354835106

