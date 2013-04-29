/**
 *	@file GDE\Renderer\Primitives.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _PRIMITIVES_H__Included_1002471904
#define _PRIMITIVES_H__Included_1002471904

namespace GDE
{

/**
 *	@ingroup Renderer
 *
 *	Each primitive type that can be drawn.
 *	Note: not all renderers will support all possible primitive types!
 */
enum EPrimitiveType
{
	PT_LineList = 0,	//!< List of line segments.
	PT_TriangleList,	//!< List of triangles.
	PT_QuadList,		//!< List of quads.
	PT_Count			// Must come last
};

}
#endif //_PRIMITIVES_H__Included_1002471904

