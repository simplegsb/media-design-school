/**
 *	@file GDE\Maths\Matrix4x4.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "Matrix4x4.h"
#include "FVector3.h"
#include "FVector4.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <xnamath.h>

namespace GDE
{

/**
 *	Initialise identity matrix
 */
const static SFMat4x4 gs_m4x4Identity =
{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};
const CFMat4x4 g_m4x4Identity = gs_m4x4Identity;

/**
 *	Cast SFMat4x4 to CFMat4x4
 */
SFMat4x4::operator const CFMat4x4 (void) const
{
	CFMat4x4 RV;
	RV = *this;
	return RV;
}

/**
 *	Assignment operator, allows setting to the non-aligned version.
 *	@param mOther	Non aligned matrix to load this from.
 */
CFMat4x4& CFMat4x4::operator=( const SFMat4x4& mOther )
{
	// Note, this can be faster implemented with a loadF instruction...
	XMMATRIX& M = *reinterpret_cast<XMMATRIX*>(this);
	M.r[0] = XMVectorSet(mOther.m_fVal[0][0], mOther.m_fVal[0][1], mOther.m_fVal[0][2], mOther.m_fVal[0][3]);
    M.r[1] = XMVectorSet(mOther.m_fVal[1][0], mOther.m_fVal[1][1], mOther.m_fVal[1][2], mOther.m_fVal[1][3]);
    M.r[2] = XMVectorSet(mOther.m_fVal[2][0], mOther.m_fVal[2][1], mOther.m_fVal[2][2], mOther.m_fVal[2][3]);
    M.r[3] = XMVectorSet(mOther.m_fVal[3][0], mOther.m_fVal[3][1], mOther.m_fVal[3][2], mOther.m_fVal[3][3]);
	return *this;
}

////////////////////////////////////////////////////////////////
//
//	Note: Students always want to implement these.
//	Please don't - it shouldn't really be needed to get/set
//	individual elements.
//
///**
// *	Get a single floating point value from the matrix
// *	Note, avoid doing this as it is slow!
// *
// *	@return the value at the given row and column.
// */
//FLOAT32		CFMat4x4::Get( UINT32 uRow, UINT32 uColumn )
//{
//	// note: this is not the way to implement, we don't get elements like this.
//	return m_Matrix.m_fVal[uRow][uColumn];
//}
//
///**
// *	Set a single floating point value to the matrix
// *	Note, avoid doing this as it is slow!
// *
// *	@param uRow			Row to set.
// *	@param uColumn		Column to set.
// *	@param fValue		Value to set.
// *  @author Campbell Dixon 
// */
//void		CFMat4x4::Set( UINT32 uRow, UINT32 uColumn, FLOAT32 fValue )
//{
//	// note: this is not the way to implement, we don't get elements like this.
//	m_Matrix.m_fVal[uRow][uColumn] = fValue;
//}

// scalar overloads:
CFMat4x4	CFMat4x4::operator *	( FLOAT32 fScalar ) const
{
	//Create the CFMat4x4 to return.
	CFMat4x4 matReturn;

	//Set its matrix member to the same as the LHS matrix class.
	matReturn.m_Matrix = m_Matrix;

	//Do the multiplication.
	for ( UINT32 uiRow = 0; uiRow < 4; ++uiRow )
	{
		for ( UINT32 uiCol = 0; uiCol < 4; ++uiCol )
		{
			matReturn.m_Matrix.m_fVal[uiRow][uiCol] *= fScalar;
		}
	}

	//Return it.
	return matReturn;
}

/**
 *	Divides a matrix by a scalar, and returns the divided matrix.
 *
 *	@param fScalar The scalar to divide by.
 *  @return Returns the result of the division as a matrix.
 */
CFMat4x4	CFMat4x4::operator /	( FLOAT32 fScalar ) const
{
	//Create the CFMat4x4 to return.
	CFMat4x4 matReturn;

	//Set its matrix member to the same as the LHS matrix class.
	matReturn.m_Matrix = m_Matrix;

	//Optimization: Do division once with 1 as numerator. Then multiply with it.
	//Avoids multiple divisions!
	FLOAT32 fDivisor = 1.0f / fScalar;

	//Do the division.
	for ( UINT32 uiRow = 0; uiRow < 4; ++uiRow )
	{
		for ( UINT32 uiCol = 0; uiCol < 4; ++uiCol )
		{
			matReturn.m_Matrix.m_fVal[uiRow][uiCol] *= fDivisor;
		}
	}

	//Return it.
	return matReturn;
}

/**
 *	Multiplys a matrix by a scalar.
 *
 *	@param fScalar The scalar to multiply by.
 */
CFMat4x4&	CFMat4x4::operator *=	( FLOAT32 fScalar )
{
	//Do the multiplication.
	//Component wise, as scalar multiplication.
	for ( UINT32 uiRow = 0; uiRow < 4; ++uiRow )
	{
		for ( UINT32 uiCol = 0; uiCol < 4; ++uiCol )
		{
			m_Matrix.m_fVal[uiRow][uiCol] *= fScalar;
		}
	}

	//Return it.
	return *this;
}

/**
 *	Divides a matrix by a scalar.
 *
 *	@param fScalar The scalar to divide by.
 */
CFMat4x4&	CFMat4x4::operator /=	( FLOAT32 fScalar )
{
	//Optimization: Do division once with 1 as numerator. Then multiply with it.
	//Avoids multiple divisions!
	FLOAT32 fDivisor = 1.0f / fScalar;

	//Do the division.
	//Component wise, as scalar division.
	for ( UINT32 uiRow = 0; uiRow < 4; ++uiRow )
	{
		for ( UINT32 uiCol = 0; uiCol < 4; ++uiCol )
		{
			m_Matrix.m_fVal[uiRow][uiCol] *= fDivisor;
		}
	}

	//Return it.
	return *this;
}

/**
 *	Multiplies a matrix by another matrix, and returns the resulting matrix.
 *
 *	@param mOther The matrix to multiply by.
 *  @return Returns the result of the multiplication as a matrix.
 */
CFMat4x4	CFMat4x4::operator *	( CFMat4x4Arg mOther ) const
{
	//Create the CFMat4x4 to return.
	CFMat4x4 matReturn;

	//Get referances of type XMMATRIX to our CFMat4x4 classes.
	const XMMATRIX& matA = *reinterpret_cast<const XMMATRIX*>( this );
	const XMMATRIX& matB = *reinterpret_cast<const XMMATRIX*>( &mOther );
	XMMATRIX& matResult = *reinterpret_cast<XMMATRIX*>( &matReturn );

	//Do the multiplication
	matResult = XMMatrixMultiply( matA, matB ) ;

	//Return the temp matrix.
	return matReturn;
}

/**
 *	Divides a matrix by another matrix, and returns the resulting matrix.
 *
 *	@param mOther The matrix to divide by.
 *  @return Returns the result of the division as a matrix.
 */
CFMat4x4	CFMat4x4::operator /	( CFMat4x4Arg mOther ) const
{
	//Create the CFMat4x4 to return.
	CFMat4x4 matReturn;

	XMVECTOR vDeterminant;

	const XMMATRIX& matA = *reinterpret_cast<const XMMATRIX*>( this );
	const XMMATRIX& matB = *reinterpret_cast<const XMMATRIX*>( &mOther );
	XMMATRIX& matResult = *reinterpret_cast<XMMATRIX*>( &matReturn );

	//Get the inverse of the matrix:
	matResult = XMMatrixInverse( &vDeterminant, matA );
	
	//Multiply by the other matrix:
	matResult = XMMatrixMultiply( matResult, matB );

	return matReturn;
}

/**
 *	Multiplies this matrix by another matrix.
 *
 *	@param mOther The matrix to multiply by.
 *  @return self
 */
CFMat4x4&	CFMat4x4::operator *=	( CFMat4x4Arg mOther )
{
	//Get referances of type XMMATRIX to our CFMat4x4 classes.
	XMMATRIX& matA = *reinterpret_cast<XMMATRIX*>( this );
	const XMMATRIX& matB = *reinterpret_cast<const XMMATRIX*>( &mOther );

	//Do the multiplication
	matA = XMMatrixMultiply( matA, matB ) ;

	return *this;
}

/**
 *	Divides this matrix by another matrix.
 *
 *	@param mOther The matrix to multiply by.
 *  @return self.
 */
CFMat4x4&	CFMat4x4::operator /=	( CFMat4x4Arg mOther )
{
	XMVECTOR vDeterminant;

	XMMATRIX& matA = *reinterpret_cast<XMMATRIX*>( this );
	const XMMATRIX& matB = *reinterpret_cast<const XMMATRIX*>( &mOther );
		
	//Get the inverse of the matrix:
	matA = XMMatrixInverse( &vDeterminant, matA );
	
	//Multiply by the other matrix:
	matA = XMMatrixMultiply( matA, matB );

	return *this;
}

/**
 *	Set this matrix to be the matrix that performs the transform in
 *	mOther followed by the transform already in this matrix.
 *
 *	@param mOther The matrix to transform by.
 */
void		CFMat4x4::PreTransformBy( CFMat4x4Arg mOther )
{
	XMMATRIX& matA = *reinterpret_cast<XMMATRIX*>( this );
	const XMMATRIX& matB = *reinterpret_cast<const XMMATRIX*>( &mOther );

	matA = XMMatrixMultiply( matB, matA );
}

/**
 *	@param mOther The matrix to transform by.
 *	@return A matrix that performs the transforms in mOther then the 
 *			transforms in this matrix.
 */
CFMat4x4	CFMat4x4::GetPreTransformedBy( CFMat4x4Arg mOther ) const
{
	//Create the CFMat4x4 to return.
	CFMat4x4 matReturn;

	const XMMATRIX& matA = *reinterpret_cast<const XMMATRIX*>( this );
	const XMMATRIX& matB = *reinterpret_cast<const XMMATRIX*>( &mOther );
	XMMATRIX& matResult = *reinterpret_cast<XMMATRIX*>( &matReturn );

	matResult = XMMatrixMultiply( matB, matA );

	return matReturn; 
}

/**
 *	Set this matrix to be one that applies the transforms in mOther
 *	followed by the prexisting transforms in this.
 *
 *	@param mOther The matrix to multiply by.
 */
void		CFMat4x4::PostTransformBy( CFMat4x4Arg mOther )
{
	XMMATRIX& matA = *reinterpret_cast<XMMATRIX*>( this );
	const XMMATRIX& matB = *reinterpret_cast<const XMMATRIX*>( &mOther );

	matA = XMMatrixMultiply( matA, matB );
}

/**
 *	@param mOther The matrix to transform by.
 *	@return A matrix that performs the transforms in this matrix then the 
 *			transforms in mOther.
 */
CFMat4x4	CFMat4x4::GetPostTransformedBy( CFMat4x4Arg mOther ) const
{
	//Create the CFMat4x4 to return.
	CFMat4x4 matReturn;

	const XMMATRIX& matA = *reinterpret_cast<const XMMATRIX*>( this );
	const XMMATRIX& matB = *reinterpret_cast<const XMMATRIX*>( &mOther );
	XMMATRIX& matResult = *reinterpret_cast<XMMATRIX*>( &matReturn );

	matResult = XMMatrixMultiply( matA, matB );

	return matReturn; 
}

/**
 *	Transpose the matrix.
 */
void		CFMat4x4::Transpose( void )
{
	XMMATRIX& matA = *reinterpret_cast<XMMATRIX*>( this );
	
	matA = XMMatrixTranspose( matA );
}

/**
 *	Transpose the matrix, and return the result as a new matrix.
 *
 *  @return The resulting matrix.
 */
CFMat4x4	CFMat4x4::GetTransposed( void )
{
	//Create the CFMat4x4 to return.
	CFMat4x4 matReturn;

	const XMMATRIX& matA = *reinterpret_cast<const XMMATRIX*>( this );
	XMMATRIX& matResult = *reinterpret_cast<XMMATRIX*>( &matReturn );

	matResult = XMMatrixTranspose( matA );

	return matReturn;

}

/**
 *	Invert the matrix. 
 *
 *  @return false if there is no inverse.
 */
bool		CFMat4x4::Inverse( void )
{
	//Create a vector to store the determinant.
	XMVECTOR vDeterminant;
	
	//Create a copy of the matrix so we can revert if the operation fails.
	CFMat4x4 matCopy = *this;

	XMMATRIX& matA = *reinterpret_cast<XMMATRIX*>( this );
	XMMATRIX& matTest = *reinterpret_cast<XMMATRIX*>( &matCopy );

	//Get the inverse.
	matTest = XMMatrixInverse( &vDeterminant, matA );

	bool bSuccess = false;
	
	//Test if the operation was successful. Change the value of bSuccess depending on outcome.
	XMMatrixIsInfinite( matTest ) ? bSuccess = false : bSuccess = true;

	//If the operation was successful, copy the inverse into matA.
	bSuccess ? matA = matTest : matA;

	//Return success.
	return bSuccess;
}

/**
 *	Invert the matrix, and return the result as a new matrix.
 *
 *  @param mResult The inverted matrix
 *  @return false if there is no inverse.
 */
bool		CFMat4x4::GetInverse( CFMat4x4& mResult ) const
{
	//Create a vector to store the determinant.
	XMVECTOR vDeterminant;
	
	const XMMATRIX& matA = *reinterpret_cast<const XMMATRIX*>( this );
	XMMATRIX& matResult = *reinterpret_cast<XMMATRIX*>( &mResult );

	//Get the inverse.
	matResult = XMMatrixInverse( &vDeterminant, matA );

	bool bSuccess = false;
	
	//Test if the operation was successful. Change the value of bSuccess depending on outcome.
	XMMatrixIsInfinite( matResult ) ? bSuccess = false : bSuccess = true;

	//Return success.
	return bSuccess;
}

/**
 *	Convert the handedness of the matrix, such that Z=-Z
 */
void		CFMat4x4::ConvertHandedness( void )
{
	// http://vfxbrain.com/questions/1/how-do-i-convert-a-transformation-matrix-from-right-handed-to-left-handed-coordin
	XMVECTOR* pVecs = reinterpret_cast<XMVECTOR*>( this );
	const static XMVECTOR vA = { 1.0f, 1.0f, -1.0f, 1.0f };
	const static XMVECTOR vB = { -1.0f, -1.0f, 1.0f, -1.0f };

	pVecs[0] = XMVectorMultiply( pVecs[0], vA );
	pVecs[1] = XMVectorMultiply( pVecs[1], vA );
	pVecs[2] = XMVectorMultiply( pVecs[2], vB );
	pVecs[3] = XMVectorMultiply( pVecs[3], vA );
}


/**
 *	Transform a three component vector by the current matrix.
 *	Note: This will only perform rotations as it treats the
 *	matrix as only a 3x3 matrix.
 *
 *  @param vVec The vector to transform.
 *  @return The resulting vector.
 */
CFVec3		CFMat4x4::TransformVec3( CFVec3Arg vVec )
{
	CFVec3 v3Return;

	const XMMATRIX& matA = *reinterpret_cast<const XMMATRIX*>( this );
	const XMVECTOR& vArg = *reinterpret_cast<const XMVECTOR*>( &vVec );

	XMVECTOR& vResult = *reinterpret_cast<XMVECTOR*>( &v3Return );

	//Transform vArg by the matrix:
	vResult = XMVector3Transform( vArg, matA );

	return v3Return;
}

/**
 *	Transforms a three component vector by the current matrix
 *	adding in a fourth, implicit w value of one to support
 *	translation.
 *
 *  @param vVec The vector to transform.
 *  @return The resulting vector.
 */
CFVec4		CFMat4x4::TransformVec4( CFVec3Arg vVec )
{
	CFVec4 v4Return;

	const XMMATRIX& matA = *reinterpret_cast<const XMMATRIX*>( this );
	const XMVECTOR& vArg = *reinterpret_cast<const XMVECTOR*>( &vVec );

	XMVECTOR& vResult = *reinterpret_cast<XMVECTOR*>( &v4Return );

	//Transform vArg by the matrix:
	vResult = XMVector3Transform( vArg, matA );

	return v4Return;
}

/**
 *	Transforms a four component vector by the current matrix.
 *
 *  @param vVec The vector to transform.
 *  @return The resulting vector.
 */
CFVec4		CFMat4x4::TransformVec4( CFVec4Arg vVec )
{
	CFVec4 v4Return;

	const XMMATRIX& matA = *reinterpret_cast<const XMMATRIX*>( this );
	const XMVECTOR& vArg = *reinterpret_cast<const XMVECTOR*>( &vVec );

	XMVECTOR& vResult = *reinterpret_cast<XMVECTOR*>( &v4Return );

	//Transform vArg by the matrix:
	vResult = XMVector4Transform( vArg, matA );

	return v4Return;
}

/**
 *	Create a translation matrix from three floats.
 *
 *  @param fX The x position.
 *  @param fY The y position.
 *  @param fZ The z position.
 *  @return The resulting matrix.
 */
CFMat4x4	CFMat4x4::CreateTranslation( FLOAT32 fX, FLOAT32 fY, FLOAT32 fZ )
{
	CFMat4x4 matReturn;

	XMMATRIX& matResult = *reinterpret_cast<XMMATRIX*>( &matReturn );

	matResult = XMMatrixTranslation( fX, fY, fZ );

	return matReturn;
}

/**
 *	Create a translation matrix for a given position.
 *
 *  @param vPosition The position expressed as a CFVec3.
 *  @return The resulting matrix.
 */
CFMat4x4	CFMat4x4::CreateTranslation( CFVec3Arg vPosition )
{
	CFMat4x4 matReturn;

	XMMATRIX& matResult = *reinterpret_cast<XMMATRIX*>( &matReturn );
	const XMVECTOR& vArg = *reinterpret_cast<const XMVECTOR*>( &vPosition );

	matResult = XMMatrixTranslationFromVector( vArg );

	return matReturn;
}

/**
 *	Create a scale matrix from three floats.
 *
 *  @param fX The amount to scale x by.
 *  @param fY The amount to scale y by.
 *  @param fZ The amount to scale z by.
 *  @return The resulting matrix.
 */
CFMat4x4	CFMat4x4::CreateScale( FLOAT32 fX, FLOAT32 fY, FLOAT32 fZ )
{
	CFMat4x4 matReturn;

	XMMATRIX& matResult = *reinterpret_cast<XMMATRIX*>( &matReturn );

	matResult = XMMatrixScaling( fX, fY, fZ );

	return matReturn;
}

/**
 *	Creates a rotation matrix from Euler angles expressed as radians.
 *	Perform roll, then pitch then yaw.
 *	Angles are clockwise when looking along the axis towards the origin.
 *
 *  @param fYaw Yaw, in radians.
 *  @param fPitch Pitch, in radians.
 *  @param fRoll Roll, in radians.
 *  @return The resulting matrix.
 */
CFMat4x4	CFMat4x4::CreateYawPitchRoll( FLOAT32 fYaw, FLOAT32 fPitch, FLOAT32 fRoll )
{
	CFMat4x4 matReturn;

	XMMATRIX& matResult = *reinterpret_cast<XMMATRIX*>( &matReturn );

	matResult = XMMatrixRotationRollPitchYaw( fPitch, fYaw, fRoll );

	return matReturn;
}

/**
 *	Creates a rotation matrix about the X axis.
 *	Angles is clockwise when looking along the axis towards the origin.
 *
 *  @param fRadians The angle to rotate by.
 *  @return The resulting matrix.
 */
CFMat4x4	CFMat4x4::CreateRotationX( FLOAT32 fRadians )
{
	CFMat4x4 matReturn;

	XMMATRIX& matResult = *reinterpret_cast<XMMATRIX*>( &matReturn );

	matResult = XMMatrixRotationX( fRadians );

	return matReturn;
}

/**
 *	Creates a rotation matrix about the Y axis.
 *	Angles is clockwise when looking along the axis towards the origin.
 *
 *  @param fRadians The angle to rotate by.
 *  @return The resulting matrix.
 */
CFMat4x4	CFMat4x4::CreateRotationY( FLOAT32 fRadians )
{
	CFMat4x4 matReturn;

	XMMATRIX& matResult = *reinterpret_cast<XMMATRIX*>( &matReturn );

	matResult = XMMatrixRotationY( fRadians );

	return matReturn;
}

/**
 *	Creates a rotation matrix about the Z axis.
 *	Angles is clockwise when looking along the axis towards the origin.
 *
 *  @param fRadians The angle to rotate by.
 *  @return The resulting matrix.
 */
CFMat4x4	CFMat4x4::CreateRotationZ( FLOAT32 fRadians )
{
	CFMat4x4 matReturn;

	XMMATRIX& matResult = *reinterpret_cast<XMMATRIX*>( &matReturn );

	matResult = XMMatrixRotationZ( fRadians );

	return matReturn;
}

/**
 *	Creates a rotation matrix about the described axis.
 *
 *  @param fX Describes the x component of the axis.
 *  @param fY Describes the y component of the axis.
 *  @param fZ Describes the z component of the axis.
 *  @param fRadians Angle of rotation, measured clockwise when looking along the rotation axis.
 *  @return The resulting matrix.
 */
CFMat4x4	CFMat4x4::CreateRotationAxis( FLOAT32 fX, FLOAT32 fY, FLOAT32 fZ, FLOAT32 fRadians )
{
	CFMat4x4 matReturn;
	CFVec3 v3Axis;

	v3Axis.X( fX );
	v3Axis.Y( fY );
	v3Axis.Z( fZ );

	XMMATRIX& matResult = *reinterpret_cast<XMMATRIX*>( &matReturn );
	XMVECTOR& vAxis = *reinterpret_cast<XMVECTOR*>( &v3Axis );

	vAxis = XMVectorSetW( vAxis, 0.0f );

	matResult = XMMatrixRotationAxis( vAxis, fRadians );

	return matReturn;
}

/**
 *	Creates a projection matrix.
 *
 *  @param fFOV The horizontal field of view, in radians.
 *  @param fAspect The aspect ratio (width/height).
 *  @param fNear The near clip plane.
 *  @param fFar The far clip plane.
 *  @return The resulting matrix.
 */
CFMat4x4	CFMat4x4::CreateProjection( FLOAT32 fFOV, FLOAT32 fAspect, FLOAT32 fNear, FLOAT32 fFar )
{
	//Assume a LH perspective matrix

	CFMat4x4 matReturn;

	XMMATRIX& matResult = *reinterpret_cast<XMMATRIX*>( &matReturn );

	matResult = XMMatrixPerspectiveFovLH( fFOV, fAspect, fNear, fFar );

	return matReturn;
}

/**
 *	Create an orthographic projection matrix (left handed)
 *
 *	@param fViewWidth	The width of the view.
 *	@param fViewHeight	The height of the view.
 *	@param fNearZ		The near Z plane distance.
 *	@param fFarZ		The far Z plane distance.
 */
CFMat4x4	CFMat4x4::CreateOrtho( FLOAT32 fViewWidth, FLOAT32 fViewHeight, FLOAT32 fNearZ, FLOAT32 fFarZ )
{
	CFMat4x4 matReturn;
	XMMATRIX& matResult = *reinterpret_cast<XMMATRIX*>( &matReturn );

	matResult = XMMatrixOrthographicLH( fViewWidth, fViewHeight, fNearZ, fFarZ );

	return matReturn;
}

/**
 *	Create view matrix using position, direction and up vectors
 *
 *	@param vPos Position of the camera.
 *	@param vDir View direction for the camera to look along.
 *	@param vUp A vector that describes the up direction of the camera.
 *	@return View matrix
 */
CFMat4x4 CFMat4x4::CreateViewDirection(CFVec3Arg vPos, CFVec3Arg vDir, CFVec3Arg vUp)
{
	CFMat4x4 mOut;
	XMMATRIX& rmOut = *reinterpret_cast<XMMATRIX*>(&mOut);
	rmOut = XMMatrixLookToLH(*reinterpret_cast<const XMVECTOR*>(&vPos),
		*reinterpret_cast<const XMVECTOR*>(&vDir),
		*reinterpret_cast< const XMVECTOR*>(&vUp));
	return mOut;
}

/**
 *	Create view matrix using position, target and up vectors
 *
 *	@param vPos Position of the camera.
 *	@param vTarget Target for the camera to look at.
 *	@param vUp A vector that describes the up direction of the camera.
 *	@return View matrix
 */
CFMat4x4 CFMat4x4::CreateViewLookAt(CFVec3Arg vPos, CFVec3Arg vTarget, CFVec3Arg vUp)
{
	CFMat4x4 mOut;
	XMMATRIX& rmOut = *reinterpret_cast<XMMATRIX*>(&mOut);
	rmOut = XMMatrixLookAtLH(*reinterpret_cast<const XMVECTOR*>(&vPos),
		*reinterpret_cast<const XMVECTOR*>(&vTarget),
		*reinterpret_cast<const XMVECTOR*>(&vUp));
	return mOut;
}

} // end namespace GDE

