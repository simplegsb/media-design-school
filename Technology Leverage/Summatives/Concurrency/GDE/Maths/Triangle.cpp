/**
 *	Construct the triangle from its three corner vertices in clockwise winding order.
 *
 *  @param vA The first point of the triangle. 
 *  @param vB The second point of the triangle. 
 *  @param vC The third point of the triangle. 
 *	@return false if the input verts are degenerate.
 */
#include "GDE/Core/Core.h"
#include "Triangle.h"
#include "FVector4.h"

#include <cmath>

using namespace GDE;

/**
 *  Test if the triangle intersects the given ray.
 *
 *	@param	vRayNormal	The normalised direction vector of the ray.
 *	@param	vRayStart	The start point on the ray.
 *	@param	fTime		Filled in with the parametric time for the intersection if there is one.
 *	@return true if the infinte ray intersects the triangle.
 */
bool CTriangle::Init( CFVec3Arg vA, CFVec3Arg vB, CFVec3Arg vC )
{
	//Cross product of two sides gives 2 * Area of the triangle.
	//If the cross product of two sides is 0, then we have a degenerate:
	CFVec3 v3CrossToCheck = ( vB - vA ).CrossProduct( ( vC - vA ) ) ;

	if ( GDE::g_fv3ZERO == v3CrossToCheck )
	{
		//Degenerate
		return false;
	}

	m_fv3Verts[0] = vA;
	m_fv3Verts[1] = vB;
	m_fv3Verts[2] = vC;

	m_Bounds.ConstructTriMinBounds( vA, vB, vC );

	CFVec3 v3Test = CFVec3::Init( 10.0, 10.0f, 0.0f );
	v3Test.Normalise();
	assert( v3Test.IsNormal() );

	m_Plane.InitFromPts( vA, vB, vC );

	return true;
}

/**
 *	Compute the Barycentric coordinates for the three points corner points
 *
 *	The third coordinate for the first corner (U) can be calculated as 1-(fV+fW)
 *	@param	vPoint	Input point.
 *	@param	fV		Filled in with the weight for the second corner.
 *	@param	fW		Filled in with the weight for the third corner.
 */
bool CTriangle::IntersectsRay( CFVec3Arg vRayStart, CFVec3Arg vRayFwd, FLOAT32& fTime )
{
	//Does ray intersect plane? If so, get intersection time.
	if ( false == m_Plane.RayIntersectionTime( vRayStart, vRayFwd, fTime ) )
	{
		return false;
	}

	//Calculate the point of intersection.
	CFVec3 v3Point = vRayStart + vRayFwd * fTime;

	//Does this point lie inside the triangle?
	return ContainsPlanarPoint( v3Point );
}

/**
 *	Does the triangle contain the planar point?
 *	@param	vPoint	A point on the triangle's plane to test.
 *	@return true if the point is contained.
 */
void CTriangle::ComputeBaryCentric( CFVec3Arg vPoint, FLOAT32 &fV, FLOAT32 &fW )
{
	//Real-time Collision Detection (pg 47-48).
	//http://books.google.co.nz/books?id=WGpL6Sk9qNAC&pg=PA46&lpg=PA46&dq=realtime+collision+detection+barycentric&source=bl&ots=Pl4VgK6alN&sig=hppIPE6qncb05YYbiv0UBjvOfyk&hl=en&ei=r-A8TO7DKILuvQOmsMDSDg&sa=X&oi=book_result&ct=result&resnum=1&ved=0CBQQ6AEwAA#v=onepage&q&f=false
	CFVec3 v30 = m_fv3Verts[1] - m_fv3Verts[0];
	CFVec3 v31 = m_fv3Verts[2] - m_fv3Verts[0];
	CFVec3 v32 = vPoint - m_fv3Verts[0];

	FLOAT32 fD00 = v30.DotProduct( v30 );
	FLOAT32 fD01 = v30.DotProduct( v31 );
	FLOAT32 fD11 = v31.DotProduct( v31 );
	FLOAT32	fD20 = v32.DotProduct( v30 );
	FLOAT32 fD21 = v32.DotProduct( v31 );
	FLOAT32 fDenom = fD00 * fD11 - fD01 * fD01;
	
	fV = ( fD11 * fD20 - fD01 * fD21 ) / fDenom;
	fW = ( fD00 * fD21 - fD01 * fD20 ) / fDenom;
} 

/**
 *  Retrieves the closest point on the given line segment to the given point.
 * 
 *  @param vLineStart The start point of the line segment.
 *  @param vLineEnd The end point of the line segment.
 *  @param vPoint The point to check against.
 *  @param vClosest Populated with the closest point on the line segement to the point. 
 */
bool CTriangle::ContainsPlanarPoint( CFVec3Arg vPoint )
{
	//Compute barycentric coords:
	FLOAT32 fU = 0.0f;
	FLOAT32 fV = 0.0f;
	ComputeBaryCentric( vPoint, fU, fV );

	if ( ( fU >= 0.0f ) && ( fV >= 0.0f ) && ( fU + fV <= 1.0f ) )
	{
		return true;
	}

	return false;
}

/**
 *	Sweep the sphere towards the triangle.
 *	If the sphere starts behind the plane then it will *not* collide.
 *	The sphere also will not collide with the ray if the ray is parallel
 *	to the triangle even if it should hit "end on".
 *	So make sure all collision primitives are closed and have volume if
 *	using this!
 *	TODO: a version than deals with these parallel edge collisions when the ray
 *	is parallel.  Requires sphere-ray collision code.
 *	In industry we would have to do this or have some code to confirm that art assets
 *	and any procedurally generated assets fulfill these requirements.
 *
 *	@param vSphereStart	The starting point of the sphere
 *	@param vDirection	The direction ray the sphere will move along.  
 *						The offset to the final position of the sphere given no collision.
 *	@param fTime		Filled in with the time of the sphere collision if there is one.
 *						AKA the multiplier for vDirection to give the offset to collided centre.
 *	@return true if there is a collision, otherwise false.
 */
void CTriangle::ClosestPointOnLineSegmentToPoint( CFVec3Arg vLineStart, CFVec3Arg vLineEnd, CFVec3Arg vPoint, CFVec3 &vClosest )
{
	//http://www.gamedev.net/community/forums/topic.asp?topic_id=495120
	CFVec3 v3U = vLineEnd - vLineStart;
	CFVec3 v3V = vPoint - vLineStart;
	
	FLOAT32 fULength;
	v3U.Normalise( fULength );
	
	FLOAT32 fUDoTV = v3U.DotProduct( v3V );

	if ( 0.0f > fUDoTV )
	{
		vClosest = vLineStart;
		return;
	}

	if ( fULength < fUDoTV )
	{
		vClosest = vLineEnd;
		return;
	}

	vClosest = v3U * v3U.DotProduct( v3V ) + vLineStart;
}

/**
 *  Returns the plane, useful for projecting a ray or getting the surface normal.
 *  
 *  @return Returns this triangle's plane.
 */
bool CTriangle::SphereSweep( CSphereArg SphereStart, CFVec3Arg vDirection, FLOAT32 &fTime )
{
	CFVec3 v3PlaneNormal = m_Plane.GetNormal();

	//Get the most important ray from the sphere. 
	//Move from the center towards the plane by the radius.
	CFVec3 v3PointOnSphere = SphereStart.GetPosition() + (-v3PlaneNormal * SphereStart.GetRadius() );

	//Return false if the sphere is starting from behind the plane:
	//Check for a collision:
	if ( false == m_Plane.RayIntersectionFwdTime( v3PointOnSphere, vDirection, fTime ) )
	{
		return false;
	}
	
	//Find the closest point of approach:
	CFVec3 v3ClosestPoint = v3PointOnSphere + vDirection * fTime;

	//If this point is inside the triangle, it will be our point of intersection:
	if ( ContainsPlanarPoint( v3ClosestPoint ) )
	{
		//Can simply move the sphere by t along the motion ray.
		return true;
	}

	//Otherwise, need to get the closest point on the triangle edge to the point where the plane
	//intersects the ray of closest approach:
	CFVec3 v3ClosestPointA;
	CFVec3 v3ClosestPointB; 
	CFVec3 v3ClosestPointC;
	FLOAT32 fADist;
	FLOAT32 fBDist;
	FLOAT32 fCDist;

	ClosestPointOnLineSegmentToPoint( m_fv3Verts[0], m_fv3Verts[1], v3ClosestPoint, 
									  v3ClosestPointA );

	ClosestPointOnLineSegmentToPoint( m_fv3Verts[1], m_fv3Verts[2], v3ClosestPoint, 
									  v3ClosestPointB );

	ClosestPointOnLineSegmentToPoint( m_fv3Verts[2], m_fv3Verts[0], v3ClosestPoint, 
									  v3ClosestPointC );

	//Pick the closest point:
	fADist = v3ClosestPointA.SquareDistance( v3ClosestPoint );
	fBDist = v3ClosestPointB.SquareDistance( v3ClosestPoint );
	fCDist = v3ClosestPointC.SquareDistance( v3ClosestPoint );
	
	v3ClosestPoint = v3ClosestPointA;
	FLOAT32 fClosestDist = fADist;

	//Pick the minimum:
	if ( fBDist <= fClosestDist )
	{
		v3ClosestPoint = v3ClosestPointB;
		fClosestDist = fBDist;
	}
	if ( fCDist <= fClosestDist )
	{
		v3ClosestPoint = v3ClosestPointC;
		fClosestDist = fCDist;
	}

	//Reverse ray cast from that point against the sphere:
	FLOAT32 fTimeTempA = 0.0f;
	FLOAT32 fTimeTempB = 0.0f;
	FLOAT32 fLength = 0.0f;

	CFVec3 v3ReverseDir = -vDirection;
	v3ReverseDir.Normalise( fLength );

	GDE::UINT32 uiNumRoots = SphereStart.GetRayIntersections( v3ClosestPoint, v3ReverseDir, 
															  fTimeTempA, fTimeTempB );
	if ( 0 < uiNumRoots )
	{
		if ( uiNumRoots > 1 )
		{
			fTime = fTimeTempA / fLength;
		}
		else
		{
			fTime = fTimeTempA > fTimeTempB ? fTimeTempB : fTimeTempA;
			fTime /= fLength;
		}		

		return true;
	}

	return false;
}

/**
 *  Returns the specified triangle vertex / point.
 *  
 *  @param uIndex The index of the vertex to retrieve.
 *  @return Returns the specified vertex.
 */
const CPlane&	
CTriangle::GetPlane( void )
{ 
	return m_Plane; 
}

/**
 *  Returns the specified triangle vertex / point.
 *  
 *  @param uIndex The index of the vertex to retrieve.
 *  @return Returns the specified vertex.
 */
CFVec3Arg	
CTriangle::GetVertex( GDE::UINT32 uIndex )
{ 
	assert_msg( false == (uIndex < 0 || uIndex >= 3), "Triange vertex %d is invalid", uIndex );

	return m_fv3Verts[uIndex];
}

