/**
 *	@file GDE\Maths\Plane.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "Plane.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <xnamath.h>

using namespace GDE;

/**
 *	@return The plane normal unit vector.
 */
CFVec3Arg CPlane::GetNormal( void ) const
{
	const CFVec3& v3Return = *reinterpret_cast<const CFVec3*>( this );

	return v3Return;
}

/**
 *	@return The closest distance from the plane to the origin.
 */
FLOAT32 CPlane::GetD( void ) const
{
	return m_v4Plane.W();
}

/**
 *	@param	vNorm The plane normal unit vector to set.
 */
void	CPlane::SetNormal( CFVec3Arg vNorm )
{
	m_v4Plane.X( vNorm.X() );
	m_v4Plane.Y( vNorm.Y() );
	m_v4Plane.Z( vNorm.Z() );
}	

/**
 *	@param fD	The shortest distance from the plane to the origin.
 */
void	CPlane::SetD( FLOAT32 fD )
{
	m_v4Plane.W( fD );
}

/**
 *	Initialize the plane from 3 points with clockwise winding order defining the front of the plane.
 * 
 *  @param Pt1 The first point.
 *  @param Pt2 The second point.
 *  @param Pt3 The third point.
 */
void CPlane::InitFromPts( CFVec3Arg Pt1, CFVec3Arg Pt2, CFVec3Arg Pt3 )
{
	const XMVECTOR& v3PointA = *reinterpret_cast<const XMVECTOR*>( &Pt1 );
	const XMVECTOR& v3PointB = *reinterpret_cast<const XMVECTOR*>( &Pt2 );
	const XMVECTOR& v3PointC = *reinterpret_cast<const XMVECTOR*>( &Pt3 );
	XMVECTOR& v4Plane = *reinterpret_cast<XMVECTOR*>( this );

	v4Plane = XMPlaneFromPoints( v3PointA, v3PointB, v3PointC );
}

/**
 *	Initialize the plane from a normal and a point.
 *
 *  @param Normal The plane normal.
 *  @param Point The point on the plane.
 */
void CPlane::InitFromNormalAndPt( CFVec3Arg Normal, CFVec3Arg Point )
{
	const XMVECTOR& v3Point = *reinterpret_cast<const XMVECTOR*>( &Point );
	const XMVECTOR& v3Normal = *reinterpret_cast<const XMVECTOR*>( &Normal );

	XMVECTOR& v4Plane = *reinterpret_cast<XMVECTOR*>( this );

	v4Plane = XMPlaneFromPointNormal( v3Point, v3Normal);
}

/**
 *	Intersect an unbounded ray with a plane, will collide with either side of the plane.
 *
 *	@param fv3Start		The start point of the ray.
 *	@param fv3RayFwd	The ray forward vector.
 *	@param fTime		Filled with a value if the ray hits the plane.
 *						Value between zero and one lies along fv3RayForward from the start point.
 *	@return	true if the infinite ray intersects the plane, false otherwise when they are parallel.
 */
bool CPlane::RayIntersectionTime( CFVec3Arg fv3Start, CFVec3Arg fv3RayFwd, FLOAT32& fTime ) const
{
	// http://www.gamespp.com/algorithms/collisionDetectionTutorial02.html
	CFVec3 v3PlaneNormal = GetNormal();

	//Need angle between the plane normal and vector ray (both prenormalized)
	FLOAT32 fCosAlpha = fv3RayFwd.DotProduct( v3PlaneNormal );

	//If 90 deg, planes will never intersect
	if ( 0.0f == fCosAlpha )
	{
		return false;
	}

	//Maths assumes RHS, need to flip Z
	FLOAT32 fDelta =  -m_v4Plane.W() - fv3Start.DotProduct( v3PlaneNormal );
	
	fTime = fDelta / fCosAlpha;

	return true;
}

/**
 *	Intersect an unbounded ray with a plane - only collides with the front side of the plane...
 *
 *	@param fv3Start		The start point of the ray.
 *	@param fv3RayFwd	The ray forward vector.
 *	@param fTime		Filled with a value if the ray hits the plane.
 *						Value between zero and one lies along fv3RayForward from the start point.
 *	@return	true if the infinite ray intersects the plane, false otherwise when they are parallel.
 */
bool CPlane::RayIntersectionFwdTime( CFVec3Arg fv3Start, CFVec3Arg fv3RayFwd, FLOAT32& fTime ) const
{
	// http://www.gamespp.com/algorithms/collisionDetectionTutorial02.html
	CFVec3 v3PlaneNormal = GetNormal();

	//Need angle between the plane normal and vector ray (both prenormalized)
	FLOAT32 fCosAlpha = v3PlaneNormal.DotProduct( fv3RayFwd );

	//If 90 deg, planes will never intersect.
	//Also, normal and direction must be roughly opposite in direction.
	if ( 0.0f <= fCosAlpha )
	{
		return false;
	}

	//Maths assumes RHS, need to flip Z
	//FLOAT32 fPlaneDist = -1.0f * m_v4Plane.W();
	FLOAT32 fDelta =  -m_v4Plane.W() - fv3Start.DotProduct( v3PlaneNormal );
	
	if ( fDelta > 0.0f )
	{
		fTime = 0.0f;
		return false;
	}

	fTime = fDelta / fCosAlpha;

	return true;
}

/**
 *  Gets the distance from a point to the plane.  
 *
 *  @param fv3Point The point to get distance to.
 *	@return The shortest distance from the plane to a point.
 *			This value is signed so if the point is behind the plane 
 *			it will be negative.
 */
FLOAT32 CPlane::DistanceFromPoint( CFVec3Arg fv3Point ) const
{
	CFVec3 v3PlaneNormal = GetNormal();

	//Maths assumes RHS, need to flip Z - add dont subtract.
	FLOAT32 fDistance = v3PlaneNormal.DotProduct( fv3Point ) + m_v4Plane.W();

	return fDistance;
}

/**
 *	Project a vector so it lies on the plane.
 *
 *  @param vRay The vector to project onto the plane.
 *  @return The projected vector.
 */
CFVec3 CPlane::ProjectRay( CFVec3Arg vRay ) const
{
	return vRay - GetNormal()*vRay.DotProduct(GetNormal());
}

/**
 *	@param vPoint Point to get clostest point from.
 *	@return The closest point on the plane to vPoint.
 */
CFVec3 CPlane::PointClosestTo(CFVec3Arg rPoint) const
{
	FLOAT32 fT = 0.0f;
	RayIntersectionTime(rPoint, -GetNormal(), fT);
	return rPoint - (GetNormal() * fT);
}