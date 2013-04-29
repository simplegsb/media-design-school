/**
 *	@file Mandelbrot/MandelbrotBuffer.cpp
 *	@brief
 *
 *	Copyright()
 *		Copyright © 2009-2010 Media Design School. All Rights Reserved.
 *
 *	@author Jon Booth
 *
 *********************************************************************************************/
#include "GDE/Core/Core.h"
#include "MandelbrotBuffer.h"

#include <cmath>
#include <cstring>

/**
 * Construct and fill the Madelbrot buffer.
 *
 *	@author Jon Booth
 */
CMandelbrotBuffer::CMandelbrotBuffer( UINT32 uWidth, UINT32 uHeight, UINT32 uTexWidth, UINT32 uTexHeight )
: m_uMaxIterations( 0 ) // is filled in later
, m_pPalette( NULL )
, m_uWidth( uWidth )
, m_uHeight( uHeight )
, m_uTexWidth( uTexWidth )
, m_uTexHeight( uTexHeight )
, m_fX( -2.0f )
, m_fY( -1.0f )
, m_fWidth( 3.0f )
, m_fHeight( 2.0f )
{
	m_pBuffer = new UINT32[uTexWidth*uTexHeight];
	// build the palette at the highest point you can.
	SetMaxIterations( 256 );
	//Fill();
}

CMandelbrotBuffer::~CMandelbrotBuffer( void )
{
	GDE::SafeDelete ( m_pPalette );
	GDE::SafeDelete( m_pBuffer );
}

void CMandelbrotBuffer::Clear()
{
	memset(m_pBuffer, 0, (UINT32) (m_fWidth * m_fHeight * sizeof(UINT32)));
}

/**
 *	Set the maximum number of iterations and build the palette for colours from that.
 *
 *	@author Jon Booth
 */
void CMandelbrotBuffer::SetMaxIterations( UINT32 uNumIterations )
{
	if ( uNumIterations == m_uMaxIterations )
	{
		return;
	}
	m_uMaxIterations = uNumIterations;
	if ( NULL != m_pPalette )
	{
		delete[] m_pPalette;
	}
	m_pPalette = new UINT32[m_uMaxIterations+1];
	static FLOAT32 fSat = 1.0f;
	static FLOAT32 fVal = 1.0f;
	const FLOAT32 fIterRatio = 1.0f/FLOAT32(m_uMaxIterations);
	for ( UINT32 i=0; i<m_uMaxIterations; i++ )
	{
		m_pPalette[i] = HSVMake( FLOAT32(i) * fIterRatio, fSat, fVal );
	}
	m_pPalette[m_uMaxIterations] =  MakeColF( 0.0f, 0.0f, 0.0f ); // black
}

/**
 *	Fill for the current window onto the Mandelbrot plane.
 *
 *	@author Jon Booth
 */
void CMandelbrotBuffer::Fill( void )
{
	Fill( m_fX, m_fY, m_fWidth, m_fHeight, 0, m_uWidth, 0, m_uHeight );
}

/**
 *	Fill for the given window onto the Mandelbrot plane.
 *
 *	@author Jon Booth
 */
void CMandelbrotBuffer::Fill( FLOAT32 fX, FLOAT32 fY, FLOAT32 fWidth, FLOAT32 fHeight,
							 UINT32 fxStart, UINT32 fxRange, UINT32 fyStart, UINT32 fyRange )
{
	// x = -0.695f;
	// y = -0.469f;
	// scale 0.00008f; // 1280x1024
	const FLOAT32 fXRatio = fWidth/FLOAT32( m_uWidth );
	const FLOAT32 fYRatio = fHeight/FLOAT32( m_uHeight );
//	const FLOAT32 fIterRatio = 1.0f/FLOAT32(m_uMaxIterations);
	UINT32 uIterations;
	for ( UINT32 y = fyStart; y < fyStart + fyRange; y++ )
	{
		for ( UINT32 x = fxStart; x < fxStart + fxRange; x++ )
		{
			uIterations = CalculateForPoint( fX+FLOAT32(x)*fXRatio, fY+FLOAT32(y)*fYRatio );
//			static FLOAT32 fSat = 1.0f;
//			static FLOAT32 fVal = 1.0f;
//			if ( uIterations == m_uMaxIterations )
//			{
//				TexelAt( x, y ) = MakeColF( 0.0f, 0.0f, 0.0f ); // black
//			} else
//			{
//				TexelAt( x, y ) = HSVMake( FLOAT32(uIterations) * fIterRatio, fSat, fVal );
////				TexelAt( x, y ) = MakeColF( FLOAT32(uIterations*255) * fIterRatio, 255, 255 );
//			}
			// optmise:
			TexelAt( x, y ) = m_pPalette[uIterations];
		}
	}
}

void CMandelbrotBuffer::FillPartial( UINT32 uDivisions, UINT32 uDivision )
{
	UINT32 uYRange = m_uHeight / uDivisions;
	UINT32 uYStart = uDivision * uYRange;
	if (uDivision == uDivisions - 1)
	{
		uYRange = m_uHeight - uYStart;
	}

	Fill( m_fX, m_fY, m_fWidth, m_fHeight, 0, m_uWidth, uYStart, uYRange );
}

/**
 *	Create a colour from a given Hue Saturation and Value.
 *
 *	@author Jon Booth
 */
GDE::UINT32 CMandelbrotBuffer::HSVMake( FLOAT32 fH, FLOAT32 fS, FLOAT32 fV )
{
	 // H is given on [0, 1] or UNDEFINED. S and V are given on [0, 1].
	fH *= 6.0f;
	// RGB are each returned on [0, 1].
	FLOAT32 m, n, f;
	int i;
	
	i = int(fH);
	f = fH - i;
	if ( !(i&1) )
	{ 
		f = 1.0f - f; // if i is even
	}
	m = fV * (1.0f - fS);
	n = fV * (1.0f - fS * f);
	switch (i) 
	{
		case 6:
		case 0: return MakeColF(fV, n, m);
		case 1: return MakeColF(n, fV, m);
		case 2: return MakeColF(m, fV, n);
		case 3: return MakeColF(m, n, fV);
		case 4: return MakeColF(n, m, fV);
		case 5: return MakeColF(fV, m, n);
	}
	return MakeColF( fV, fV, fV );
}

/**
 *	Create a colour from floating point Red, Green and Blue values.
 *
 *	@author Jon Booth
 */
GDE::UINT32 CMandelbrotBuffer::MakeColF( FLOAT32 fR, FLOAT32 fG, FLOAT32 fB )
{
	return 0xFF000000|(UINT32(fR*255.0f)<<16)|(UINT32(fG*255.0f)<<8)|UINT32(fB*255.0f);
}


/**
 *	Calculate a given number of iterations for a given point
 */
GDE::UINT32 CMandelbrotBuffer::CalculateForPoint( FLOAT32 fX, FLOAT32 fY ) const
{
	const FLOAT32 fX0 = fX;
	const FLOAT32 fY0 = fY;
	fX = fY = 0.0f;
	FLOAT32 fT;
	for ( UINT32 i=0; i<m_uMaxIterations; i++ )
	{
		if ( fX*fX+fY*fY > 4.0f )
		{
			return i;
		}
		fT = fX*fX - fY*fY + fX0;
		fY = 2.0f*fX*fY + fY0;
		fX = fT;
	}
	return m_uMaxIterations;
}
