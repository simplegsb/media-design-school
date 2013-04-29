/**
 *	@file GDE\Maths\Frustum.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _FRUSTUM_H__Included_72124492
#define _FRUSTUM_H__Included_72124492

#include "GDE/Maths/Plane.h"
#include "GDE/Maths/Sphere.h"

namespace GDE
{

class CAABB;	// fwd reference.

/**
 *	@ingroup Maths
 *
 *	A six sided frustum often used to represent the space a camera looks at.
 */
class CFrustum6
{
public:
	enum EPlaneName
	{
		TOP = 0,
		RIGHT,
		BOTTOM,
		LEFT,
		NARROW,	// AKA the near plane
		WIDE	// AKA the far plane
	};

	EPlaneName GetOppositePlane( EPlaneName ePlane ) const
	{
		return sm_eOppositePlanes[ePlane];
	}

	void SetPlane( CPlaneArg Plane, EPlaneName ePlane )
	{
		m_Planes[ePlane] = Plane;
	}

	CPlaneArg GetPlane( EPlaneName ePlane ) const
	{
		return m_Planes[ePlane];
	}

	CPlane& GetPlaneRef( EPlaneName ePlane ) 
	{
		return m_Planes[ePlane];
	}

	void InitForViewProjection(
						CFVec3Arg vForwards,
						CFVec3Arg vUp,
						CFVec3Arg vOrigin,
						FLOAT32 fNear,
						FLOAT32 fFar,
						FLOAT32 fFOV,
						FLOAT32 fAspect );

	bool OverlapsSphere( CSphereArg Sphere ) const;
	bool OverlapsEnclosingPoints( const CFVec3* pPoints, UINT32 uNumPoints ) const;
	bool OverlapsAABB( const CAABB& AABB );
	bool ContainsPoint( CFVec3Arg vPoint ) const;
	bool ContainsPointIgnoringPlane( CFVec3Arg vPoint, EPlaneName ePlaneToIgnore ) const;
protected:
	const static EPlaneName sm_eOppositePlanes[6];
	const static EPlaneName sm_eOtherPlanes[6][5];
	CPlane	m_Planes[6];
};


} // end namespace GDE
#endif //_FRUSTUM_H__Included_72124492

