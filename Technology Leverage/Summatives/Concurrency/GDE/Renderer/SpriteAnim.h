/**
 *	@file GDE\Renderer\SpriteAnim.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _SPRITEANIM_H__Included_1313763516
#define _SPRITEANIM_H__Included_1313763516

#include "GDE/Core/ResourceBridge.h"

namespace GDE
{

class ISpriteAnimImp;
class CSpriteAnimManager;
class ISpriteAnimManagerImp;
class CTexture;
class CSprite;
struct SFRect;

/**
 *	@ingroup Renderer
 *
 *	The animated (flipbook) sprite class.
 */
class CSpriteAnim : public TResourceBridge< CSpriteAnim, CSpriteAnimManager, ISpriteAnimImp, ISpriteAnimManagerImp, CREATE_DLL_ID( 'S','P','R','A' ) >
{
public:

	typedef CSpriteAnim TThis;
	typedef ISpriteAnimImp TImplementer;	//!< All bridges need this typedef in the final type.
	typedef TResourceBridge< CSpriteAnim, CSpriteAnimManager, ISpriteAnimImp, ISpriteAnimManagerImp, CREATE_DLL_ID( 'S','P','R','A') > TParent;
	typedef TResourceManagerBridge< CSpriteAnimManager, ISpriteAnimManagerImp, TThis, CREATE_DLL_ID( 'S','P','R','A' ) > TManagerBase;

	// functionality.
	bool	Play( void );
	bool	IsPlaying( void ) const;
	bool	Stop( void );
	bool	Reset( void );
	bool	SetLooping( bool bValue );
	bool	IsLooping( void ) const;
	bool	SetReverse( bool bValue );
	bool	IsReverse( void ) const;
	bool	SetPingPong( bool bValue );
	bool	IsPingPong( void ) const;
	bool	SetSpeed( FLOAT32 fSpeed );
	FLOAT32	GetSpeed( void ) const;
	UINT32	GetFrame( void ) const;
	UINT32	GetFrameCount( void ) const;
	bool	Update( FLOAT32 fTimeDelta );
	CSprite* GetSprite( void );

protected:
	// need to be a special friend to allow the manager access to create these:
	friend class CSpriteAnimManager;	// also the sprite manager needs to do this!
	friend class TManagerBase;
	/**
	 *	Constructor - only used by the manager, has to be overloaded though explicitly
	 */
	CSpriteAnim( ISpriteAnimImp* pImplementer, const CResourceId& Id, CSpriteAnimManager* pManager, CSpriteAnim* pNext )
	: TParent( pImplementer, Id,  pManager, pNext ) 
	{	// empty.
	}
};

} // end namespace GDE

#endif //_SPRITEANIM_H__Included_1313763516

