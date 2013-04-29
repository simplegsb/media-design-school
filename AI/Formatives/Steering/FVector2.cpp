/**
 *	@file QuadTree\FVector2.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "FVector2.h"
#include <math.h>

FLOAT32 CFVec2::Magnitude( void ) const
{
	return sqrtf( SquareMagnitude() );
}

bool CFVec2::IsNormal( void ) const
{
	return( fabsf(SquareMagnitude() - 1.0f) <= 0.0001f );
}

/**
 *	@param fRadians Amount to rotate clockwise, in radians.
 */
CFVec2Arg CFVec2::RotateZ( FLOAT32 fRadians )
{
	const FLOAT32 fSin = sinf( -fRadians );
	const FLOAT32 fCos = cosf( -fRadians );
	const FLOAT32 fX = fCos*m_Vec.x + fSin*m_Vec.y;
	m_Vec.y = -fSin*m_Vec.x + fCos*m_Vec.y;
	m_Vec.x = fX;
	return *this;
}

/**
 *	Set this vector to the clockwise rotation (from straight up) around Z.
 *	@param fRadians Amount to rotate clockwise, in radians from straight up.
 */
void CFVec2::SetToRotationZ( FLOAT32 fRadians )
{
	const FLOAT32 fSin = sinf( fPI-fRadians );
	const FLOAT32 fCos = cosf( fPI-fRadians );
	m_Vec.x = fSin;
	m_Vec.y = fCos;
}
