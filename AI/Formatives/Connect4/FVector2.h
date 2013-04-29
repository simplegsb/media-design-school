/**
 *	@file Connect4\FVector2.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2012 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/

#ifndef _FVECTOR2_H__Included_1946371963
#define _FVECTOR2_H__Included_1946371963

// 2 component floating point vector class
// Should be passed by value ??? - however use this:
// as at the moment gonna do by reference
typedef const class CFVec2& CFVec2Arg;
class CFVec2
{
	struct FVec2
	{
		FLOAT32 x,y;
	};
public:
	///////////////////////////////////////////////////////
	// accessors
	inline void X( FLOAT32 v )
	{
		m_Vec.x = v;
	}
	inline void Y( FLOAT32 v )
	{
		m_Vec.y = v;
	}

	inline FLOAT32 X( void ) const
	{
		return m_Vec.x;
	}
	inline FLOAT32 Y( void ) const
	{
		return m_Vec.y;
	}

	inline void XAdd( FLOAT32 v )
	{
		m_Vec.y += v;
	}

	inline void YAdd( FLOAT32 v )
	{
		m_Vec.y += v;
	}

	//////////////////////////////////////////////////////////
	// operator overloads
	inline bool operator == (CFVec2Arg arg ) const
	{
		return X() == arg.X() && Y() == arg.Y();
	}

	inline bool operator != (CFVec2Arg arg ) const
	{
		return X() != arg.X() || Y() != arg.Y();
	}

	// vector operations
	inline CFVec2 operator-( CFVec2Arg arg ) const
	{
		//TODO: optimise with using the vector unit maths
		CFVec2 RetVal;
		RetVal.X( X() - arg.X() );
		RetVal.Y( Y() - arg.Y() );
		return RetVal;
	}

	// negate
	inline CFVec2 operator-( void ) const
	{
		//TODO: optimise with using the vector unit maths
		CFVec2 RetVal;
		RetVal.X( -X() );
		RetVal.Y( -Y() );
		return RetVal;
	}


	inline CFVec2 operator+( CFVec2Arg arg ) const
	{
		//TODO: optimise with using the vector unit maths
		CFVec2 RetVal;
		RetVal.X( X() + arg.X() );
		RetVal.Y( Y() + arg.Y() );
		return RetVal;
	}


	inline CFVec2Arg operator+=( CFVec2Arg arg )
	{
		//TODO: optimise with using the vector unit maths
		m_Vec.x += arg.X();
		m_Vec.y += arg.Y();
		return *this;

	}

	inline CFVec2Arg operator-=( CFVec2Arg arg )
	{
		//TODO: optimise with using the vector unit maths
		m_Vec.x -= arg.X();
		m_Vec.y -= arg.Y();
		return *this;

	}
	// scalar opertaors
	inline CFVec2Arg operator*=( const FLOAT32 fArg )
	{
		//TODO: optimise with using the vector unit maths
		m_Vec.x *= fArg;
		m_Vec.y *= fArg;
		return *this;
	}

	inline CFVec2 operator*( const FLOAT32 fArg ) const
	{
		//TODO: optimise with using the vector unit maths
		CFVec2 RetVal;
		RetVal.X( X() * fArg );
		RetVal.Y( Y() * fArg );
		return RetVal;
	}


	inline CFVec2Arg operator/=( const FLOAT32 fArg )
	{
		//TODO: optimise with using the vector unit maths
		assert( fArg != 0.0f );
		return ((*this)*=(1.0f/fArg));
	}

	inline CFVec2 operator/( const FLOAT32 fArg ) const
	{
		//TODO: optimise with using the vector unit maths
		assert( fArg != 0.0f );
//		return ((*this)*(1.0f/arg));
		return (*this) * (1.0f/fArg);
	}

	// vector operators:
	// scalar opertaors
	inline CFVec2Arg operator*=( CFVec2Arg arg )
	{
		//TODO: optimise with using the vector unit maths
		m_Vec.x *= arg.m_Vec.x;
		m_Vec.y *= arg.m_Vec.y;
		return *this;
	}

	inline FLOAT32 SquareMagnitude( void ) const
	{
		return (X()*X()+Y()*Y());
	}

	FLOAT32 Magnitude( void ) const;

	inline FLOAT32 SquareDistance( CFVec2Arg fv2To ) const
	{
		return ((*this)-fv2To).SquareMagnitude();
	}

	inline CFVec2Arg Normalise( void )
	{
		// TODO: optimise
//		(*this) /= ( Magnitude() );
//		return *this;
		assert( this != NULL );
		const FLOAT32 Mag = Magnitude();
		assert( Mag != 0.0f );

		const FLOAT32 fMult = 1.0f/Mag;

		X( X() * fMult );
		Y( Y() * fMult );
		return (*this);
	}

	inline CFVec2Arg Normalise( FLOAT32 &fMagnitude )
	{
		// TODO: optimise
//		(*this) /= ( Magnitude() );
//		return *this;
		assert( this != NULL );
		fMagnitude = Magnitude();
		assert( fMagnitude != 0.0f );

		const FLOAT32 fMult = 1.0f/fMagnitude;

		X( X() * fMult );
		Y( Y() * fMult );
		return (*this);
	}

	inline CFVec2Arg SetPerpendicular( void )
	{
		const FLOAT32& t = X();
		X( Y() );
		Y( -t );
		return (*this);
	}

	inline CFVec2 GetNormal( void ) const
	{
		CFVec2 RetVal = *this;
		return RetVal.Normalise();
	}

	inline CFVec2 GetNormal( FLOAT32 &fMagnitude ) const
	{
		CFVec2 RetVal = *this;
		return RetVal.Normalise( fMagnitude );
	}

	bool IsNormal( void ) const;

	inline FLOAT32 DotProduct( CFVec2Arg arg ) const
	{
		// u . v = |u||v|*cos( angle between vectors )
		// TODO: optimise
		FLOAT32 RetVal = (X() * arg.X()) + (Y() * arg.Y());
		return RetVal;
	}

	CFVec2Arg RotateZ( FLOAT32 fRadians );
	void SetToRotationZ( FLOAT32 fRadians );

#ifdef _DEBUG
	void DebugDump( void ) const
	{
		DbgPrint( "%0.2f, %0.2f",
			X(),
			Y() );
	}
	void DebugDumpCR( void ) const
	{
		DbgPrint( "%0.2f, %0.2f\n",
			X(),
			Y() );
	}
#else
	inline void DebugDump( void ) const {}
	inline void DebugDumpCR( void ) const {}
#endif
protected:
	FVec2 m_Vec;
};

/**
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

	// cast to a CFVec3
	operator const CFVec2& (void) const
	{
		return *((CFVec2*)this);
	}
};

const static CFVec2 fv2EMPTY = SFVec2( 0.0f, 0.0f );

#endif //_FVECTOR2_H__Included_1946371963

