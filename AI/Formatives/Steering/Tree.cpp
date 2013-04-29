/**
 *	@file AI_Steering\Tree.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "Tree.h"
#include "SteeringContext.h"
#include "Circle.h"

void CTree::Initialise( CFVec2Arg vPosition, FLOAT32 fRadius )
{
	m_vPos = vPosition;
	m_fRadius = fRadius;
	m_bIntersected = false;
}

/**
 *	Draw the tree trunk. Must show differently if the cat intersects it.
 */
void CTree::DrawTrunk( void )
{
	const UINT32 uColour = m_bIntersected?0xFFFF0000:0xFF7F7F00;
	CCircle Circle;
	Circle.Initialise( m_vPos, m_fRadius );
	CSteeringContext::GetInstance().DrawFilledCircle( Circle, uColour );
}

/**
 *	Draw the tree canopy.
 */
void CTree::DrawCanopy( void )
{
	CCircle Circle;
	Circle.Initialise( m_vPos, m_fRadius*7.0f );
	CSteeringContext::GetInstance().DrawFilledCircle( Circle, 0x7F3FFF3F );	
}


/**
 *	@param Circle A circle to evaluate to see if it intersects the trunk.
 *	@return true if the circle intersects the trunk.
 */
bool CTree::Intersects( const CCircle& Circle ) const
{
	CCircle Trunk;
	Trunk.Initialise( m_vPos, m_fRadius );
	return Circle.Overlaps( Trunk );
}

/**
 *	Reset the state for intersection with the cat - done at the frame start.
 *	@see SetIntersectedByCat
 */
void CTree::ResetIntersection( void )
{
	m_bIntersected = false;
}
/**
 *	Set the trunk to be intersected by the cat.
 */
void CTree::SetIntersectedByCat( void )
{
	m_bIntersected = true;
}
