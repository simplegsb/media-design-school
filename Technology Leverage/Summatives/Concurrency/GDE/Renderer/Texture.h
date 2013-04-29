/**
 *	@file GDE\Renderer\Texture.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright ? 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _TEXTURE_H__Included_278708145
#define _TEXTURE_H__Included_278708145

#include "GDE/Core/ResourceBridge.h"

namespace GDE
{
	class ITextureImp;
	class CTextureManager;
	class ITextureManagerImp;
	class CResourceId;
	struct SIRect;

	/**
	 *	@ingroup Renderer
	 *
	 *	A texture, can be extended to also be a render target, or could be the NULL texture (requesting untextured rendering).
	 */
	class CTexture : public TResourceBridge< CTexture, CTextureManager, ITextureImp, ITextureManagerImp, CREATE_DLL_ID( 'R','N','D','R') >
	{
	public:
		typedef CTexture TThis;
		typedef ITextureImp TImplementer;	//!< All bridges need this typedef in the final type.
		typedef TResourceBridge< CTexture, CTextureManager, ITextureImp, ITextureManagerImp, CREATE_DLL_ID( 'R','N','D','R') > TParent;
		typedef TResourceManagerBridge< CTextureManager, ITextureManagerImp, TThis, CREATE_DLL_ID( 'R','N','D','R') > TManagerBase;

		bool		Apply( UINT32 uTextureStage );
		UINT32		GetWidth( void ) const;
		UINT32		GetHeight( void ) const;
		bool		LockTexels( UINT32* &pTexels, UINT32& uPitch, bool bReadOnly = true, const SIRect* pRegion = NULL );
		bool		UnlockTexels( void );
		bool		Target( UINT32 uIndex );
	protected:
		// need to be a special friend to allow the manager access to create these:
		friend class CTextureManager;
		friend class TManagerBase;

		/**
		 *	Constructor - only used by the manager, has to be overloaded though explicitly
		 */
		CTexture( ITextureImp* pImplementer, const CResourceId& Id, CTextureManager* pManager, CTexture* pNext )
		: TParent( pImplementer, Id, pManager, pNext )
		{	// empty.
		}
	};
} // end namespace GDE

#endif //_TEXTURE_H__Included_278708145

