/**
 *	@file GDE\Maths\FVector3.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _FVECTOR3_H__Included_1478343780
#define _FVECTOR3_H__Included_1478343780

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
 *	Used for initialising CFVec3's
 *	Also is a simple version with no particular memory alignment requirements or pretensions
 *	of being hardware specific vectorized and in a register (SSE, AltiVec, VPU etc).
 **/
struct SFVec3 
{
	SFVec3( FLOAT32 x, FLOAT32 y, FLOAT32 z )
		: m_fX( x )
		, m_fY( y )
		, m_fZ( z )
	{ // intentionally empty
	}

	SFVec3( void )
	{ // intentionally empty
	}

	// cast to a CFVec3
	operator const CFVec3 (void) const;

	FLOAT32 m_fX;
	FLOAT32 m_fY;
	FLOAT32 m_fZ;
};

/**
 *	@ingroup Maths
 *
 *	A fast, memory aligned three component floating point vector.
 */
_ALIGNED_PREFIX( 16 ) class CFVec3 : public TAligned<16>
{
public:
	// no construtor thanx... - see initialiser
	// however this means you can't write
	// CFVec3 fv3Mine = (ScePspFVector3) other;
	// as it tries for a constuctor - you'll have to do
	// the assignment on a seperate line to the declaration

	// copy operator
#ifdef _GDE_TARGET_PSP
	CFVec3Arg operator=( const ScePspFVector3 &arg );
	CFVec3Arg operator=( const SceGmoVec3 &arg );
	// convenience cast:
	const ScePspFVector3* GePSPVecP( void ) const
	{
		return &m_Vec;
	}
#else
	struct FVec3
	{
		FLOAT32 x,y,z;
	};
#endif

	static CFVec3 Init( FLOAT32 fX, FLOAT32 fY, FLOAT32 fZ = 0.0f );
	CFVec3&		operator=( const SFVec3 &arg );

	void		DebugDump( void ) const;
	void		DebugDumpCR( void ) const;

	///////////////////////////////////////////////////////
	// accessors
	void		X( FLOAT32 fVal );
	void		Y( FLOAT32 fVal );
	void		Z( FLOAT32 fVal );
	FLOAT32		X( void ) const;
	FLOAT32		Y( void ) const;
	FLOAT32		Z( void ) const;

	void		SetXY( CFVec2Arg fv2Source );

	void		XAdd( FLOAT32 fVal );
	void		YAdd( FLOAT32 fVal );
	void		ZAdd( FLOAT32 fVal );

	//////////////////////////////////////////////////////////
	// operator overloads
	bool		operator == (CFVec3Arg vArg ) const;
	bool		operator != (CFVec3Arg vArg ) const;

	// vector operations
	CFVec3		operator -	( void ) const;

	CFVec3		operator -	( CFVec3Arg vArg ) const;
	CFVec3		operator +	( CFVec3Arg vArg ) const;
	CFVec3		operator *	( CFVec3Arg vArg ) const;
	CFVec3&		operator +=	( CFVec3Arg vArg );
	CFVec3&		operator -=	( CFVec3Arg vArg );
	CFVec3&		operator *=	( CFVec3Arg vArg );

	// scalar opertaors
	CFVec3&		operator *=	( const FLOAT32 fArg );
	CFVec3		operator *	( const FLOAT32 fArg ) const;
	CFVec3&		operator /=	( const FLOAT32 fArg );
	CFVec3		operator /	( const FLOAT32 fArg ) const;

	///////////////////////////////////////////////////////////
	// standard vector math:
	CFVec3		CrossProduct( CFVec3Arg vArg ) const;
	FLOAT32		DotProduct( CFVec3Arg vArg ) const;
	CFVec3		DotProductV( CFVec3Arg vArg ) const;
	FLOAT32		SquareDistance( CFVec3Arg fv3To ) const;
	CFVec3		SquareDistanceV( CFVec3Arg fv3To ) const;
	FLOAT32		Distance( CFVec3Arg fv3To ) const;
	CFVec3		DistanceV( CFVec3Arg fv3To ) const;
	FLOAT32		SquareMagnitude( void ) const;
	CFVec3		SquareMagnitudeV( void ) const;
	FLOAT32		Magnitude( void ) const;
	CFVec3		MagnitudeV( void ) const;
	void		Normalise( void );
	void		Normalise( FLOAT32 &fMagnitude );
	void		Normalise( CFVec3 &vMagnitude );
	CFVec3		GetNormal( void ) const;
	CFVec3		GetNormal( FLOAT32 &fMagnitude ) const;
	CFVec3		GetNormal( CFVec3 &vMagnitude ) const;
	bool		IsNormal( void ) const;

	void		RotateX( FLOAT32 fRadians );
	void		RotateY( FLOAT32 fRadians );
	void		RotateZ( FLOAT32 fRadians );
	void		Rotate(FLOAT32 fPitch, FLOAT32 fYaw, FLOAT32 fRoll);

	// if a direction vector - converting to Euler without roll
	FLOAT32		GetHeading( void ) const;
	FLOAT32		GetElevation( void ) const;

	CFVec3&		MinimizeWith( CFVec3Arg fv3Other );
	CFVec3		GetMin( CFVec3Arg fv3Other ) const;
	CFVec3&		MaximizeWith( CFVec3Arg fv3Other );
	CFVec3		GetMax( CFVec3Arg fv3Other ) const;

	CFVec3		Lerp( CFVec3Arg vTarget, GDE::FLOAT32 fT );

private:
#ifdef _GDE_TARGET_PSP
	ScePspFVector3	m_Vec;
#else
	FVec3			m_Vec;
	FLOAT32			m_fPad;	//!< Does nothing, is just nasty padding.
#endif
};

/**
 *	@author Jon Booth.
 *	@ingroup Maths
 *
 *	A global three component vector, zero initialised.
 */
extern const CFVec3 g_fv3ZERO;

} // end namespace GDE

#endif //_FVECTOR3_H__Included_1478343780

