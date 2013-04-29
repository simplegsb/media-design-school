/**
 *	@file GDE\Maths\AABB.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _AABB_H__Included_1115948706
#define _AABB_H__Included_1115948706

#include "GDE/Maths/FVector3.h"

namespace GDE
{

/**
 *	@ingroup Maths
 *
 *	An axis aligned bounding box.
 */
class CAABB
{
public:
	void InitForPoints( const CFVec3* pPoints, UINT32 uNumPoints );
	void InitForCentreExtents( CFVec3Arg vCentre, CFVec3Arg vExtents );
	void InitForMinMax( CFVec3Arg vMin, CFVec3Arg vMax );
	void GetCorners( CFVec3* pCorners ) const;
protected:
};

} // end namespace GDE.

#endif //_AABB_H__Included_1115948706

