/**
 *	@file GDE\Core\MemoryAlignment.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _MEMORYALIGNMENT_CPP__Included_1037504631
#define _MEMORYALIGNMENT_CPP__Included_1037504631
#include "Core.h"
#include "MemoryAlignment.h"

//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
#include <stdlib.h>
#ifdef __GNUC__
# include <malloc.h>
#endif

#ifdef _PS2_
// quick and dirty hack - no array new and delete appear to be defined!
void* operator new[] (size_t NumBytes)
{
	return malloc(NumBytes);
}

void operator delete[]( void* pData )
{
	free( pData );
}

void* operator new( size_t NumBytes )
{
	return malloc( NumBytes );
}

void operator delete( void* pData )
{
	free( pData );
}

// PS2 also needs something to do when a pure virtual is called (should never happen...)
extern "C" void __cxa_pure_virtual( void )
{
	assert_now( "A pure virtual function was called... wow!");
}
#endif

namespace GDE
{

/**
 *	@ingroup Core
 *	Allocate memory alligned on a certain boundary.
 *	
 *	@param NumBytes		The number of bytes to allocate.
 *	@param Alignment	The number of bytes to align to.
 *	@see FreeAligned.
 */
void*	AllocAligned( size_t NumBytes, size_t Alignment )
{
#ifndef __GNUC__
	return _aligned_malloc( NumBytes, Alignment );
#else
	return memalign( Alignment, NumBytes );
#endif
}

/**
 *	@ingroup Core
 *
 *	@param pMem Memory to free that was previously allocated with AllocAligned.
 */
void	FreeAligned( void* pMem )
{
#ifndef __GNUC__
	return _aligned_free( pMem );
#else
	return free( pMem );
#endif
}

} // end namespace GDE


#endif //_MEMORYALIGNMENT_CPP__Included_1037504631

