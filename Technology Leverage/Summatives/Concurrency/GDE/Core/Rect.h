/**
 *	@file GDE\Core\Rect.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _RECT_H__Included_521458946
#define _RECT_H__Included_521458946

namespace GDE
{

/**
 *	@ingroup Core
 *
 *	A rectangle structure, containing floats.
 */
struct SFRect
{
	FLOAT32 m_fX;		//!< Top left.
	FLOAT32	m_fY;		//!< Top right.
	FLOAT32	m_fWidth;	//!< The width.
	FLOAT32 m_fHeight;	//!< The height.
};

/**
 *	@ingroup Core
 *
 *	A rectangle structure, containing signed ints.
 */
struct SIRect
{
	SINT32	m_iX;		//!< Top left.
	SINT32	m_iY;		//!< Top right.
	SINT32	m_iWidth;	//!< The width.
	SINT32	m_iHeight;	//!< The height.
};
} // end namespace GDE

#endif //_RECT_H__Included_521458946

