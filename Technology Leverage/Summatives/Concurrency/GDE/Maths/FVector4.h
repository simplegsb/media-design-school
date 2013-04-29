/**
 *	@file GDE\Maths\FVector4.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _FVECTOR4_H__Included_1500975245
#define _FVECTOR4_H__Included_1500975245

#include "GDE/Core/MemoryAlignment.h"

namespace GDE
{

/**
 *	@author Jon Booth.
 *	@ingroup Maths
 *
 *	When passing a CFVec2 around, use this as an argument rather than 
 *	directly passing by value, or by reference - unless that specific behaviour
 *	is desired. As a result, platform and implementation optimised passing behaviour
 *	can be implemented.
 */
typedef const class CFVec2& CFVec2Arg;
/**
 *	@author Jon Booth.
 *	@ingroup Maths
 *
 *	When passing a CFVec3 around, use this as an argument rather than 
 *	directly passing by value, or by reference - unless that specific behaviour
 *	is desired. As a result, platform and implementation optimised passing behaviour
 *	can be implemented.
 */
typedef const class CFVec3& CFVec3Arg;
/**
 *	@author Jon Booth.
 *	@ingroup Maths
 *
 *	When passing a CFVec4 around, use this as an argument rather than 
 *	directly passing by value, or by reference - unless that specific behaviour
 *	is desired. As a result, platform and implementation optimised passing behaviour
 *	can be implemented.
 */
typedef const class CFVec4& CFVec4Arg;

/**
 *	@author Jon Booth.
 *	@ingroup Maths
 *
 *	Used for initialising CFVec4's
 *	Also is a simple version with no particular memory alignment requirements or pretensions
 *	of being hardware specific vectorized and in a register (SSE, AltiVec, VPU etc).
 **/
struct SFVec4 
{
	SFVec4( FLOAT32 x, FLOAT32 y, FLOAT32 z, FLOAT32 w )
		: m_fX( x )
		, m_fY( y )
		, m_fZ( z )
		, m_fW( w )
	{ // intentionally empty
	}

	SFVec4( void )
	{ // intentionally empty
	}

	// cast to a CFVec4
	operator const CFVec4 (void) const;

	FLOAT32 m_fX;
	FLOAT32 m_fY;
	FLOAT32 m_fZ;
	FLOAT32 m_fW;
};

/**
 *	@ingroup Maths
 *
 *	A four floating point component vector4, aligned and ready for
 *	fast vector procesesing.
 */
_ALIGNED_PREFIX( 16 ) class CFVec4 : public TAligned<16>
{
public:
#ifdef _GDE_TARGET_PSP
	CFVec4Arg operator=( const ScePspFVector3 &arg );
	CFVec4Arg operator=( const SceGmoVec4 &arg );
	// convenience cast:
	const ScePspFVector4* GePSPVecP( void ) const
	{
		return &m_Vec;
	}
#else
	struct FVec4
	{
		FLOAT32 x,y,z,w;
	};
#endif

	static CFVec4 Init( FLOAT32 fX, FLOAT32 fY, FLOAT32 fZ = 0.0f, FLOAT32 fW = 1.0f );
	CFVec4&		operator=( const SFVec4 &arg );

	void		DebugDump( void ) const;
	void		DebugDumpCR( void ) const;

	///////////////////////////////////////////////////////
	// accessors		
	void X( FLOAT32 v );
	void Y( FLOAT32 v );
	void Z( FLOAT32 v );
	void W( FLOAT32 v );
	FLOAT32 X( void ) const;
	FLOAT32 Y( void ) const;
	FLOAT32 Z( void ) const;
	FLOAT32 W( void ) const;

	void		SetXY( CFVec2Arg fv2Source );
	void		SetXYZ( CFVec3Arg fv2Source );


	void XAdd( FLOAT32 v );
	void YAdd( FLOAT32 v );
	void ZAdd( FLOAT32 v );
	void WAdd( FLOAT32 v );
	//////////////////////////////////////////////////////////
	// operator overloads
	bool		operator == (CFVec4Arg vArg ) const;
	bool		operator != (CFVec4Arg vArg ) const;

	// vector operations
	CFVec4		operator -	( void ) const;

	CFVec4		operator -	( CFVec4Arg vArg ) const;
	CFVec4		operator +	( CFVec4Arg vArg ) const;
	CFVec4		operator *	( CFVec4Arg vArg ) const;
	CFVec4&		operator +=	( CFVec4Arg vArg );
	CFVec4&		operator -=	( CFVec4Arg vArg );
	CFVec4&		operator *=	( CFVec4Arg vArg );

	// scalar opertaors
	CFVec4&		operator *=	( const FLOAT32 fArg );
	CFVec4		operator *	( const FLOAT32 fArg ) const;
	CFVec4&		operator /=	( const FLOAT32 fArg );
	CFVec4		operator /	( const FLOAT32 fArg ) const;

	///////////////////////////////////////////////////////////
	// standard vector math:
	CFVec4		CrossProduct( CFVec4Arg vArg ) const;
	FLOAT32		DotProduct( CFVec4Arg vArg ) const;
	CFVec4		DotProductV( CFVec4Arg vArg ) const;
	FLOAT32		SquareDistance( CFVec4Arg fv3To ) const;
	CFVec4		SquareDistanceV( CFVec4Arg fv3To ) const;
	FLOAT32		Distance( CFVec4Arg fv3To ) const;
	CFVec4		DistanceV( CFVec4Arg fv3To ) const;
	FLOAT32		SquareMagnitude( void ) const;
	CFVec4		SquareMagnitudeV( void ) const;
	FLOAT32		Magnitude( void ) const;
	CFVec4		MagnitudeV( void ) const;
	void		Normalise( void );
	void		Normalise( FLOAT32 &fMagnitude );
	void		Normalise( CFVec4 &vMagnitude );
	CFVec4		GetNormal( void ) const;
	CFVec4		GetNormal( FLOAT32 &fMagnitude ) const;
	CFVec4		GetNormal( CFVec4 &vMagnitude ) const;
	bool		IsNormal( void ) const;

	void		RotateX( FLOAT32 fRadians );
	void		RotateY( FLOAT32 fRadians );
	void		RotateZ( FLOAT32 fRadians );

	// if a direction vector - converting to Euler without roll
	FLOAT32		GetHeading( void ) const;
	FLOAT32		GetElevation( void ) const;

	CFVec4&		MinimizeWith( CFVec4Arg fv4Other );
	CFVec4		GetMin( CFVec4Arg fv4Other ) const;
	CFVec4&		MaximizeWith( CFVec4Arg fv4Other );
	CFVec4		GetMax( CFVec4Arg fv4Other ) const;

	UINT32		QuantizeToU8N( void ) const;
	UINT32		QuantizeToU8( void ) const;

private:
#ifdef _GDE_TARGET_PSP
	ScePspFVector4	m_Vec;
#else
	FVec4			m_Vec;
#endif
};

/**
 *	@author Jon Booth.
 *	@ingroup Maths
 *
 *	A global four component vector, zero initialised.
 */
extern const CFVec4 g_fv4ZERO;

}; // end namespace GDE

#endif //_FVECTOR4_H__Included_1500975245

