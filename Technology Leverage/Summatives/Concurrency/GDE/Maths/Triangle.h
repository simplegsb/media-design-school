/**
 *	@file GDE\Maths\Triangle.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _TRIANGLE_H__Included_810359254
#define _TRIANGLE_H__Included_810359254

#include "FVector3.h"
#include "Plane.h"
#include "Sphere.h"
#include "GDE/Core/MemoryAlignment.h"

namespace GDE
{

/**
 *	@ingroup Maths
 *
 *	A triangle, consiting of three vertices.
 *	Each triangle will also store it's plane and it's bounding sphere.
 */
class CTriangle : public GDE::TAligned<16>
{
public:
	static void ClosestPointOnLineSegmentToPoint( CFVec3Arg vLineStart, CFVec3Arg vLineEnd, CFVec3Arg vPoint, CFVec3 &vClosest );

	bool		Init( CFVec3Arg v0, CFVec3Arg v1, CFVec3Arg v2 );	

	void		ComputeBaryCentric( CFVec3Arg vPoint, FLOAT32 &fV, FLOAT32 &fW );
	bool		ContainsPlanarPoint( CFVec3Arg vPoint );

	bool		IntersectsRay( CFVec3Arg vRayStart, CFVec3Arg vRayFwd, FLOAT32& fTime );	
	bool		SphereSweep( CSphereArg vSphereStart, CFVec3Arg vSweepDirection, FLOAT32 &fTime );

	const CPlane&	GetPlane( void );
	CFVec3Arg		GetVertex( GDE::UINT32 uiIndex );

protected:
	CFVec3	m_fv3Verts[3];		//!< The three bounding vertices.
	CPlane	m_Plane;			//!< The plane the triangle lies upon.
	CSphere	m_Bounds;			//!< The (minimum) bounding sphere for the triangle.
};

} // end namespace GDE.

#endif //_TRIANGLE_H__Included_810359254

