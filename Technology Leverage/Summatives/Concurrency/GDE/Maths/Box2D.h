/**
 *	@file GDE\Maths\Box2D.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _BOX2D_H__Included_129105959
#define _BOX2D_H__Included_129105959

#include "FVector2.h"

namespace GDE
{

class CCircle;

/**
 *	@author Jon Booth.
 *	@ingroup Maths
 *
 *	A 2D axis aligned box (or rectangle)
 */
class CBox2D
{
public:
	bool		InitExtents( CFVec2Arg vCentre, CFVec2Arg vExtents );
	bool		InitCorners( CFVec2Arg v0, CFVec2Arg v1 );

	bool		Intersects( const CCircle& Circle ) const;
	bool		Intersects( const CBox2D& Box ) const;
	bool		OverlapsLineSegment( CFVec2Arg v0, CFVec2Arg v1 ) const;
	bool		ContainsPoint( CFVec2Arg vPoint ) const;
	CFVec2Arg	GetCentre( void ) const { return m_vCentre; }
	CFVec2Arg	GetExtents( void ) const { return m_vExtents; }
protected:
	CFVec2 m_vCentre;		//!< The centre point of the box.
	CFVec2 m_vExtents;		//!< The box Extents.
};

} // end namespace GDE

#endif //_BOX2D_H__Included_129105959

