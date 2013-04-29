/**
 *	@file MobMoleMadness\LineSegment2D.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _LINESEGMENT2D_H__Included_2089235880
#define _LINESEGMENT2D_H__Included_2089235880

#include "FVector2.h"

class CCircle;

/**
 *	A 2D line segment.
 */
class CLineSegment2D
{
public:
	void Initialise( CFVec2Arg v0, CFVec2Arg v1 );

	bool Intersects( const CCircle& Circle ) const;
	bool Intersects( const CLineSegment2D& Other ) const;
	bool GetLineIntersectionTime( CFVec2Arg vRayStart, CFVec2Arg vRayOffset, FLOAT32& fT ) const;
	bool GetLineIntersectionTimes( CFVec2Arg vRayStart, CFVec2Arg vRayOffset, FLOAT32& fTOther, FLOAT32& fTThis ) const;
	bool GetClosestPointToPoint( CFVec2Arg vPoint, CFVec2& vClosest ) const;

	CFVec2Arg	GetEnd0( void ) const { return m_vEnd[0]; }
	CFVec2Arg	GetEnd1( void ) const { return m_vEnd[1]; }
protected:
	CFVec2	m_vEnd[2];			//!< The end points of the wall's line.
};

#endif //_LINESEGMENT2D_H__Included_2089235880

