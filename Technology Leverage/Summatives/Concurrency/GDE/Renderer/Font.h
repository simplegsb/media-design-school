/**
 *	@file GDE\Renderer\Font.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _FONT_H__Included_2005467751
#define _FONT_H__Included_2005467751

#include "GDE/Core/ResourceBridge.h"
#include "GDE/Renderer/FontConstants.h"

namespace GDE
{

class IFontImp;
class CFontManager;
class IFontManagerImp;

/**
 *	@ingroup Renderer
 *
 *	The font class.
 */
class CFont : public TResourceBridge< CFont, CFontManager, IFontImp, IFontManagerImp, CREATE_DLL_ID( 'F','O','N','T') >
{
public:
	typedef CFont TThis;
	typedef IFontImp TImplementer;	//!< All bridges need this typedef in the final type.
	typedef TResourceBridge< CFont, CFontManager, IFontImp, IFontManagerImp, CREATE_DLL_ID( 'F','O','N','T') > TParent;
	typedef TResourceManagerBridge< CFontManager, IFontManagerImp, TThis, CREATE_DLL_ID( 'F','O','N','T') > TManagerBase;

	bool Render( FLOAT32 fX, FLOAT32 fY, EFontJustification eJustify, const char* strMessage );
	bool SetColour( UINT32 colFont );
	UINT32 GetColour( void ) const;
	FLOAT32 GetHeight( void ) const;
	FLOAT32 GetStringWidth( const char* strMessage ) const;
protected:
	// need to be a special friend to allow the manager access to create these:
	friend class CFontManager;
	friend class TManagerBase;

	CFont( IFontImp* pImplementer, const CResourceId& Id, CFontManager* pManager, CFont* pNext );
};

} // end namespace GDE

#endif //_FONT_H__Included_2005467751

