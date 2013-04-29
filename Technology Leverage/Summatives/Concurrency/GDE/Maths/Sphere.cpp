/**
 *	@file GDE\Maths\Sphere.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "Sphere.h"
#include "GDE/Maths/MathConstants.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <xnamath.h>
#include <cmath>

using namespace GDE;

/**
 *	Set the sphere to be the minimum bounds of a given triangle.
 *
 *  @param vA The first point of the triangle.
 *  @param vB The second point of the triangle.
 *  @param vC The third point of the triangle.
 */
void CSphere::ConstructTriMinBounds( CFVec3Arg vA, CFVec3Arg vB, CFVec3Arg vC )
{
	//http://realtimecollisiondetection.net/blog/?p=20

	CFVec3 v3NewSphereCenterPosition;

	CFVec3 BA = vB - vA;
	CFVec3 CA = vC - vA;

	FLOAT32 fDotABAB = BA.DotProduct( BA );
	FLOAT32 fDotABAC = BA.DotProduct( CA ); 
	FLOAT32 fDotACAC = CA.DotProduct( CA );
	FLOAT32 fD = 2.0f * ( fDotABAB * fDotACAC - fDotABAC * fDotABAC );

	CFVec3 v3RefPoint = vA;

	if ( fabsf( fD ) <= GDE::fEPSILON )
	{
		//All 3 points lie on the same line. Circle center is the center of AABB of the points, and
		//radius is distance from the circle center to AABB corner.

		v3RefPoint = vA;
		v3RefPoint.MinimizeWith( vB );
		v3RefPoint.MinimizeWith( vC );

		CFVec3 v3Max = vA;
		v3Max.MaximizeWith( vB );
		v3Max.MaximizeWith( vC );

		//AABB bBox = ;
		v3NewSphereCenterPosition = ( v3RefPoint + v3Max ) * 0.5f;
		//v3RefPoint = bBox.min;
	}
	else
	{
		FLOAT32 fS = ( fDotABAB * fDotACAC - fDotACAC * fDotABAC ) / fD;
		FLOAT32 fT = ( fDotACAC * fDotABAB - fDotABAB * fDotABAC ) / fD;

		//fS controls height over AC, fT over AB (1-s-t) over BC
		if ( 0.0f >= fS )
		{
			v3NewSphereCenterPosition = ( vA + vC ) * 0.5f;
		}
		else if ( 0.0f >= fT ) 
		{
			v3NewSphereCenterPosition = ( vA + vB ) * 0.5f;
		}
		else if ( 1.0f <= fS + fT )
		{
			v3NewSphereCenterPosition = ( vB + vC ) * 0.5f;

			v3RefPoint = vB;
		}	
		else
		{
			v3NewSphereCenterPosition = vA + ( ( vB - vA ) * fS + ( vC - vA ) * fT );
		}
	}

	//CFVec3 vMinusRefPt = v3NewSphereCenterPosition - v3RefPoint;
	//Faster sqrt method - proven by matt.
	//CFVec4 vec4Sqrt = CFVec4::Init( vMinusRefPt.DotProduct( vMinusRefPt ), 0.0f, 0.0f, 0.0f );
	//vec4Sqrt.FastSqrt();

	//FLOAT32 fRadius = vec4Sqrt.X();
	const FLOAT32 fRadius = v3NewSphereCenterPosition.Distance( v3RefPoint );

	m_v4Sphere = CFVec4::Init( v3NewSphereCenterPosition.X(), v3NewSphereCenterPosition.Y(), 
							   v3NewSphereCenterPosition.Z(), fRadius );
}


/**
 *  Get the center of the sphere.
 *  
 *	@return the position of the sphere's centre.
 */
CFVec3Arg CSphere::GetPosition( void ) const
{
	const CFVec3& v3Position = *reinterpret_cast<const CFVec3*>( this );

	return v3Position;
}

/**
 *	Set the sphere's position.
 *  
 *  @param vPos The new position of the center of the sphere.
 */
void CSphere::SetPosition( CFVec3Arg vPos )
{
	XMVECTOR& v3Position = *reinterpret_cast<XMVECTOR*>( this );

	v3Position = XMVectorSet( vPos.X(), vPos.Y(), vPos.Z(), m_v4Sphere.W() );
}

/**
 *  Get the sphere's radius.
 * 
 *	@return The radius of the sphere.
 */
FLOAT32 CSphere::GetRadius( void ) const
{
	return m_v4Sphere.W();
}

/**
 *  Set the sphere's radius.
 * 
 *	@param v The radius value to set.
 */
void CSphere::SetRadius( FLOAT32 fRadius )
{
	m_v4Sphere.W( fRadius );
}


/**
 *  Test if this sphere overlaps a given point.
 * 
 *	@param vPoint Point to check against.
 *	@return true if the point is contained.
 */
bool CSphere::OverlapsPoint( CFVec3Arg vPoint ) const
{
	//Essentially extending the 2d circle test into 3d:
	FLOAT32 fRadiusSqrd = m_v4Sphere.W() * m_v4Sphere.W();

	CFVec3 v3SpherePos = GetPosition();

	FLOAT32 fDistanceSqrd = ( ( vPoint.X() - v3SpherePos.X() ) *  ( vPoint.X() - v3SpherePos.X() ) ) + 
							( ( vPoint.Y() - v3SpherePos.Y() ) *  ( vPoint.Y() - v3SpherePos.Y() ) ) + 
							( ( vPoint.Z() - v3SpherePos.Z() ) *  ( vPoint.Z() - v3SpherePos.Z() ) );

	if ( fRadiusSqrd <= fDistanceSqrd )
	{
		return true;
	}

	return false;
}


/**
 *  Test if this sphere overlaps the given sphere.
 * 
 *	@param Other Sphere to check against
 *	@return true if the spheres touch or overlap.
 */
bool CSphere::OverlapsSphere( CSphereArg Other ) const
{
	//Essentially extending the 2d circle test into 3d:
	CFVec3 v3SpherePos = GetPosition();
	CFVec3 v3OtherSpherePos = Other.GetPosition();

	const FLOAT32 fDistSq = v3SpherePos.SquareDistance( v3OtherSpherePos );

	const FLOAT32 fRadiusSum = m_v4Sphere.W() + Other.GetRadius();

	return fDistSq <= fRadiusSum*fRadiusSum;
}

/**
 *	Can return 0, 1 or 2 intersections.
 *	MUST be given a normalised input vector.
 *	The ray is of infinite length.
 *
 *  @param fv3RayOrigin The origin of the ray.
 *  @param fv3RayNormal The normal of the ray.
 *  @param fTime1 The parametric time to the first intersection (if any).
 *  @param fTime2 The parametric time to the second intersection (if any).
 *  @return Returns the number of intersections.
 */
GDE::UINT32 CSphere::GetRayIntersections( CFVec3Arg fv3RayOrigin, CFVec3Arg fv3RayNormal, FLOAT32 &fTime1, FLOAT32 &fTime2 ) const
{
	//http://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection

	//Get position and radius from the vec4.
	//Same test as line test, but center = sphere center - ray origin.
	const CFVec3 v3Center = GetPosition() - fv3RayOrigin;
	const FLOAT32 fRadius = GetRadius();
	const FLOAT32 fRadiusSqr = fRadius * fRadius;
	
	//Optimization: Save this for later, used to compute discriminant and intersection times.
	const FLOAT32 fRayDotCenter = fv3RayNormal.DotProduct( v3Center );

	//Calculate the discriminant
	const FLOAT32 fDiscriminant = fRayDotCenter * fRayDotCenter - v3Center.SquareMagnitude() + fRadiusSqr;


	//If fDiscriminant is negative, there is no real roots (no solution).
	if ( 0.0f > fDiscriminant )
	{
		return 0;
	}

//	CFVec4 vec4Sqrt = CFVec4::Init( fDiscriminant, 
//									0.0f, 0.0f, 0.0f );
//	vec4Sqrt.FastSqrt();
//	const FLOAT32 fSqrt = vec4Sqrt.X();
	const FLOAT32 fSqrt = sqrtf( fDiscriminant );

	//If fDiscriminant is 0, there is one real root (one solution).
	if ( 0.0f == fDiscriminant )
	{
		fTime1 = fRayDotCenter + fSqrt;

		return 1;
	}
	//If fDiscriminant is positive, there are two real roots (two solution).
	fTime1 = fRayDotCenter - fSqrt;

	fTime2 = fRayDotCenter + fSqrt;

	return 2;
}

/**
 *	This only returns true on an intersection along the length of the bounded ray
 *	and also only the first collision if there are two.
 *
 *  @param fv3RayOrigin The origin of the ray.
 *	@param fv3RayMotion	The direction vector with magnitude of the length of the ray.
 *  @return True if the line intersects the sphere, else false.
 */
bool CSphere::LineIntersects( CFVec3Arg fv3RayOrigin, CFVec3Arg fv3RayMotion, FLOAT32& fTime ) const
{
#pragma warnmsg( "Nothing done when fdiscr is zero... potential divide by zero here..." )
	//http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection
	
	const CFVec3 v3Origin = fv3RayOrigin - GetPosition();

	const FLOAT32 fA = fv3RayMotion.DotProduct( fv3RayMotion );
	const FLOAT32 fB = 2.0f * fv3RayMotion.DotProduct( v3Origin );
	const FLOAT32 fC = v3Origin.DotProduct( v3Origin ) - ( GetRadius() * GetRadius() );

	//Find discriminant:
	const FLOAT32 fDiscriminant = fB * fB - 4 * fA * fC;

	//Line is in the negative direction, line misses the sphere
	if ( fDiscriminant < 0.0f )
	{
		return false;
	}

	
//	CFVec4 vec4Sqrt = CFVec4::Init( fDiscriminant, 0.0f, 0.0f, 0.0f );
//	vec4Sqrt.FastSqrt();
//	const FLOAT32 fSqrtDiscr = vec4Sqrt.X();
	const FLOAT32 fSqrtDiscr = sqrtf( fDiscriminant );

	const FLOAT32 fDist = fSqrtDiscr;
	const FLOAT32 fQ = (fB < 0.0f) ? ( -fB - fDist ) * 0.5f : ( -fB + fDist ) * 0.5f;


	//Find point of intersection:
	const FLOAT32 fT0 = fQ / fA;
	const FLOAT32 fT1 = fC / fQ;



	//Intersection point is at t1.
	if ( fT0 > fT1 )
	{
		fTime = fT1;
	} else
	{
		fTime = fT0;
	}

	return true;
}