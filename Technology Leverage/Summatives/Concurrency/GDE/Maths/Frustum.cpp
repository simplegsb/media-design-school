/**
 *	@file GDE\Maths\Frustum.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "Frustum.h"
#include "AABB.h"

using namespace GDE;

/**
 *	Does the frustum overlap a sphere?
 *
 *	@return true if any part of them can be considered overlapping.
 *	Note: may produce false positives for 'corner' cases.
 */
bool CFrustum6::OverlapsSphere( CSphereArg Sphere ) const
{
	UNUSED_VAR( Sphere );
#pragma warnmsg( "stub function that needs implementation" )
	assert_now( "stub function" );
	return false;
}

/**
 *	Does the Frustum overlap the primitive enclosed by the points passed in?
 *
 *	@param pPoints		Pointer to start of array of points.
 *	@param uNumPoints	The number of points defining the bounds of the primitive.
 *
 *	@return true if the primitive overlaps the frustum.
 *	Note: may produce false positives for 'corner' cases.
 */
bool CFrustum6::OverlapsEnclosingPoints( const CFVec3* pPoints, UINT32 uNumPoints ) const
{
	UNUSED_VAR( pPoints );
	UNUSED_VAR( uNumPoints );
#pragma warnmsg( "stub function that needs implementation" )
	assert_now( "stub function" );
	return false;
}


/**
 *	Does the frustum overlap the axis aligned bounding box?
 *
 *	@param	AABB	The axis aligned bounding box to test against.
 *	@return	true if the two overlap in space.
 *
 *	Note: may produce false positives for 'corner' cases.
 */
bool CFrustum6::OverlapsAABB( const CAABB& AABB )
{
	UNUSED_VAR( AABB );
#pragma warnmsg( "stub function that needs implementation" )
	assert_now( "stub function" );
	return false;
}

/**
 *	Build a frustum to enclose a view projection.
 *
 *	@param vForwards	View forwards vector.
 *	@param vUp			View up vector.
 *	@param vOrigin		View origin (Camera position).
 *	@param fNear		Near plane distance.
 *	@param fFar			Far plane distance.
 *	@param fFOV			Vertical field of view of view.
 *	@param fAspect		Aspect ratio of view (Width/Height).
 */
void CFrustum6::InitForViewProjection(
						CFVec3Arg vForwards,
						CFVec3Arg vUp,
						CFVec3Arg vOrigin,
						FLOAT32 fNear,
						FLOAT32 fFar,
						FLOAT32 fFOV,
						FLOAT32 fAspect )
{
#pragma warnmsg( "stub function that needs implementation" )
	assert_now( "stub function" );
	UNUSED_VAR( vForwards );
	UNUSED_VAR( vUp );
	UNUSED_VAR( vOrigin );
	UNUSED_VAR( fNear );
	UNUSED_VAR( fFar );
	UNUSED_VAR( fFOV );
	UNUSED_VAR( fAspect );
}

