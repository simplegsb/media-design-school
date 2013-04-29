/**
 *	@file GDE\Maths\Box2D.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "Box2D.h"
#include "Circle.h"
#include "LineSegment2D.h"

using namespace GDE;

/**
 *	@param vCentre	Centre of the new box.
 *	@param vExtents	Extents of the new box.
 */
bool	CBox2D::InitExtents( CFVec2Arg vCentre, CFVec2Arg vExtents )
{
	m_vCentre = vCentre;
	m_vExtents = vExtents;
	return true;
}

/**
 *	@param v0	One corner of the new box.
 *	@param v1	The diagonal opposite corner of the new box.
 */
bool	CBox2D::InitCorners( CFVec2Arg v0, CFVec2Arg v1 )
{
	m_vCentre = (v0+v1)*0.5f;
	m_vExtents = v1-m_vCentre;
	m_vExtents.SetAbs();
	return true;
}

bool	CBox2D::Intersects( const CCircle& Circle ) const
{
	CFVec2 vOffset = (Circle.Centre() - m_vCentre);
	vOffset.SetAbs();
	vOffset -= m_vExtents;
	return ( vOffset.X() < Circle.Radius() && vOffset.Y() < Circle.Radius() );
}

bool	CBox2D::Intersects( const CBox2D& Box ) const
{
	CFVec2 vOffset = (Box.m_vCentre - m_vCentre);
	vOffset.SetAbs();
	vOffset -= m_vExtents;
	vOffset -= Box.m_vExtents;
	return ( vOffset.X() < 0.0f && vOffset.Y() < 0.0f );

}

bool	CBox2D::OverlapsLineSegment( CFVec2Arg v0, CFVec2Arg v1 ) const
{
	if ( ContainsPoint( v0 ) || ContainsPoint( v1 ) )
	{
		return true;
	}

	CLineSegment2D Seg;
	Seg.Initialise( v0, v1 );
	CLineSegment2D Edge;
	CFVec2 vCorners[5];
	vCorners[0] = m_vCentre-m_vExtents;
	vCorners[4] = vCorners[0];
	vCorners[1].X( m_vCentre.X() + m_vExtents.X() );
	vCorners[1].Y( m_vCentre.Y() - m_vExtents.Y() );
	vCorners[2] = m_vCentre+m_vExtents;
	vCorners[3].X( m_vCentre.X() - m_vExtents.X() );
	vCorners[3].Y( m_vCentre.Y() + m_vExtents.Y() );
	for ( UINT32 i = 0; i<4; i++ )
	{
		Edge.Initialise( vCorners[i], vCorners[i+1] );
		if ( Edge.Intersects( Seg ) )
		{
			return true;
		}
	}
	return false;
}

bool	CBox2D::ContainsPoint( CFVec2Arg vPoint ) const
{
	CFVec2 vOffset = (vPoint - m_vCentre);
	vOffset.SetAbs();
	vOffset -= m_vExtents;
	return ( vOffset.X() < 0.0f && vOffset.Y() < 0.0f );

}


