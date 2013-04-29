/**
 *	@file MobMoleMadness\Wall.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _WALL_H__Included_1049460091
#define _WALL_H__Included_1049460091

#include "LineSegment2D.h"

class CBox2D;

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
	bool Intersects( const CBox2D& Box ) const;

	bool GetLineIntersectionTime( CFVec2Arg vRayStart, CFVec2Arg vRayOffset, FLOAT32& fT ) const;

	void SetHighlighted( bool bHighlighted ) { m_bHighlighted = bHighlighted; }
	bool IsHighlighted( void ) const { return m_bHighlighted; }

	CFVec2Arg	GetEnd0( void ) const { return m_Line.GetEnd0(); }
	CFVec2Arg	GetEnd1( void ) const { return m_Line.GetEnd1(); }
protected:
	CLineSegment2D	m_Line;
	bool			m_bHighlighted;		//!< Is this wall currently highlighted;
};

#endif //_WALL_H__Included_1049460091

