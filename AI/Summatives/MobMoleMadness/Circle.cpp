/**
 *	@file MobMoleMadness\Circle.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "Circle.h"

#include <math.h>
/**
 *	@param vCentre	The centre of the circle to set.
 *	@param fRadius	The radius to set the circle to.
 */
void CCircle::Initialise( CFVec2Arg vCentre, FLOAT32 fRadius )
{
	m_vCentre = vCentre;
	m_fRadius = fRadius;
}

/**
 *	@param Other	A circle to test against.
 *	@return			True if the two circles overlap.
 */
bool CCircle::Overlaps(const CCircle &Other) const
{
	const FLOAT32 fDistSq = m_vCentre.SquareDistance( Other.m_vCentre );
	const FLOAT32 fRadiusSum = m_fRadius + Other.m_fRadius;
	return fDistSq <= fRadiusSum*fRadiusSum;
}

/**
 *	@param vRayStart	Start point of an infinite ray.
 *	@param vRayOffset	Direction of the ray, parametric t multiplier.
 *	@param fT0			Filled in with time of first intersection (if present).
 *	@param fT1			Filled in with time of second intersection (if present).
 *	@return				The number of intersections found.
 */
UINT32 CCircle::GetLineIntersectionTimes(CFVec2Arg vRayStart, CFVec2Arg vRayOffset, GDE::FLOAT32 &fT0, GDE::FLOAT32 &fT1) const
{
	const CFVec2 vToRay = vRayStart - m_vCentre;
	// TODO: reformulate without the square root?
	const FLOAT32 fExtentsRecip = 1.0f/vRayOffset.Magnitude();
	const FLOAT32 fRayDotToRay = vToRay.DotProduct( vRayOffset ) * fExtentsRecip;
	const FLOAT32 fDiscr = fRayDotToRay*fRayDotToRay - (vToRay.SquareMagnitude()-m_fRadius*m_fRadius);
	if ( fDiscr <= 0.0f )
	{
		return 0;	// no intersections.
	}
	if ( fDiscr == 0.0f )
	{
		fT0 = -fRayDotToRay * fExtentsRecip;
		return 1;	// one intersection, just touching
	} else
	{
		const FLOAT32 fRoot = sqrtf( fDiscr );
		fT0 = (-fRayDotToRay - fRoot) * fExtentsRecip;
		fT1 = (-fRayDotToRay + fRoot) * fExtentsRecip;
		return 2;	// two intersections.
	}
}

/**
 *	@param v0	One end of line segment.
 *	@param v1	The other end of the line segment.
 *	@return Does the circle overlap the line segment?
 */
bool	CCircle::OverlapsLineSegment( CFVec2Arg v0, CFVec2Arg v1 ) const
{
	// calculate all intersections on the infinite line:
	FLOAT32 fIntersections[2];
	const UINT32 uNumIntersections = GetLineIntersectionTimes( v0, v1-v0, fIntersections[0], fIntersections[1] );
	for ( UINT32 i=0; i<uNumIntersections; i++ )
	{
		// is the intersection within the line segment?
		if ( fIntersections[i] >= 0.0f && fIntersections[i] <= 1.0f )
		{
			return true;
		}
	}
	return false;
}
