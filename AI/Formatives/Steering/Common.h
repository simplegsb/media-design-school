/**
 *	@file QuadTree\Common.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _COMMON_H__Included_5303354
#define _COMMON_H__Included_5303354

#include "GDE/Core/Core.h"
#include "GDE/Core/Logger.h"
typedef GDE::UINT64 UINT64;
typedef GDE::SINT64 SINT64;
typedef GDE::UINT32 UINT32;
typedef GDE::SINT32 SINT32;
typedef GDE::UINT16 UINT16;
typedef GDE::SINT16 SINT16;
typedef GDE::UINT8 UINT8;
typedef GDE::SINT8 SINT8;
typedef GDE::FLOAT32 FLOAT32;
typedef GDE::FLOAT64 FLOAT64;
const static FLOAT32 fEPSILON = 0.00000001f;
const static FLOAT32 fPI = 3.141592653589793f;

inline FLOAT32 fDegsToRad( FLOAT32 fDegs )
{
	return (fDegs/180.0f)*fPI;
}

inline FLOAT32 fRadsToDegs( FLOAT32 fRads )
{
	return (fRads/fPI)*180.0f;
}

#endif //_COMMON_H__Included_5303354

