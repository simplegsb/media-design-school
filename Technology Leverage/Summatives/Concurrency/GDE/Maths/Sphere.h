/**
 *	@file GDE\Maths\Sphere.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _SPHERE_H__Included_192325287
#define _SPHERE_H__Included_192325287

#include "FVector3.h"
#include "FVector4.h"
#include "GDE/Core/MemoryAlignment.h"

namespace GDE
{
/**
 *	@author Jon Booth.
 *	@ingroup Maths
 *
 *	When passing a CSpheres around, use this as an argument rather than 
 *	directly passing by value, or by reference - unless that specific behaviour
 *	is desired. As a result, platform and implementation optimised passing behaviour
 *	can be implemented.
 */
typedef const class CSphere& CSphereArg; // might want to pass by value on some architectures.


/**
 *	@ingroup Maths
 *
 *	This class represents a sphere, has a centre and a volume.
 */
class CSphere : public GDE::TAligned<16>
{
public:
	void ConstructTriMinBounds( CFVec3Arg v0, CFVec3Arg v1, CFVec3Arg v2 );

	CFVec3Arg GetPosition( void ) const;
	void SetPosition( CFVec3Arg vPos );
	FLOAT32 GetRadius( void ) const;
	void SetRadius( FLOAT32 fRadius );

	bool OverlapsSphere( CSphereArg Other ) const;
	bool OverlapsPoint( CFVec3Arg vPoint ) const;

	// This only returns true on an intersection along the length of the line motion ( 0->1 )
	// and also only the first collision if there are two.
	bool LineIntersects( CFVec3Arg fv3LineOrigin, CFVec3Arg fv3LineMotion, FLOAT32& fTime ) const;

	// can return 0, 1 or 2 intersections - MUST be given a normalised input vector fTime1 will always be < fTime2
	UINT32 GetRayIntersections( CFVec3Arg fv3RayOrigin, CFVec3Arg fv3RayNormal, FLOAT32 &fTime1, FLOAT32 &fTime2 ) const;

protected:
	CFVec4 m_v4Sphere;	//XYZ are center position, w is radius.
};


} // end namespace GDE

#endif //_SPHERE_H__Included_192325287

