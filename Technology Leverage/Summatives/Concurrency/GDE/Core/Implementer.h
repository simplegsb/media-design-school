/**
 *	@file GDE\Core\Implementer.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _IMPLEMENTER_H__Included_44349734
#define _IMPLEMENTER_H__Included_44349734

namespace GDE
{

/**
 *	@ingroup Core
 *
 *	All bridge implementers need to inherit from this.
 */
class IImplementer
{
public:
	virtual ~IImplementer( void ) {};	// force deconstructors to be virtual.
	
	/**
	 *	@return An unique description of the renderer implementation.
	 */
	virtual const CLibraryDescription&	GetDesc( void ) const = 0;
};

} // end namespace GDE

#endif //_IMPLEMENTER_H__Included_44349734

