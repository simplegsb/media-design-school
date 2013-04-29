/**
 *	@file Mandelbrot/Headers/MandelbrotBuffer.h
 *	@brief
 *
 *	Copyright()
 *		Copyright © 2009-2010 Media Design School. All Rights Reserved.
 *
 *	@author Jon Booth
 *
 *********************************************************************************************/

#ifndef _MANDELBROTBUFFER_H__Included_79333008
#define _MANDELBROTBUFFER_H__Included_79333008

/**
 *	This constructs the madelbrot image.
 *
 *	@author Jon Booth.
 */
class CMandelbrotBuffer
{
public:
	typedef GDE::UINT32 UINT32;
	typedef GDE::FLOAT32 FLOAT32;
	CMandelbrotBuffer( UINT32 uWidth, UINT32 uHeight, UINT32 uTexWidth, UINT32 uTexHeight );
	~CMandelbrotBuffer( void );

	void Clear();
	void SetMaxIterations( UINT32 uNumIterations );
	void Fill( void );
	void FillPartial( UINT32 uDivisions, UINT32 uDivision );

	const UINT32& TexelAt( UINT32 uX, UINT32 uY ) const
	{
		return m_pBuffer[uY*m_uTexWidth + uX];
	}

	UINT32& TexelAt( UINT32 uX, UINT32 uY )
	{
		return m_pBuffer[uY*m_uTexWidth + uX];
	}


	const UINT32* GetPixels( void ) const
	{
		return m_pBuffer;
	}
protected:
	void Fill( FLOAT32 fX, FLOAT32 fY, FLOAT32 fWidth, FLOAT32 fHeight,
		UINT32 fxStart, UINT32 fxRange, UINT32 fyStart, UINT32 fyRange );
	UINT32 CalculateForPoint( FLOAT32 fX, FLOAT32 fY ) const;

	static UINT32 HSVMake( FLOAT32 fH, FLOAT32 fS, FLOAT32 fV );
	static UINT32 MakeColF( FLOAT32 fR, FLOAT32 fG, FLOAT32 fB );

	UINT32	m_uMaxIterations;
	UINT32*	m_pPalette;
	UINT32	m_uWidth;
	UINT32	m_uHeight;

	// for the underlying texture.
	UINT32	m_uTexWidth;
	UINT32	m_uTexHeight;
	UINT32*	m_pBuffer;

	// below are the parameters for the x, y, width and height...
	FLOAT32 m_fX;
	FLOAT32 m_fY;
	FLOAT32 m_fWidth;
	FLOAT32 m_fHeight;
};


#endif //_MANDELBROTBUFFER_H__Included_79333008

