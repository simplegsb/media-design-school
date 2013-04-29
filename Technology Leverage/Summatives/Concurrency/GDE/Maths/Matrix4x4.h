/**
 *	@file GDE\Maths\Matrix4x4.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _MATRIX4X4_H__Included_1844716804
#define _MATRIX4X4_H__Included_1844716804

#include "GDE/Core/MemoryAlignment.h"

namespace GDE
{
	class CFMat4x4;
	// these types are used to pass by value, on some platforms
	// passing by reference would be preferred, hence these
	// defines will be different...
	typedef const class CFVec3& CFVec3Arg;
	typedef const class CFVec4& CFVec4Arg;

	/**
	 *	@ingroup Maths
	 *
	 *	Unaligned version, used for initialisation etc.
	 *	@see CFMat4x4.
	 */
	struct SFMat4x4
	{
		FLOAT32	m_fVal[4][4];

		operator const CFMat4x4 (void) const;
	};

	typedef const class CFMat4x4& CFMat4x4Arg;	// used so can be passed by value if required.

	/**
	 *	@ingroup Maths
	 *
	 *	A fast, memory aligned optimised 4x4 matrix.
	 */
	_ALIGNED_PREFIX( 16 ) class CFMat4x4 : public TAligned<16>
	{
	public:
		// no constructor, can be part of union etc.
		// assignment, equal to an SFMat4x4
		CFMat4x4&		operator=( const SFMat4x4& mOther );

		// accessors:
//		FLOAT32			Get( UINT32 uRow, UINT32 uColumn );
//		void			Set( UINT32 uRow, UINT32 uColumn, FLOAT32 fValue );

		// scalar overloads:
		CFMat4x4		operator *	( FLOAT32 fScalar ) const;
		CFMat4x4		operator /	( FLOAT32 fScalar ) const;
		CFMat4x4&		operator *=	( FLOAT32 fScalar );
		CFMat4x4&		operator /=	( FLOAT32 fScalar );

		// matrix overloads
		CFMat4x4		operator *	( CFMat4x4Arg mOther ) const;
		CFMat4x4		operator /	( CFMat4x4Arg mOther ) const;
		CFMat4x4&		operator *=	( CFMat4x4Arg mOther );
		CFMat4x4&		operator /=	( CFMat4x4Arg mOther );

		// applying transforms in order:
		void			PreTransformBy( CFMat4x4Arg mOther );
		CFMat4x4		GetPreTransformedBy( CFMat4x4Arg mOther ) const;
		void			PostTransformBy( CFMat4x4Arg mOther );
		CFMat4x4		GetPostTransformedBy( CFMat4x4Arg mOther ) const;

		// transpose and inverse:
		void			Transpose( void );
		CFMat4x4		GetTransposed( void );
		bool			Inverse( void );
		bool			GetInverse( CFMat4x4& mResult ) const;

		// convert handedness
		void			ConvertHandedness( void );

		// transform vectors:
		CFVec3			TransformVec3( CFVec3Arg vVec );
		CFVec4			TransformVec4( CFVec3Arg vVec );
		CFVec4			TransformVec4( CFVec4Arg vVec );

		// Create prebuilt transforms:
		static CFMat4x4	CreateTranslation( FLOAT32 fX, FLOAT32 fY, FLOAT32 fZ );
		static CFMat4x4	CreateTranslation( CFVec3Arg vPosition );
		
		static CFMat4x4	CreateScale( FLOAT32 fX, FLOAT32 fY, FLOAT32 fZ );
		static CFMat4x4	CreateYawPitchRoll( FLOAT32 fYaw, FLOAT32 fPitch, FLOAT32 fRoll );
		static CFMat4x4	CreateRotationX( FLOAT32 fRadians );
		static CFMat4x4	CreateRotationY( FLOAT32 fRadians );
		static CFMat4x4	CreateRotationZ( FLOAT32 fRadians );
		static CFMat4x4	CreateRotationAxis( FLOAT32 fX, FLOAT32 fY, FLOAT32 fZ, FLOAT32 fDegs );
		static CFMat4x4	CreateProjection( FLOAT32 fFOV, FLOAT32 fAspect, FLOAT32 fNear, FLOAT32 fFar );
		static CFMat4x4 CreateOrtho( FLOAT32 fViewWidth, FLOAT32 fViewHeight, FLOAT32 fNearZ, FLOAT32 fFarZ );
		static CFMat4x4 GetIdentity( void );

		// View matrix creation
		static CFMat4x4 CreateViewDirection(CFVec3Arg vPos, CFVec3Arg vDir, CFVec3Arg vUp);
		static CFMat4x4 CreateViewLookAt(CFVec3Arg vPos, CFVec3Arg vTarget, CFVec3Arg vUp);

	protected:
		SFMat4x4	m_Matrix;		//!< The matrix itself.
	};
	
	/**
	 *	@ingroup Maths
	 *
	 *	The identity matrix.
	 */
	extern const CFMat4x4 g_m4x4Identity;
};

#endif //_MATRIX4X4_H__Included_1844716804

