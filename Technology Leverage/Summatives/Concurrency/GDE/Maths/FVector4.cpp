/**
 *	@file GDE\Maths\FVector4.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "GDE/Core/Logger.h"
#include "FVector4.h"
#include "FVector3.h"
#include "FVector2.h"
#include "Matrix4x4.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <xnamath.h>

namespace GDE
{

const CFVec4 g_fv4ZERO = SFVec4( 0.0f, 0.0f, 0.0f, 0.0f );

SFVec4::operator const CFVec4 (void) const
{
	CFVec4 RV;
	RV = *this;
	return RV;
}

CFVec4 CFVec4::Init( FLOAT32 fX, FLOAT32 fY, FLOAT32 fZ, FLOAT32 fW )
{
	CFVec4 RV;
	XMVECTOR& V = *reinterpret_cast<XMVECTOR*>(&RV);
	V = XMVectorSet(fX, fY, fZ, fW);
	return RV;
}

CFVec4&	CFVec4::operator=( const SFVec4 &arg )
{
	XMVECTOR& V = *reinterpret_cast<XMVECTOR*>(this);
	V = XMLoadFloat4( (const XMFLOAT4*)&arg );
	return *this;
}


/**
 *	Print out to the debug stream with no carraige return.
 */
void		CFVec4::DebugDump( void ) const
{
	DbgPrint( "%0.2f, %0.2f, %0.2f %0.2f",
			X(),
			Y(),
			Z(),
			W() );
}


/**
 *	Print out to the debug stream with a carraige return.
 */
void		CFVec4::DebugDumpCR( void ) const
{
	DbgPrint( "%0.2f, %0.2f, %0.2f\n",
		X(),
		Y(),
		Z(),
		W() );
}

void		CFVec4::X( FLOAT32 fVal )
{
	XMVECTOR& V4 = *reinterpret_cast<XMVECTOR*>(this);
	V4 = XMVectorSetX( V4, fVal );
}

void		CFVec4::Y( FLOAT32 fVal )
{
	XMVECTOR& V4 = *reinterpret_cast<XMVECTOR*>(this);
	V4 = XMVectorSetY( V4, fVal );
}

void		CFVec4::Z( FLOAT32 fVal )
{
	XMVECTOR& V4 = *reinterpret_cast<XMVECTOR*>(this);
	V4 = XMVectorSetZ( V4, fVal );
}

void		CFVec4::W( FLOAT32 fVal )
{
	XMVECTOR& V4 = *reinterpret_cast<XMVECTOR*>(this);
	V4 = XMVectorSetW( V4, fVal );
}


FLOAT32		CFVec4::X( void ) const
{
	const XMVECTOR& V4 = *reinterpret_cast<const XMVECTOR*>(this);
	return XMVectorGetX( V4 );
}

FLOAT32		CFVec4::Y( void ) const
{
	const XMVECTOR& V4 = *reinterpret_cast<const XMVECTOR*>(this);
	return XMVectorGetY( V4 );
}

FLOAT32		CFVec4::Z( void ) const
{
	const XMVECTOR& V4 = *reinterpret_cast<const XMVECTOR*>(this);
	return XMVectorGetZ( V4 );
}

FLOAT32		CFVec4::W( void ) const
{
	const XMVECTOR& V4 = *reinterpret_cast<const XMVECTOR*>(this);
	return XMVectorGetW( V4 );
}

void		CFVec4::SetXY( CFVec2Arg fv2Source )
{
	const XMVECTOR& v2V = *reinterpret_cast<const XMVECTOR*>( &fv2Source );
	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>(this);

	v4V = XMVectorSetX( v4V, XMVectorGetX( v2V ) );
	v4V = XMVectorSetY( v4V, XMVectorGetY( v2V ) );
}

void		CFVec4::SetXYZ( CFVec3Arg fv3Source )
{
	const XMVECTOR& v3V = *reinterpret_cast<const XMVECTOR*>( &fv3Source );
	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>(this);

	v4V = XMVectorSetX( v4V, XMVectorGetX( v3V ) );
	v4V = XMVectorSetY( v4V, XMVectorGetY( v3V ) );
	v4V = XMVectorSetZ( v4V, XMVectorGetZ( v3V ) );
}


void		CFVec4::XAdd( FLOAT32 fVal )
{
	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>(this);
	
	v4V = XMVectorSetX( v4V, XMVectorGetX( v4V ) + fVal );
}

void		CFVec4::YAdd( FLOAT32 fVal )
{
	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>(this);
	
	v4V = XMVectorSetY( v4V, XMVectorGetY( v4V ) + fVal );
}

void		CFVec4::ZAdd( FLOAT32 fVal )
{
	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>(this);
	
	v4V = XMVectorSetZ( v4V, XMVectorGetZ( v4V ) + fVal );
}

void		CFVec4::WAdd( FLOAT32 fVal )
{
	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>(this);
	
	v4V = XMVectorSetW( v4V, XMVectorGetW( v4V ) + fVal );
}

//////////////////////////////////////////////////////////
// operator overloads
bool		CFVec4::operator == (CFVec4Arg vArg ) const
{
	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v4V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );

	bool bSuccess = false;

	XMVector4Equal( v4V, v4V2 ) ? bSuccess = true : bSuccess = false;

	return bSuccess;
}

bool		CFVec4::operator != (CFVec4Arg vArg ) const
{
	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v4V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );

	bool bSuccess = false;

	XMVector4Equal( v4V, v4V2 ) ? bSuccess = false : bSuccess = true;

	return bSuccess;
}

// vector operations
CFVec4		CFVec4::operator -	( void ) const
{
	CFVec4 v4Return;

	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );
	XMVECTOR& v4Result = *reinterpret_cast<XMVECTOR*>( &v4Return );

	//Negate
	v4Result = XMVectorNegate( v4V );

	return v4Return;
}

CFVec4		CFVec4::operator -	( CFVec4Arg vArg ) const
{
	CFVec4 v4Return;

	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v4V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );
	XMVECTOR& v4Result = *reinterpret_cast<XMVECTOR*>( &v4Return );

	//Negate
	v4Result = XMVectorSubtract( v4V, v4V2 );

	return v4Return;
}

CFVec4		CFVec4::operator +	( CFVec4Arg vArg ) const
{
	CFVec4 v4Return;

	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v4V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );
	XMVECTOR& v4Result = *reinterpret_cast<XMVECTOR*>( &v4Return );

	//Negate
	v4Result = XMVectorAdd( v4V, v4V2 );

	return v4Return;
}

CFVec4		CFVec4::operator *	( CFVec4Arg vArg ) const
{
	CFVec4 v4Return;

	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v4V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );
	XMVECTOR& v4Result = *reinterpret_cast<XMVECTOR*>( &v4Return );

	//Negate
	v4Result = XMVectorMultiply( v4V, v4V2 );

	return v4Return;
}

CFVec4&	CFVec4::operator +=	( CFVec4Arg vArg )
{
	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>( this );
	const XMVECTOR& v4V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );

	//Negate
	v4V = XMVectorAdd( v4V, v4V2 );

	return *this;
}

CFVec4&	CFVec4::operator -=	( CFVec4Arg vArg )
{
	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>( this );
	const XMVECTOR& v4V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );

	v4V = XMVectorSubtract( v4V, v4V2 );

	return *this;
}

CFVec4&	CFVec4::operator *=	( CFVec4Arg vArg )
{
	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>( this );
	const XMVECTOR& v4V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );

	v4V = XMVectorMultiply( v4V, v4V2 );

	return *this;
}

// scalar opertaors
CFVec4&	CFVec4::operator *=	( const FLOAT32 fArg )
{
	XMVECTOR v4Scaler;
	v4Scaler = XMVectorSet( fArg, fArg, fArg, fArg );

	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>( this );

	//Negate
	v4V = XMVectorMultiply( v4V, v4Scaler );

	return *this;
}

CFVec4		CFVec4::operator *	( const FLOAT32 fArg ) const
{
	CFVec4 v4Return;

	XMVECTOR v4Scaler;
	v4Scaler = XMVectorSet( fArg, fArg, fArg, fArg );

	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );
	XMVECTOR& v4Result = *reinterpret_cast<XMVECTOR*>( &v4Return );

	//Negate
	v4Result = XMVectorMultiply( v4V, v4Scaler );

	return v4Return;
}

CFVec4&	CFVec4::operator /=	( const FLOAT32 fArg )
{
	FLOAT32 fDivisor = 1.0f / fArg;

	XMVECTOR v4Scaler;
	v4Scaler = XMVectorSet( fDivisor, fDivisor, fDivisor, fDivisor );

	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>( this );

	//Negate
	v4V = XMVectorMultiply( v4V, v4Scaler );

	return *this;
}

CFVec4		CFVec4::operator /	( const FLOAT32 fArg ) const
{
	CFVec4 v4Return;

	FLOAT32 fDivisor = 1.0f / fArg;

	XMVECTOR v4Scaler;
	v4Scaler = XMVectorSet( fDivisor, fDivisor, fDivisor, fDivisor );

	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );
	XMVECTOR& v4Result = *reinterpret_cast<XMVECTOR*>( &v4Return );

	//Negate
	v4Result = XMVectorMultiply( v4V, v4Scaler );

	return v4Return;
}

CFVec4		CFVec4::CrossProduct( CFVec4Arg vArg ) const
{
	UNUSED_VAR( vArg );
#pragma warnmsg( "stub function that needs implementation" )
	assert_now( "stub function" );
	return g_fv4ZERO;
}

FLOAT32		CFVec4::DotProduct( CFVec4Arg vArg ) const
{
	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v4V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );

	return XMVectorGetX( XMVector4Dot( v4V, v4V2 ) );
}

/**
 *	@return A vector quantity with the scalar in each component.
 */
CFVec4		CFVec4::DotProductV( CFVec4Arg vArg ) const
{
	CFVec4 v4Return;

	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v4V2 = *reinterpret_cast<const XMVECTOR*>( &vArg );
	XMVECTOR& v4Result = *reinterpret_cast<XMVECTOR*>( &v4Return );

	v4Result = XMVector4Dot( v4V, v4V2 );

	return v4Return;
}

FLOAT32		CFVec4::SquareDistance( CFVec4Arg fv4To ) const
{
	CFVec4 v4Temp;
	v4Temp = fv4To - *this;

	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>( &v4Temp );

	return XMVectorGetX( XMVector4LengthSq( v4V ) );
}

/**
 *	@return A vector quantity with the scalar in each component.
 */
CFVec4		CFVec4::SquareDistanceV( CFVec4Arg fv4To ) const
{
	CFVec4 v4Return;
	v4Return = fv4To - *this;

	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>( &v4Return );

	v4V = XMVector4LengthSq( v4V );

	return v4Return;
}


FLOAT32		CFVec4::Distance( CFVec4Arg fv4To ) const
{
	CFVec4 v4Temp;
	v4Temp = fv4To - *this;

	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>( &v4Temp );

	return XMVectorGetX( XMVector4Length( v4V ) );
}

/**
 *	@return A vector quantity with the scalar in each component.
 */
CFVec4		CFVec4::DistanceV( CFVec4Arg fv4To ) const
{
	CFVec4 v4Temp;
	v4Temp = fv4To - *this;

	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>( &v4Temp );

	v4V = XMVector4Length( v4V );

	return v4Temp;
}

FLOAT32		CFVec4::SquareMagnitude( void ) const
{
	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );

	return XMVectorGetX( XMVector4LengthSq( v4V ) );
}

/**
 *	@return A vector quantity with the scalar in each component.
 */
CFVec4		CFVec4::SquareMagnitudeV( void ) const
{
	CFVec4 v4Return;

	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );
	XMVECTOR& v4Result = *reinterpret_cast<XMVECTOR*>( &v4Return );

	v4Result = XMVector4LengthSq( v4V );

	return v4Return;
}

FLOAT32		CFVec4::Magnitude( void ) const
{
	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );

	return XMVectorGetX( XMVector4Length( v4V ) );
}

/**
 *	@return A vector quantity with the scalar in each component.
 */
CFVec4		CFVec4::MagnitudeV( void ) const
{
	CFVec4 v4Return;

	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );
	XMVECTOR& v4Result = *reinterpret_cast<XMVECTOR*>( &v4Return );

	v4Result = XMVector4Length( v4V );

	return v4Return;
}

void	CFVec4::Normalise( void )
{
	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>( this );

	v4V = XMVector4Normalize( v4V );
}

void	CFVec4::Normalise( FLOAT32 &fMagnitude )
{
	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>( this );

	//TODO: Is there a cheaper way of doing this?
	fMagnitude = XMVectorGetX( XMVector3Length( v4V ) );

	v4V = XMVector3Normalize( v4V );
}

/**
 *	@param vMagnitude	Contains the scalar magnitude in each component.
 */
void	CFVec4::Normalise( CFVec4 &vMagnitude )
{
	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>( this );
	XMVECTOR& v4VMag = *reinterpret_cast<XMVECTOR*>( &vMagnitude );

	//TODO: Is there a cheaper way of doing this?
	v4VMag = XMVector4Length( v4V );

	v4V = XMVector4Normalize( v4V );
}


CFVec4		CFVec4::GetNormal( void ) const
{
	CFVec4 v4Return;

	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );
	XMVECTOR& v4Result = *reinterpret_cast<XMVECTOR*>( &v4Return );

	v4Result = XMVector4Normalize( v4V );

	return v4Return;
}

CFVec4		CFVec4::GetNormal( FLOAT32 &fMagnitude ) const
{
	CFVec4 v4Return;

	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );
	XMVECTOR& v4Result = *reinterpret_cast<XMVECTOR*>( &v4Return );

	//TODO: Is there a cheaper way of doing this?
	fMagnitude = XMVectorGetX( XMVector4Length( v4V ) );

	v4Result = XMVector4Normalize( v4V );

	return v4Return;
}

/**
 *	@param vMagnitude	Contains the scalar magnitude in each component.
 */
CFVec4		CFVec4::GetNormal( CFVec4 &vMagnitude ) const
{
	CFVec4 v4Return;

	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );
	XMVECTOR& v4VMag = *reinterpret_cast<XMVECTOR*>( &vMagnitude );
	XMVECTOR& v4Result = *reinterpret_cast<XMVECTOR*>( &v4Return );

	//TODO: Is there a cheaper way of doing this?
	v4VMag = XMVector4Length( v4V );

	v4Result = XMVector4Normalize( v4V );

	return v4Return;
}


bool		CFVec4::IsNormal( void ) const
{
	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );

	//TODO: Cheaper way of doing this?
	//Optimization: Getting squared length is cheaper, and 1^2 is still 1.
	FLOAT32 fMag = XMVectorGetX( XMVector4LengthSq( v4V ) );

	bool bSuccess = false;
	fMag == 1.0f ? bSuccess = true : bSuccess = false;

	return bSuccess;
}

/**
 *	Component wise set to minimum of this and other.
 */
CFVec4&		CFVec4::MinimizeWith( CFVec4Arg fv4Other )
{
	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>( this );
	const XMVECTOR& v4V2 = *reinterpret_cast<const XMVECTOR*>( &fv4Other );

	v4V = XMVectorMin( v4V, v4V2 );
	
	return *this;
}

CFVec4		CFVec4::GetMin( CFVec4Arg fv4Other ) const
{
	CFVec4 v4Return;

	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v4V2 = *reinterpret_cast<const XMVECTOR*>( &fv4Other );
	XMVECTOR& v4Result = *reinterpret_cast<XMVECTOR*>( &v4Return );

	v4Result = XMVectorMin( v4V, v4V2 );

	return v4Return;
}

/**
 *	Component wise set to maximum of this and other.
 */
CFVec4&		CFVec4::MaximizeWith( CFVec4Arg fv4Other )
{
	XMVECTOR& v4V = *reinterpret_cast<XMVECTOR*>( this );
	const XMVECTOR& v4V2 = *reinterpret_cast<const XMVECTOR*>( &fv4Other );

	v4V = XMVectorMax( v4V, v4V2 );
	
	return *this;
}

CFVec4		CFVec4::GetMax( CFVec4Arg fv4Other ) const
{
	CFVec4 v4Return;

	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );
	const XMVECTOR& v4V2 = *reinterpret_cast<const XMVECTOR*>( &fv4Other );
	XMVECTOR& v4Result = *reinterpret_cast<XMVECTOR*>( &v4Return );

	v4Result = XMVectorMax( v4V, v4V2 );

	return v4Return;
}

/**
 *	Convert this to 4 components in a ubyte 4, with zero mapping to zero and one mapping to 255, clamped.
 */
UINT32		CFVec4::QuantizeToU8N( void ) const
{
	UINT32 uRV;
	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );
	XMStoreUByteN4( reinterpret_cast<XMUBYTEN4*>(&uRV), v4V );
	return uRV;
}


/**
 *	Convert this to a ubyte value, with zero mapping to zero and 255 mapping to 255, clamped.
 */
UINT32		CFVec4::QuantizeToU8( void ) const
{
	UINT32 uRV;
	const XMVECTOR& v4V = *reinterpret_cast<const XMVECTOR*>( this );
	XMStoreUByte4( reinterpret_cast<XMUBYTE4*>(&uRV), v4V );
	return uRV;
}

} // end namespace GDE
