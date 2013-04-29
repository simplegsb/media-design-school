/**
 *	@file GDE\Maths\AABB.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "AABB.h"

using namespace GDE;

/**
 *	Initialise to be the AABB that minimally contains the points passed in
 *
 *	Degenerate bounds are allowed, however the must be at least one point.
 *
 *	@param pPoints		Pointer to array of points to be contained.
 *	@param uNumPoints	Number of points in pPoints array.
 */
void CAABB::InitForPoints( const CFVec3* pPoints, UINT32 uNumPoints )
{
#pragma warnmsg( "stub function that needs implementation" )
	assert_now( "stub function" );
	UNUSED_VAR( pPoints );
	UNUSED_VAR( uNumPoints );
}

/**
 *	Initialise this to contain a centre points and extents from that centre point
 *	to a corner.
 *	So the extents passed in are half the overall extents of the bounds.
 *
 *	@param vCentre	The centre of the primitive.
 *	@param vExtents	The half extents of the final primitive.
 */
void CAABB::InitForCentreExtents( CFVec3Arg vCentre, CFVec3Arg vExtents )
{
	UNUSED_VAR( vCentre );
	UNUSED_VAR( vExtents );
#pragma warnmsg( "stub function that needs implementation" )
	assert_now( "stub function" );
}

/**
 *	Initialise this to cover a minimum and maximum point
 *
 *	May be degenerate but min must be less or equal to max.
 *	@param vMin	Minimum point of the AABB
 *	@param vMax Maximum point of the AABB
 */
void CAABB::InitForMinMax( CFVec3Arg vMin, CFVec3Arg vMax )
{
	UNUSED_VAR( vMin );
	UNUSED_VAR( vMax );
	assert_msg( vMin.GetMin( vMax ) == vMin, "Min and max must be correct" );
#pragma warnmsg( "stub function that needs implementation" )
	assert_now( "stub function" );
}

/**
 *	Fill the array passed in with the corners of the AABB
 *
 *	@param pCorners	Array to be filled in with the corners.  Must have space for 8 points.
 */
void CAABB::GetCorners( CFVec3* pCorners ) const
{
	UNUSED_VAR( pCorners );
#pragma warnmsg( "stub function that needs implementation" )
	assert_now( "stub function" );
}
