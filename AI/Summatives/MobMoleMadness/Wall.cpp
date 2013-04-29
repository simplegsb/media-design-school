/**
 *	@file MobMoleMadness\Wall.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "Wall.h"
#include "Box2D.h"
#include "MMMContext.h"

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
	m_Line.Initialise( v0, v1 );
	m_bHighlighted = false;
}

/**
 *	Draw the wall. Must show differently if the cat intersects it.
 */
void CWall::Draw( void )
{
	const UINT32 uColour = m_bHighlighted?0xFFFF0000:0xFF000000;
	GDE::SVertex Verts[2];
	Verts[0].m_fX = m_Line.GetEnd0().X();
	Verts[0].m_fY = m_Line.GetEnd0().Y();
	Verts[0].m_fZ = 0.5f;
	Verts[0].m_uColour = uColour;

	Verts[1].m_fX = m_Line.GetEnd1().X();
	Verts[1].m_fY = m_Line.GetEnd1().Y();
	Verts[1].m_fZ = 0.5f;
	Verts[1].m_uColour = uColour;
	
	// untextured:
	GDE::CRenderer* pRenderer = CMMMContext::GetInstance().GetRenderer();
	pRenderer->GetTextureManager().GetNULLTexture()->Apply( 0 );
	pRenderer->DrawPrimitives( GDE::PT_LineList, 1, Verts );
}


/**
 *  @see CLineSegment2D::Intersects
 */
bool CWall::Intersects( const CCircle& Circle ) const
{
	return m_Line.Intersects( Circle );
}

/**
 *	Check this wall with an infinite ray, given in parametric form and return the collision if applicable.
 *	@see CLineSegment2D::GetLineIntersectionTime
 */
bool CWall::GetLineIntersectionTime(  CFVec2Arg vRayStart, CFVec2Arg vRayOffset, FLOAT32& fT ) const
{
	return m_Line.GetLineIntersectionTime( vRayStart, vRayOffset, fT );
}

bool CWall::Intersects( const CBox2D& Box ) const
{
	return Box.OverlapsLineSegment( m_Line.GetEnd0(), m_Line.GetEnd1() );
}

