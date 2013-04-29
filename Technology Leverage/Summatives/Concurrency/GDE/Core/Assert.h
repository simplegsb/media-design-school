/**
 *	@file GDE\Core\Assert.h
 *	@brief Assertation functionality.
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASSERT_H__Included_917084036
#define _ASSERT_H__Included_917084036

#include "Bridge.h"

#ifdef _PS2_
# define __FUNCTION__ "FuncUnknown"
#endif
namespace GDE
{

class IAssertQuery;
/**
 *	@ingroup Core
 *	This class allows us to handle all asserts.
 *	It starts with some default implementations, however these can be overriden
 *	so, for example, on PSP/IPhone the assert is sent to the console screen
 *	as opening a window is a pain. Another alternative would be to implement
 *	a networked assert.
 *
 *	This class is a Meyers singleton as we only ever want a single assert handler.
 */
class CAssertHandler : public TBridge <IAssertQuery, CREATE_DLL_ID( 'A','S','R','T' )>
{
public:
	typedef TBridge<IAssertQuery, CREATE_DLL_ID( 'A','S','R','T' )> TSuperClass; //!< Convenience typedef
	bool Assert( const char* strMsg, UINT32 uLine, const char* strFileName, const char* strFunction, bool& bSkip );
	bool MessageAssert( 
		const char* strExpression,
		UINT32 uLine,
		const char* strFileName, 
		const char* strFunction,
		bool& bSkip,
		const char* strMessageFmt,
		...
		);

	static CAssertHandler& GetInstance( void );

	void	SetAssertQuery( IAssertQuery* pQuery );
	IAssertQuery*	GetAssertQuery( void );
protected:
	CAssertHandler( IAssertQuery* pQuery );
};

	/*
void RegesterAssertQuery( IAssertQuery* pQuery );
IAssertQuery* GetAssertQuery( void );
*/

#ifndef _TOSTR
# define _XSTR( arg ) #arg
# define _TOSTR( arg ) _XSTR( arg )
# define warnmsg(arg) message( __FILE__ "(" _TOSTR(__LINE__) ") : " arg )
#endif

#ifdef _DEBUG

#ifdef _PS2_
extern void DebugBreakFunc( void );
#endif

// please never use DEBUG_BREAK in your code - use assert instead..
# ifdef _MSC_VER
//#   define DEBUG_BREAK DebugBreak()
#  define DEBUG_BREAK _asm{ int 3 };
# else
#  ifdef _IPHONE_DEVICE
#   define DEBUG_BREAK asm { trap };
#  else
#   ifdef _PS2_
//#    define DEBUG_BREAK asm __volatile__("break 1") // nice PS2 debugger break. Maybe works on PSP too!
#    define DEBUG_BREAK GDE::DebugBreakFunc()
#  else
#    define DEBUG_BREAK asm ( "int3;" )
#  endif
# endif
#endif
// you can override this and provide your own implementation in a platform

#ifndef _PS2_
inline void PleasePutSemiColonAfterAssert( void ) {} // intentionally empty
#else
inline void PleasePutSemiColonAfterAssert( void ) {} // intentionally empty
#endif

#ifdef assert
# warning "Removing previous assert definition" // XCode?
# undef assert
#endif

/**
 *	In debug assert if expr is false.
 */
#define assert( expr ) 											\
{																\
	static bool bSkip = false; 									\
	if ( 0 == (expr) )											\
	{															\
		if ( GDE::CAssertHandler::GetInstance().Assert(			\
			_TOSTR( expr ), __LINE__, __FILE__,	__FUNCTION__,	\
			bSkip ) )											\
		{														\
			DEBUG_BREAK;										\
		}														\
	}															\
} GDE::PleasePutSemiColonAfterAssert()

/**
 *	In debug assert if expr is false, with the printf formatted message as the following arguments.
 */
# define assert_msg( expr, ... )								\
{																\
	static bool bSkip = false; 									\
	if ( 0 == (expr) )											\
	{															\
	if ( GDE::CAssertHandler::GetInstance().MessageAssert(		\
			_TOSTR( expr ), __LINE__, __FILE__,	__FUNCTION__ ,	\
			bSkip, __VA_ARGS__ ) )								\
		{														\
			DEBUG_BREAK;										\
		}														\
	}															\
} GDE::PleasePutSemiColonAfterAssert()

/**
 *	In debug always assert with the printf formatted message passed in.
 */
# define assert_now( ... )										\
{																\
	static bool bSkip = false; 									\
	if ( GDE::CAssertHandler::GetInstance().MessageAssert(		\
			"", __LINE__, __FILE__,	__FUNCTION__,				\
			bSkip, __VA_ARGS__ ) )								\
	{															\
		DEBUG_BREAK;											\
	}															\
} GDE::PleasePutSemiColonAfterAssert()

/**
 *	In debug assert if the result of arg is false, in release just do arg.
 */
# define validate( arg ) assert( arg )
/**
 *	In debug assert if the result of arg is false, using the printf formatted message supplied, in release just do arg.
 */
# define validate_msg( arg, ... ) assert_msg( arg, __VA_ARGS__ )
#else

/**
 *	In debug assert if expr is false.
 */
# define assert( arg )
/**
 *	In debug assert if expr is false, with the printf formatted message as the following arguments.
 */
# define assert_msg( arg, ... )
/**
 *	In debug always assert with the printf formatted message passed in.
 */
# define assert_now( ... )
/**
 *	In debug assert if the result of arg is false, in release just do arg.
 */
# define validate( arg ) arg
/**
 *	In debug assert if the result of arg is false, using the printf formatted message supplied, in release just do arg.
 */
# define validate_msg( arg, ... ) arg
#endif


} // end namespace GDE

#endif //_ASSERT_H__Included_917084036

