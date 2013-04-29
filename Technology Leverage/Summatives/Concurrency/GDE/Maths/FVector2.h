/**
 *	@file GDE\Maths\FVector2.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _FVECTOR2_H__Included_1946371963
#define _FVECTOR2_H__Included_1946371963

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
 *	A two component floating point vector class. Could be SSE optimised.
 */
class CFVec2
{
	struct FVec2
	{
		FLOAT32 x,y;
	};
public:
	///////////////////////////////////////////////////////
	// accessors
	void X( FLOAT32 v );
	void Y( FLOAT32 v );
	FLOAT32 X( void ) const;
	FLOAT32 Y( void ) const;
	void XAdd( FLOAT32 v );
	void YAdd( FLOAT32 v );

	//////////////////////////////////////////////////////////
	// operator overloads
	bool operator == (CFVec2Arg arg ) const;
	bool operator != (CFVec2Arg arg ) const;

	// vector operations
	CFVec2 operator-( void ) const;
	CFVec2 operator-( CFVec2Arg arg ) const;
	CFVec2 operator+( CFVec2Arg arg ) const;
	CFVec2Arg operator+=( CFVec2Arg arg );
	CFVec2Arg operator-=( CFVec2Arg arg );
	CFVec2Arg operator*=( CFVec2Arg arg );
	FLOAT32 SquareMagnitude( void ) const;
	FLOAT32 Magnitude( void ) const;

	// scalar opertaors
	CFVec2Arg operator*=( const FLOAT32 fArg );
	CFVec2 operator*( const FLOAT32 fArg ) const;
	CFVec2Arg operator/=( const FLOAT32 fArg );
	CFVec2 operator/( const FLOAT32 fArg ) const;

	FLOAT32 SquareDistance( CFVec2Arg fv2To ) const;

	CFVec2Arg Normalise( void );
	CFVec2Arg Normalise( FLOAT32 &fMagnitude );
	CFVec2Arg SetPerpendicular( void );
	CFVec2 GetNormal( void ) const;
	CFVec2 GetNormal( FLOAT32 &fMagnitude ) const;
	bool IsNormal( void ) const;

	FLOAT32 DotProduct( CFVec2Arg arg ) const;

	CFVec2Arg RotateZ( FLOAT32 fRadians );
	void SetToRotationZ( FLOAT32 fRadians );
	void SetAbs( void );
	void DebugDump( void ) const;
	void DebugDumpCR( void ) const;
protected:
	FVec2 m_Vec;
};

/**
 *	@author Jon Booth.
 *	@ingroup Maths
 *
 *	Used for initialising CFVec2's
 *	Also is a simple version with no particular memory alignment requirements or pretensions
 *	of being hardware specific vectorized and in a register (SSE, AltiVec, VPU etc).
 **/
struct SFVec2 
{
	SFVec2( FLOAT32 x, FLOAT32 y )
		: m_x( x )
		, m_y( y )
	{ // intentionally empty
	}

	SFVec2( void )
	{ // intentionally empty
	}

	FLOAT32 m_x;
	FLOAT32 m_y;

	// cast to a CFVec2
	operator const CFVec2& (void) const
	{
		return *((CFVec2*)this);
	}
};

/**
 *	@author Jon Booth.
 *	@ingroup Maths
 *
 *	A global two component vector, zero initialised.
 */
const static CFVec2 fv2EMPTY = SFVec2( 0.0f, 0.0f );

} // end namespace GDE

#endif //_FVECTOR2_H__Included_1946371963

