/**
 *	@file AI_Steering\Wall.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _WALL_H__Included_1049460091
#define _WALL_H__Included_1049460091

#include "FVector2.h"

class CCircle;

/**
 *	This class represents a wall in the world representation.
 */
class CWall
{
public:
	// no need for constructor, initialised from a list.
	void Initialise( CFVec2Arg v0, CFVec2Arg v1 );
	void Draw( void );
	bool Intersects( const CCircle& Circle ) const;

	bool GetLineIntersectionTime( CFVec2Arg vRayStart, CFVec2Arg vRayOffset, FLOAT32& fT ) const;

	void ResetIntersection( void );
	void SetIntersectedByCat( void );
protected:
	CFVec2	m_vEnd[2];			//!< The end points of the wall's line.
	bool	m_bIntersected;		//!< Does this wall currently intersect the cat? If so then it must draw highlighted somehow.
};

#endif //_WALL_H__Included_1049460091

