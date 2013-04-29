/**
 *	@file GDE\Maths\FVector2.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "GDE/Core/Logger.h"
#include "FVector2.h"
#include "MathConstants.h"

#include <math.h>

using namespace GDE;

void CFVec2::X( FLOAT32 v )
{
	m_Vec.x = v;
}
void CFVec2::Y( FLOAT32 v )
{
	m_Vec.y = v;
}

FLOAT32 CFVec2::X( void ) const
{
	return m_Vec.x;
}
FLOAT32 CFVec2::Y( void ) const
{
	return m_Vec.y;
}

void CFVec2::XAdd( FLOAT32 v )
{
	m_Vec.x += v;
}

void CFVec2::YAdd( FLOAT32 v )
{
	m_Vec.y += v;
}

//////////////////////////////////////////////////////////
// operator overloads
bool CFVec2::operator == (CFVec2Arg arg ) const
{
	return X() == arg.X() && Y() == arg.Y();
}

bool CFVec2::operator != (CFVec2Arg arg ) const
{
	return X() != arg.X() || Y() != arg.Y();
}

CFVec2 CFVec2::operator-( CFVec2Arg arg ) const
{
	//TODO: optimise with using the vector unit maths
	CFVec2 RetVal;
	RetVal.X( X() - arg.X() );
	RetVal.Y( Y() - arg.Y() );
	return RetVal;
}

// negate
CFVec2 CFVec2::operator-( void ) const
{
	//TODO: optimise with using the vector unit maths
	CFVec2 RetVal;
	RetVal.X( -X() );
	RetVal.Y( -Y() );
	return RetVal;
}


CFVec2 CFVec2::operator+( CFVec2Arg arg ) const
{
	//TODO: optimise with using the vector unit maths
	CFVec2 RetVal;
	RetVal.X( X() + arg.X() );
	RetVal.Y( Y() + arg.Y() );
	return RetVal;
}


CFVec2Arg CFVec2::operator+=( CFVec2Arg arg )
{
	//TODO: optimise with using the vector unit maths
	m_Vec.x += arg.X();
	m_Vec.y += arg.Y();
	return *this;
}

CFVec2Arg CFVec2::operator-=( CFVec2Arg arg )
{
	//TODO: optimise with using the vector unit maths
	m_Vec.x -= arg.X();
	m_Vec.y -= arg.Y();
	return *this;
}

CFVec2Arg CFVec2::operator*=( const FLOAT32 fArg )
{
	//TODO: optimise with using the vector unit maths
	m_Vec.x *= fArg;
	m_Vec.y *= fArg;
	return *this;
}

CFVec2 CFVec2::operator*( const FLOAT32 fArg ) const
{
	//TODO: optimise with using the vector unit maths
	CFVec2 RetVal;
	RetVal.X( X() * fArg );
	RetVal.Y( Y() * fArg );
	return RetVal;
}

CFVec2Arg CFVec2::operator/=( const FLOAT32 fArg )
{
	//TODO: optimise with using the vector unit maths
	assert( fArg != 0.0f );
	return ((*this)*=(1.0f/fArg));
}

CFVec2 CFVec2::operator/( const FLOAT32 fArg ) const
{
	//TODO: optimise with using the vector unit maths
	assert( fArg != 0.0f );
	return (*this) * (1.0f/fArg);
}

CFVec2Arg CFVec2::operator*=( CFVec2Arg arg )
{
	//TODO: optimise with using the vector unit maths
	m_Vec.x *= arg.m_Vec.x;
	m_Vec.y *= arg.m_Vec.y;
	return *this;
}

FLOAT32 CFVec2::SquareMagnitude( void ) const
{
	return (X()*X()+Y()*Y());
}

FLOAT32 CFVec2::SquareDistance( CFVec2Arg fv2To ) const
{
	return ((*this)-fv2To).SquareMagnitude();
}

CFVec2Arg CFVec2::Normalise( void )
{
	// TODO: optimise
//	(*this) /= ( Magnitude() );
//	return *this;
	assert( this != NULL );
	const FLOAT32 Mag = Magnitude();
	assert( Mag != 0.0f );

	const FLOAT32 fMult = 1.0f/Mag;

	X( X() * fMult );
	Y( Y() * fMult );
	return (*this);
}

CFVec2Arg CFVec2::Normalise( FLOAT32 &fMagnitude )
{
	// TODO: optimise
//	(*this) /= ( Magnitude() );
//	return *this;
	assert( this != NULL );
	fMagnitude = Magnitude();
	assert( fMagnitude != 0.0f );

	const FLOAT32 fMult = 1.0f/fMagnitude;

	X( X() * fMult );
	Y( Y() * fMult );
	return (*this);
}

CFVec2Arg CFVec2::SetPerpendicular( void )
{
	const FLOAT32& t = X();
	X( Y() );
	Y( -t );
	return (*this);
}

CFVec2 CFVec2::GetNormal( void ) const
{
	CFVec2 RetVal = *this;
	return RetVal.Normalise();
}

CFVec2 CFVec2::GetNormal( FLOAT32 &fMagnitude ) const
{
	CFVec2 RetVal = *this;
	return RetVal.Normalise( fMagnitude );
}


FLOAT32 CFVec2::DotProduct( CFVec2Arg arg ) const
{
	// u . v = |u||v|*cos( angle between vectors )
	// TODO: optimise
	FLOAT32 RetVal = (X() * arg.X()) + (Y() * arg.Y());
	return RetVal;
}

/**
 *	Set this value to be the absolute in both x and y (unsigned).
 */
void CFVec2::SetAbs( void )
{
	m_Vec.x = fabsf( m_Vec.x );
	m_Vec.y = fabsf( m_Vec.y );
}


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

/**
 *	Print out to the debug stream.
 */
void CFVec2::DebugDump( void ) const
{
	DbgPrint( "%0.2f, %0.2f",
		X(),
		Y() );
}

/**
 *	Print out to the debug stream with a carraige return.
 */
void CFVec2::DebugDumpCR( void ) const
{
	DbgPrint( "%0.2f, %0.2f\n",
		X(),
		Y() );
}
