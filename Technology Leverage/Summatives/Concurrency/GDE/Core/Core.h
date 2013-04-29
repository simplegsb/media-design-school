/**
 *	@file GDE\Core\Core.h
 *	@brief 
 *	This should always be included first, it is the primary header for the	
 *	GDE (Games Development Engine) library.
 *
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _CORE_H__Included_925621900
#define _CORE_H__Included_925621900

// figure out our platform, currently _PS2_/_PSP_/_MSWIN_

// __SNC__ is also defined for the PSP.

// this command will tell us all preprocessor macros: gcc -dM -E - < /dev/null
// or use ee-g++ for the c++ ee compiler!

#ifdef Release
// Release is a keyword used often - stuffed if defined as it is the configuration!
# undef Release
# define _Release
#endif

#ifdef __ee__
# ifndef _PS2_ // may already be defined in the makefile!
#  define _PS2_
# endif
#else
# ifdef __GNUC__
#  define _PSP_
# endif
#endif

#ifdef _PS2_
# define _SONY_
#endif
#ifdef _PSP_
# define _SONY_
#endif


#ifndef _SONY_
# define _MSWIN_ // don't use _WINDOWS_ that breaks stuff.
#endif
// end of platform calculation.

#ifndef __GNUC__ // don't use for GNU platforms - TODO: a platform detection file specific to our purposes
# include "VLD/VLD.h" // note you need to have the VLD/ folder on additional to make this work....
#else
# define MAX_PATH 512 // big enough :-)
// hack around the ms things:
# define _snprintf_s( strDest, size, size2, ... ) snprintf( (strDest), (size), __VA_ARGS__ )
# define vsnprintf_s( strDest, size, size2, fmt, ap ) vsnprintf( (strDest), (size), fmt, ap )
# define sprintf_s(buffer, buffer_size, ...) snprintf((buffer), (buffer_size), __VA_ARGS__)
# define _stricmp strcasecmp
#endif

#ifdef __GNUC__
# define __cdecl	// remove this as it's a MSVC thing.
#else
# define _SUPPORT_DLLS
#endif

// detect exceptions
#ifdef Debug_Exceptions
# define _GDE_ENABLE_EXCEPTIONS
#endif
#ifdef Release_Exceptions
# define _GDE_ENABLE_EXCEPTIONS
#endif

#ifndef _GDE_ENABLE_EXCEPTIONS
///////////////////////////////////////////////////////////////////////
// Disable exceptions from the standard libraries under visual studio
# define _HAS_EXCEPTIONS 0
# define _STATIC_CPPLIB
///////////////////////////////////////////////////////////////////////
#endif

//! this won't compile unless the expression is true.
#define _COMPILE_ASSERT( expr )	typedef char __CASSERT[ (expr) ? 1 : -1 ];
//! returns the number of elements of a static array
#define _ARRAY_SIZE( arg ) ( sizeof( arg )/sizeof( (arg)[0] ) )

//! hide the warning associated with an unused variable (allows searching later)
#define UNUSED_VAR( arg ) (void)arg

// useful to create warnings in visual studio - @pragma warnmsg will give a good warning.
#define _XSTR( arg ) #arg
#define _TOSTR( arg ) _XSTR( arg )
#define warnmsg(arg) message( __FILE__ "(" _TOSTR(__LINE__) ") : " arg )

#ifdef GCC
//! to tell the compiler to generate warnings for inappropriate printf arguments.
# define _PRINTF_FMT( FormatPos, ArgStart ) __attribute__ ((format (printf, FormatPos, ArgStart)))
//! to deprecate a function (should be able to cross compiler setup)?
# define DEPRECATED __attribute__((__deprecated__))
#else
//! to tell the compiler to generate warnings for inappropriate printf arguments.
# define _PRINTF_FMT( FormatPos, ArgStart )
//! to deprecate a function (should be able to cross compiler setup)?
# define DEPRECATED
#endif

#include "GDETypes.h"
#include "Assert.h"

namespace GDE
{

/**
 *	@ingroup Core
 *
 *	Delete an object and set the pointer to NULL - only if the object is non-null.
 *
 *	@tparam		t The type of object to delete.
 *	@param		p The object to delete.
 */
template <typename t> void SafeDelete( t* &p )
{
	if ( NULL != p )
	{
		delete p;
		p = NULL;
	}
}

/**
 *	@ingroup Core
 *
 *	Delete an object and set the pointer to NULL - only if the object is non-null.
 *
 *	@tparam		t The type of object to delete.
 *	@param		p The object to delete.
 */
template <typename t> void SafeArrayDelete( t* &p )
{
	if ( NULL != p )
	{
		delete[] p;
		p = NULL;
	}
}


/**
 *	@ingroup Core
 *
 *	Release an object and set the pointer to NULL - only if the object is non-null.
 *
 *	@tparam		t The type of object to release.
 *	@param		p	The object to release.
 */
template <typename t> void SafeRelease( t* &p )
{
	if ( NULL != p )
	{
		p->Release();
		p = NULL;
	}
}

} // end namespace GDE

#endif //_CORE_H__Included_925621900

