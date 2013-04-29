/**
 *	@file GDE\Maths\FVector3.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "GDE/Core/Logger.h"
#include "FVector3.h"
#include "FVector2.h"
#include "Matrix4x4.h"
#include "MathConstants.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <xnamath.h>

namespace GDE
{

const CFVec3 g_fv3ZERO = SFVec3( 0.0f, 0.0f, 0.0f );

SFVec3::operator const CFVec3 (void) const
{
	CFVec3 RV;
	RV = *this;
	return RV;
}

CFVec3 CFVec3::Init( FLOAT32 fX, FLOAT32 fY, FLOAT32 fZ )
{
	CFVec3 RV;
	XMVECTOR& V = *reinterpret_cast<XMVECTOR*>(&RV);
	V = XMVectorSet(fX, fY, fZ, 0.0f);	// note we don't care about w!
	return RV;
}

CFVec3&	CFVec3::operator=( const SFVec3 &arg )
{
	XMVECTOR& V = *reinterpret_cast<XMVECTOR*>(this);
	V = XMLoadFloat3( (const XMFLOAT3*)&arg );
	return *this;
}


/**
 *	Print out to the debug stream with no carraige return.
 */
void		CFVec3::DebugDump( void ) const
{
	DbgPrint( "%0.2f, %0.2f, %0.2f",
			X(),
			Y(),
			Z() );
}


/**
 *	Print out to the debug stream with a carraige return.
 */
void		CFVec3::DebugDumpCR( void ) const
{
	DbgPrint( "%0.2f, %0.2f, %0.2f\n",
		X(),
		Y(),
		Z() );
}


FLOAT32		CFVec3::X( void ) const
{
	const XMVECTOR& V = *reinterpret_cast<const XMVECTOR*>(this);
	return XMVectorGetX( V );
}

FLOAT32		CFVec3::Y( void ) const
{
	const XMVECTOR& V = *reinterpret_cast<const XMVECTOR*>(this);
	return XMVectorGetY( V );
}

FLOAT32		CFVec3::Z( void ) const
{
	const XMVECTOR& V = *reinterpret_cast<const XMVECTOR*>(this);
	return XMVectorGetZ( V );
}

//#ifndef _DONT_COMPILE_STUB_FVECTOR_FUNCS

void		CFVec3::X( FLOAT32 fVal )
{
	m_Vec.x = fVal;
}

void		CFVec3::Y( FLOAT32 fVal )
{
	m_Vec.y = fVal;
}

void		CFVec3::Z( FLOAT32 fVal )
{
	m_Vec.z = fVal;
}

void		CFVec3::SetXY( CFVec2Arg fv2Source )
{
	m_Vec.x = fv2Source.X();
	m_Vec.y = fv2Source.Y();
}

void		CFVec3::XAdd( FLOAT32 fVal )
{
	XMVECTOR& v3V = *reinterpret_cast<XMVECTOR*>(this);
	
	v3V = XMVectorSetX( v3V, XMVectorGetX( v3V ) + fVal );
}

void		CFVec3::YAdd( FLOAT32 fVal )
{
	XMVECTOR& v3V = *reinterpret_cast<XMVECTOR*>(this);
	
	v3V = XMVectorSetY( v3V, XMVectorGetY( v3V ) + fVal );
}

void		CFVec3::ZAdd( FLOAT32 fVal )
{
	XMVECTOR& v3V = *reinterpret_cast<XMVECTOR*>(this);
	
	v3V = XMVectorSetZ( v3V, XMVectorGetZ( v3V ) + fVal );
}

//////////////////////////////////////////////////////////
// operator overloads
bool		CFVec3::operator == (CFVec3Arg vArg ) const
{
	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v3V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );

	bool bSuccess = false;

	XMVector3Equal( v3V, v3V2 ) ? bSuccess = true : bSuccess = false;

	return bSuccess;
}

bool		CFVec3::operator != (CFVec3Arg vArg ) const
{
	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v3V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );

	bool bSuccess = false;

	XMVector3Equal( v3V, v3V2 ) ? bSuccess = false : bSuccess = true;

	return bSuccess;
}

// vector operations
CFVec3		CFVec3::operator -	( void ) const
{	
	CFVec3 v3Return;

	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );
	XMVECTOR& v3Result = *reinterpret_cast<XMVECTOR*>( &v3Return );

	//Negate
	v3Result = XMVectorNegate( v3V );

	return v3Return;
}

CFVec3		CFVec3::operator -	( CFVec3Arg vArg ) const
{
	CFVec3 v3Return;

	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v3V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );
	XMVECTOR& v3Result = *reinterpret_cast<XMVECTOR*>( &v3Return );

	v3Result = XMVectorSubtract( v3V, v3V2 );

	return v3Return;
}

CFVec3		CFVec3::operator +	( CFVec3Arg vArg ) const
{
	CFVec3 v3Return;

	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v3V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );
	XMVECTOR& v3Result = *reinterpret_cast<XMVECTOR*>( &v3Return );

	v3Result = XMVectorAdd( v3V, v3V2 );

	return v3Return;
}

CFVec3		CFVec3::operator *	( CFVec3Arg vArg ) const
{
	CFVec3 v3Return;

	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v3V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );
	XMVECTOR& v3Result = *reinterpret_cast<XMVECTOR*>( &v3Return );

	v3Result = XMVectorMultiply( v3V, v3V2 );

	return v3Return;
}

CFVec3&	CFVec3::operator +=	( CFVec3Arg vArg )
{
	XMVECTOR& v3V = *reinterpret_cast<XMVECTOR*>( this );
	const XMVECTOR& v3V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );

	v3V = XMVectorAdd( v3V, v3V2 );

	return *this;
}

CFVec3&	CFVec3::operator -=	( CFVec3Arg vArg )
{
	XMVECTOR& v3V = *reinterpret_cast<XMVECTOR*>( this );
	const XMVECTOR& v3V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );

	v3V = XMVectorSubtract( v3V, v3V2 );

	return *this;
}

CFVec3&	CFVec3::operator *=	( CFVec3Arg vArg )
{
	XMVECTOR& v3V = *reinterpret_cast<XMVECTOR*>( this );
	const XMVECTOR& v3V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );

	v3V = XMVectorMultiply( v3V, v3V2 );

	return *this;
}

// scalar opertaors
CFVec3&	CFVec3::operator *=	( const FLOAT32 fArg )
{
	XMVECTOR v3Scaler;
	v3Scaler = XMVectorSet( fArg, fArg, fArg, 1.0f );

	XMVECTOR& v3V = *reinterpret_cast<XMVECTOR*>( this );

	v3V = XMVectorMultiply( v3V, v3Scaler );

	return *this;
}

CFVec3		CFVec3::operator *	( const FLOAT32 fArg ) const
{
	CFVec3 v3Return;

	XMVECTOR v3Scaler;
	v3Scaler = XMVectorSet( fArg, fArg, fArg, 1.0f );

	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );
	XMVECTOR& v3Result = *reinterpret_cast<XMVECTOR*>( &v3Return );

	v3Result = XMVectorMultiply( v3V, v3Scaler );

	return v3Return;
}

CFVec3&	CFVec3::operator /=	( const FLOAT32 fArg )
{
	FLOAT32 fDivisor = 1.0f / fArg;

	XMVECTOR v3Scaler;
	v3Scaler = XMVectorSet( fDivisor, fDivisor, fDivisor, 1.0f );

	XMVECTOR& v3V = *reinterpret_cast<XMVECTOR*>( this );

	v3V = XMVectorMultiply( v3V, v3Scaler );

	return *this;
}

CFVec3		CFVec3::operator /	( const FLOAT32 fArg ) const
{
	CFVec3 v3Return;

	FLOAT32 fDivisor = 1.0f / fArg;

	XMVECTOR v3Scaler;
	v3Scaler = XMVectorSet( fDivisor, fDivisor, fDivisor, 1.0f );

	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );
	XMVECTOR& v3Result = *reinterpret_cast<XMVECTOR*>( &v3Return );

	v3Result = XMVectorMultiply( v3V, v3Scaler );

	return v3Return;
}

CFVec3		CFVec3::CrossProduct( CFVec3Arg vArg ) const
{
	CFVec3 v3Return;

	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v3V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );
	XMVECTOR& v3Result = *reinterpret_cast<XMVECTOR*>( &v3Return );

	v3Result = XMVector3Cross( v3V, v3V2 );

	return v3Return;
}

FLOAT32		CFVec3::DotProduct( CFVec3Arg vArg ) const
{
	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v3V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );

	return XMVectorGetX( XMVector3Dot( v3V, v3V2 ) );
}

/**
 *	@return A vector quantity with the scalar in each component.
 */
CFVec3		CFVec3::DotProductV( CFVec3Arg vArg ) const
{
	CFVec3 v3Return;

	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v3V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );
	XMVECTOR& v3Result = *reinterpret_cast<XMVECTOR*>( &v3Return );

	v3Result = XMVector3Dot( v3V, v3V2 );

	return v3Return;
}

FLOAT32		CFVec3::SquareDistance( CFVec3Arg fv3To ) const
{
	CFVec3 v3Temp;
	v3Temp = fv3To - *this;

	XMVECTOR& v3V = *reinterpret_cast<XMVECTOR*>( &v3Temp );

	return XMVectorGetX( XMVector3LengthSq( v3V ) );
}

/**
 *	@return A vector quantity with the scalar in each component.
 */
CFVec3		CFVec3::SquareDistanceV( CFVec3Arg fv3To ) const
{
	CFVec3 v3Return;
	v3Return = fv3To - *this;

	XMVECTOR& v3V = *reinterpret_cast<XMVECTOR*>( &v3Return );

	v3V = XMVector3LengthSq( v3V );

	return v3Return;
}


FLOAT32		CFVec3::Distance( CFVec3Arg fv3To ) const
{
	CFVec3 v3Temp;
	v3Temp = fv3To - *this;

	XMVECTOR& v3V = *reinterpret_cast<XMVECTOR*>( &v3Temp );

	return XMVectorGetX( XMVector3Length( v3V ) );
}

/**
 *	@return A vector quantity with the scalar in each component.
 */
CFVec3		CFVec3::DistanceV( CFVec3Arg fv3To ) const
{
	CFVec3 v3Temp;
	v3Temp = fv3To - *this;

	XMVECTOR& v3V = *reinterpret_cast<XMVECTOR*>( &v3Temp );

	v3V = XMVector3Length( v3V );

	return v3Temp;
}

FLOAT32		CFVec3::SquareMagnitude( void ) const
{
	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );

	return XMVectorGetX( XMVector3LengthSq( v3V ) );
}

/**
 *	@return A vector quantity with the scalar in each component.
 */
CFVec3		CFVec3::SquareMagnitudeV( void ) const
{
	CFVec3 v3Return;

	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );
	XMVECTOR& v3Result = *reinterpret_cast<XMVECTOR*>( &v3Return );

	v3Result = XMVector3LengthSq( v3V );

	return v3Return;
}

FLOAT32		CFVec3::Magnitude( void ) const
{
	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );

	return XMVectorGetX( XMVector3Length( v3V ) );
}

/**
 *	@return A vector quantity with the scalar in each component.
 */
CFVec3		CFVec3::MagnitudeV( void ) const
{
	CFVec3 v3Return;

	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );
	XMVECTOR& v3Result = *reinterpret_cast<XMVECTOR*>( &v3Return );

	v3Result = XMVector3Length( v3V );

	return v3Return;
}

void	CFVec3::Normalise( void )
{
	XMVECTOR& v3V = *reinterpret_cast<XMVECTOR*>( this );

	v3V = XMVector3Normalize( v3V );
}

void	CFVec3::Normalise( FLOAT32 &fMagnitude )
{
	XMVECTOR& v3V = *reinterpret_cast<XMVECTOR*>( this );

	//TODO: Is there a cheaper way of doing this?
	fMagnitude = XMVectorGetX( XMVector3Length( v3V ) );

	v3V = XMVector3Normalize( v3V );
}

/**
 *	@param vMagnitude	Contains the scalar magnitude in each component.
 */
void	CFVec3::Normalise( CFVec3 &vMagnitude )
{
	XMVECTOR& v3V = *reinterpret_cast<XMVECTOR*>( this );
	XMVECTOR& v3VMag = *reinterpret_cast<XMVECTOR*>( &vMagnitude );

	//TODO: Is there a cheaper way of doing this?
	v3VMag = XMVector3Length( v3V );

	v3V = XMVector3Normalize( v3V );
}


CFVec3		CFVec3::GetNormal( void ) const
{
	CFVec3 v3Return;

	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );
	XMVECTOR& v3Result = *reinterpret_cast<XMVECTOR*>( &v3Return );

	v3Result = XMVector3Normalize( v3V );

	return v3Return;
}

CFVec3		CFVec3::GetNormal( FLOAT32 &fMagnitude ) const
{
	CFVec3 v3Return;

	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );
	XMVECTOR& v3Result = *reinterpret_cast<XMVECTOR*>( &v3Return );

	//TODO: Is there a cheaper way of doing this?
	fMagnitude = XMVectorGetX( XMVector3Length( v3V ) );

	v3Result = XMVector3Normalize( v3V );

	return v3Return;
}

/**
 *	@param vMagnitude	Contains the scalar magnitude in each component.
 */
CFVec3		CFVec3::GetNormal( CFVec3 &vMagnitude ) const
{
	CFVec3 v3Return;

	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );
	XMVECTOR& v3VMag = *reinterpret_cast<XMVECTOR*>( &vMagnitude );
	XMVECTOR& v3Result = *reinterpret_cast<XMVECTOR*>( &v3Return );

	//TODO: Is there a cheaper way of doing this?
	v3VMag = XMVector3Length( v3V );

	v3Result = XMVector3Normalize( v3V );

	return v3Return;
}


bool		CFVec3::IsNormal( void ) const
{
	//const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );

	//TODO: Cheaper way of doing this?
	//Getting squared length is cheaper, and 1^2 is still 1.
	FLOAT32 fMag = SquareMagnitude();//= XMVectorGetX( XMVector3LengthSq( v3V ) );

	bool bSuccess = false;

	//The GDE::fEPSILON value has too much precision for this..
	FLOAT32 fEpsilon = 0.0000001f;

	fabsf( fMag - 1.0f ) <= fEpsilon ? bSuccess = true : bSuccess = false;

	return bSuccess;
}

void		CFVec3::RotateX( FLOAT32 fRadians )
{
	CFMat4x4 matRotation;
	matRotation = CFMat4x4::CreateRotationX( fRadians );

	*this = matRotation.TransformVec3( *this );
}

void		CFVec3::RotateY( FLOAT32 fRadians )
{
	CFMat4x4 matRotation;
	matRotation = CFMat4x4::CreateRotationY( fRadians );

	*this = matRotation.TransformVec3( *this );
}

void		CFVec3::RotateZ( FLOAT32 fRadians )
{
	CFMat4x4 matRotation;
	matRotation = CFMat4x4::CreateRotationZ( fRadians );

	*this = matRotation.TransformVec3( *this );
}

//void		CFVec3::RotateX( FLOAT32 fRadians )
//{
//	const FLOAT32 fSin = sinf( -fRadians );
//	const FLOAT32 fCos = cosf( -fRadians );
//	const FLOAT32 fY = fCos*m_Vec.y + fSin*m_Vec.z;
//	m_Vec.z = -fSin*m_Vec.y + fCos*m_Vec.z;
//	m_Vec.y = fY;
//}
//
//void		CFVec3::RotateY( FLOAT32 fRadians )
//{
//	const FLOAT32 fSin = sinf( -fRadians );
//	const FLOAT32 fCos = cosf( -fRadians );
//	const FLOAT32 fX = fCos*m_Vec.x + fSin*m_Vec.z;
//	m_Vec.z = -fSin*m_Vec.x + fCos*m_Vec.z;
//	m_Vec.x = fX;
//}
//
//void		CFVec3::RotateZ( FLOAT32 fRadians )
//{
//	const FLOAT32 fSin = sinf( -fRadians );
//	const FLOAT32 fCos = cosf( -fRadians );
//	const FLOAT32 fX = fCos*m_Vec.x + fSin*m_Vec.y;
//	m_Vec.y = -fSin*m_Vec.x + fCos*m_Vec.y;
//	m_Vec.x = fX;
//}

/**
 *
 *	Rotate vector using pitch, yaw and roll
 *	Roll is applied first, then pitch, then yaw.
 *	Angles are measured clockwise when looking along the rotation axis toward the origin.
 *
 *	@param fPitch Pitch in radians.
 *	@param fYaw Yaw in radians.
 *	@param fRoll Roll in radians.
 */
void CFVec3::Rotate(FLOAT32 fPitch, FLOAT32 fYaw, FLOAT32 fRoll)
{
	XMVECTOR& rThis = *reinterpret_cast<XMVECTOR*>(this);
	XMVECTOR qRotation = XMQuaternionRotationRollPitchYaw(fPitch, fYaw, fRoll);
	rThis = XMVector3Rotate(rThis, qRotation);
}

/**
 *	If a direction vector - converting to Euler without roll, assuming Z is up.
 */
FLOAT32		CFVec3::GetHeading( void ) const
{
	//atan2, return angle - ignore roll.
	//X and Z
#pragma warnmsg( "stub function that needs implementation" )
	assert_now( "stub function" );
	return 0.0f;
}

FLOAT32		CFVec3::GetElevation( void ) const
{
#pragma warnmsg( "stub function that needs implementation" )
	assert_now( "stub function" );
	return 0.0f;
}

/**
 *	Component wise set to minimum of this and other.
 */
CFVec3&		CFVec3::MinimizeWith( CFVec3Arg fv3Other )
{
	XMVECTOR& v3V = *reinterpret_cast<XMVECTOR*>( this );
	const XMVECTOR& v3V2 = *reinterpret_cast<const XMVECTOR*>( &fv3Other );

	v3V = XMVectorMin( v3V, v3V2 );
	
	return *this;
}

CFVec3		CFVec3::GetMin( CFVec3Arg fv3Other ) const
{
	CFVec3 v3Return;

	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v3V2 = *reinterpret_cast<const XMVECTOR*>( &fv3Other );
	XMVECTOR& v3Result = *reinterpret_cast<XMVECTOR*>( &v3Return );

	v3Result = XMVectorMin( v3V, v3V2 );

	return v3Return;
}

/**
 *	Component wise set to maximum of this and other.
 */
CFVec3&		CFVec3::MaximizeWith( CFVec3Arg fv3Other )
{
	XMVECTOR& v3V = *reinterpret_cast<XMVECTOR*>( this );
	const XMVECTOR& v3V2 = *reinterpret_cast<const XMVECTOR*>( &fv3Other );

	v3V = XMVectorMax( v3V, v3V2 );
	
	return *this;
}

CFVec3		CFVec3::GetMax( CFVec3Arg fv3Other ) const
{
	CFVec3 v3Return;

	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v3V2 = *reinterpret_cast<const XMVECTOR*>( &fv3Other );
	XMVECTOR& v3Result = *reinterpret_cast<XMVECTOR*>( &v3Return );

	v3Result = XMVectorMax( v3V, v3V2 );

	return v3Return;
}

/**
 *  Linearly interpolate two vectors.
 * 
 *  @param vTarget The destination vector.
 *  @param fT The control time.
 *  @return The resulting vector.
 */
CFVec3		CFVec3::Lerp( CFVec3Arg vTarget, GDE::FLOAT32 fT )
{
	CFVec3 v3Return;

	const XMVECTOR& v3VA = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v3VB = *reinterpret_cast<const XMVECTOR*>( &vTarget );
	XMVECTOR& v3Result = *reinterpret_cast<XMVECTOR*>( &v3Return );	

	v3Result = XMVectorLerp( v3VA, v3VB, fT );

	return v3Return;
}

} // end namespace GDE
