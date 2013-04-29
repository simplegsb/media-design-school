/**
 *	@file GDE\Maths\Plane.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _PLANE_H__Included_885383309
#define _PLANE_H__Included_885383309

#include "GDE/Maths/FVector3.h"
#include "GDE/Maths/FVector4.h"
#include "GDE/Core/MemoryAlignment.h"

namespace GDE
{

typedef const class CPlane& CPlaneArg;		//!< How to pass a plane as an argument (or return it).

/**
 *	@ingroup Maths
 *
 *	This class represents a plane in 3D space.
 */
class CPlane : public GDE::TAligned<16>
{
public:
	CFVec3Arg	GetNormal( void ) const;
	FLOAT32		GetD( void ) const;
	void		SetNormal( CFVec3Arg vNorm );
	void		SetD( FLOAT32 fD );

	void		InitFromPts( CFVec3Arg Pt1, CFVec3Arg Pt2, CFVec3Arg Pt3 );
	void		InitFromNormalAndPt( CFVec3Arg Normal, CFVec3Arg Point );


	bool		RayIntersectionTime( CFVec3Arg fv3Start, CFVec3Arg fv3RayFwd, FLOAT32& fTime ) const;
	bool		RayIntersectionFwdTime( CFVec3Arg fv3Start, CFVec3Arg fv3RayFwd, FLOAT32& fTime ) const;
	FLOAT32		DistanceFromPoint( CFVec3Arg fv3Point ) const;

	CFVec3		ProjectRay( CFVec3Arg vRay ) const;
	CFVec3		PointClosestTo(CFVec3Arg rPoint) const;

protected:
	CFVec4		m_v4Plane; //<<! Plane is defined by a normal and distance.
};

} // end namespace GDE

#endif //_PLANE_H__Included_885383309

