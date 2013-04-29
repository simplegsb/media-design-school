/**
 *	@file AI_Steering\Wall.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "Wall.h"
#include "SteeringContext.h"
#include "Circle.h"

#include "GDE/Renderer/Renderer.h"
#include "GDE/Renderer/Vertex.h"
#include "GDE/Renderer/TextureManager.h"
#include "GDE/Renderer/Texture.h"

/**
 *	Ininitialise the wall.
 *	@param v0	One end of the wall.
 *	@param v1	The other end of the wall.
 */
void CWall::Initialise( CFVec2Arg v0, CFVec2Arg v1 )
{
	m_vEnd[0] = v0;
	m_vEnd[1] = v1;
	m_bIntersected = false;
}

/**
 *	Draw the wall. Must show differently if the cat intersects it.
 */
void CWall::Draw( void )
{
	const UINT32 uColour = m_bIntersected?0xFFFF0000:0xFF000000;
	GDE::SVertex Verts[2];
	Verts[0].m_fX = m_vEnd[0].X();
	Verts[0].m_fY = m_vEnd[0].Y();
	Verts[0].m_fZ = 0.5f;
	Verts[0].m_uColour = uColour;

	Verts[1].m_fX = m_vEnd[1].X();
	Verts[1].m_fY = m_vEnd[1].Y();
	Verts[1].m_fZ = 0.5f;
	Verts[1].m_uColour = uColour;
	
	// untextured:
	GDE::CRenderer* pRenderer = CSteeringContext::GetInstance().GetRenderer();
	pRenderer->GetTextureManager().GetNULLTexture()->Apply( 0 );
	pRenderer->DrawPrimitives( GDE::PT_LineList, 1, Verts );
}

/**
 *	@param Circle A circle to evaluate to see if it intersects the wall.
 *	@return true if the circle intersects the wall.
 */
bool CWall::Intersects( const CCircle& Circle ) const
{
	return Circle.OverlapsLineSegment( m_vEnd[0], m_vEnd[1] );
}

/**
 *	Reset the state for intersection with the cat - done at the frame start.
 *	@see SetIntersectedByCat
 */
void CWall::ResetIntersection( void )
{
	m_bIntersected = false;
}
/**
 *	Set the wall to be intersected by the cat.
 */
void CWall::SetIntersectedByCat( void )
{
	m_bIntersected = true;
}

/**
 *	Check this wall with an infinite ray, given in parametric form and return the collision if applicable.
 *	@param vRayStart	Start point for parametric equation of infinite ray.
 *	@param vRayEnd		Direction vector (coefficient of t) for paremetric equation of infinitie ray.
 *	@param fT			To be filled in with the time (paremetric T) of intersection.
 *	@return				True if the intersection occurs within this line segment.
 */
bool CWall::GetLineIntersectionTime(  CFVec2Arg vRayStart, CFVec2Arg vRayOffset, FLOAT32& fT ) const
{
	// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
	const CFVec2& P1 = m_vEnd[0];
	const CFVec2& P2 = m_vEnd[1];
	const CFVec2& P3 = vRayStart;
	// no P4 required..
	const CFVec2 V1 = P2-P1;
	const CFVec2& V2 = vRayOffset;
	
	const FLOAT32 fDenom = V2.Y()*V1.X() - V2.X()*V1.Y();
	
	if ( fDenom == 0.0f )
	{
		return false; // no intersections, or the lines are the same!
	}

	const FLOAT32 fDRecip = 1.0f/fDenom;
	// and then, finally Q (for x/y1 - x/y3)
	const CFVec2 Q = P1-P3;

	const FLOAT32 fTimeThis = (V2.X()*Q.Y() - V2.Y()*Q.X())*fDRecip;

	if ( fTimeThis < 0.0f || fTimeThis > 1.0f )
	{
		// outside of the wall line segment.
		return false;
	}
	fT = (V1.X()*Q.Y() - V1.Y()*Q.X())*fDRecip;
	return true;
}
