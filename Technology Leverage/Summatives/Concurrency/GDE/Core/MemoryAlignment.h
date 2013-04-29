/**
 *	@file GDE\Core\MemoryAlignment.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _MEMORYALIGNMENT_H__Included_1957512289
#define _MEMORYALIGNMENT_H__Included_1957512289

#ifdef __GNUC__
# include <stddef.h>	// for size_t
#endif

#define _ALIGNED_PREFIX( x ) _declspec( align( x ) )

namespace GDE
{
	void*	AllocAligned( size_t NumBytes, size_t Alignment );
	void	FreeAligned( void* pMem );


	/**
	 *	@ingroup Core
	 *
	 *	Templated class to provide aligned new and delete operators.
	 *	Can be inherited for classes that require.
	 */
	template <int iAlignment> class TAligned
	{
	public:
		void* operator new( size_t NumBytes )
		{
			return AllocAligned( NumBytes, iAlignment );
		}
		void* operator new[]( size_t NumBytes )
		{
			return AllocAligned( NumBytes, iAlignment );
		}
		void operator delete( void* pMem )
		{
			FreeAligned( pMem );
		}
		void operator delete[]( void* pMem )
		{
			FreeAligned( pMem );
		}
	};

} // end namespace GDE

#endif //_MEMORYALIGNMENT_H__Included_1957512289

