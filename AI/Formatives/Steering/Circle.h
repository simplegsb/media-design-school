/**
 *	@file AI_Steering\Circle.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _CIRCLE_H__Included_1340580524
#define _CIRCLE_H__Included_1340580524

#include "FVector2.h"

/**
 *	A two dimensional circle.
 */
class CCircle
{
public:
	// no explicit constructor, instead initialise.
	void		Initialise( CFVec2Arg vCentre, FLOAT32 fRadius );
	bool		Overlaps( const CCircle& Other ) const;
	bool		OverlapsLineSegment( CFVec2Arg v0, CFVec2Arg v1 ) const;
	UINT32		GetLineIntersectionTimes( CFVec2Arg vRayStart, CFVec2Arg vRayOffset, FLOAT32& fT0, FLOAT32& fT1 ) const;
	FLOAT32		Radius( void ) const { return m_fRadius; }
	CFVec2Arg	Centre( void ) const { return m_vCentre; }
protected:
	CFVec2	m_vCentre;	//!< The circle's centre.
	FLOAT32	m_fRadius;	//!< The circle's radius.
};

#endif //_CIRCLE_H__Included_1340580524

