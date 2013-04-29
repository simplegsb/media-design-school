/**
 *	@file GDE\Core\GameTimer.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Core.h"
#include "GameTimer.h"
#include "Logger.h"

#include <math.h> // sinf

#ifndef __GNUC__
# define WIN32_LEAN_AND_MEAN
# include <Windows.h>
// for sleep and timeval we need
# include <winsock.h>
# include <time.h>
#else
// According to POSIX.1-2001
//# include <sys/select.h>
// According to earlier standards
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>
#endif

using namespace GDE;

//////// High performance timer - doesn't work... ////
//// http://www.codeguru.com/cpp/w-p/system/timers/article.php/c5759/
////----------------------------------------------------------------
//class CPreciseTimer
//{
//public:
//   CPreciseTimer() : mRes(0), toLeave(false), stopCounter(-1)
//   {
//      InitializeCriticalSection(&crit);
//      mRes = timeSetEvent(1, 0, &TimerProc, (DWORD)this,
//                          TIME_PERIODIC);
//   }
//   virtual ~CPreciseTimer()
//   {
//      mRes = timeKillEvent(mRes);
//      DeleteCriticalSection(&crit);
//   }
//   
//   ///////////////////////////////////////////////////////////////
//   // Function name   : Wait
//   // Description     : Waits for the required duration of msecs.
//   //                 : Timer resolution is precisely 1 msec
//   // Return type     : void  :
//   // Argument        : int timeout : timeout in msecs
//   ///////////////////////////////////////////////////////////////
//   void Wait(int timeout)
//   {
//      if ( timeout )
//      {
//         stopCounter = timeout;
//         toLeave = true;
//         // this will do the actual delay - timer callback shares
//         // same crit section
//         EnterCriticalSection(&crit);
//         LeaveCriticalSection(&crit);
//      }
//   }
//   ///////////////////////////////////////////////////////////////
//   // Function name   : TimerProc
//   // Description     : Timer callback procedure that is called
//   //                 : every 1msec
//   //                 : by high resolution media timers
//   // Return type     : void CALLBACK  :
//   // Argument        : UINT uiID :
//   // Argument        : UINT uiMsg :
//   // Argument        : DWORD dwUser :
//   // Argument        : DWORD dw1 :
//   // Argument        : DWORD dw2 :
//   ///////////////////////////////////////////////////////////////
//   static void CALLBACK TimerProc(UINT /*uiID*/, UINT /*uiMsg*/, DWORD
//                                  dwUser, DWORD /*dw1*/, DWORD /*dw2*/)
//   {
//      static volatile bool entered = false;
//      
//      CPreciseTimer* pThis = (CPreciseTimer*)dwUser;
//      if ( pThis )
//      {
//         if ( !entered && !pThis->toLeave )   // block section as
//                                              // soon as we can
//         {
//            entered = true;
//            EnterCriticalSection(&pThis->crit);
//         }
//         else if ( pThis->toLeave && pThis->stopCounter == 0 )
//                                              // leave section
//                                              // when counter
//                                              // has expired
//         {
//            pThis->toLeave = false;
//            entered = false;
//            LeaveCriticalSection(&pThis->crit);
//         }
//         else if ( pThis->stopCounter > 0 )   // if counter is set
//                                              // to anything, then
//                                              // continue to drop
//                                              // it...
//            --pThis->stopCounter;
//      }
//   }
//   
//private:
//   MMRESULT         mRes;
//   CRITICAL_SECTION crit;
//   volatile bool    toLeave;
//   volatile int     stopCounter;
//};
//
//static CPreciseTimer gs_PreciseTimer;

/*
//----------------------------------------------------------------
// Class usage example
//----------------------------------------------------------------
void Delay(unsigned int val)
{
   static LARGE_INTEGER freq = {0};
   static double average = 0;
   static int count = 0;

   ++count;
   LARGE_INTEGER iStart, iStop;
   if ( freq.QuadPart == 0 )
      QueryPerformanceFrequency(&freq), freq.QuadPart /= 1000;
                      // convert to msecs counter

   double sleep = 0;
   QueryPerformanceCounter(&iStart);

   timer.Wait(val); // is there anything to wait on? ... then wait

   QueryPerformanceCounter(&iStop);
   sleep = ((double)iStop.QuadPart - (double)iStart.QuadPart)
                                   / (double)freq.QuadPart;
   average += (val ? 100.0*(sleep-val)/(double)val : 0);
   printf("Waited for %6.3f (%ld). error = %5.2f\n", sleep, val,
                                   average/(double)count);
}
*/
///////////////////

/**
 *	Update to the current time.
 */
bool CSystemTime::Update( void )
{
	/* need the performance counter
#ifndef _MSC_VER
	timeval Time;
	if ( 0 == gettimeofday( &Time, NULL ) )
	{
		m_uSeconds = Time.tv_sec;
		m_uMicroSeconds = Time.tv_usec;
		return true;
	}
	return false;
#else
	GDE::UINT32 uTicks = GetTickCount();
	m_uSeconds = uTicks/1000;
	m_uMicroSeconds = (uTicks%1000)*1000;
	return true;
#endif
	*/
	const static UPerfTime uFreq = GetPerfTimerFreq();
	UPerfTime uTime = GetPerfTimer();

	m_uSeconds = GDE::UINT32(uTime/uFreq);
	UPerfTime uRemainder = uTime%uFreq;
	m_uMicroSeconds = GDE::UINT32((FLOAT64(uRemainder)/FLOAT64( uFreq ))*FLOAT64(uNUM_MICROSECONDS));
	return true;
}

void CSystemTime::SetTo( GDE::UINT32 uSeconds, GDE::UINT32 uMicroSeconds )
{
	m_uSeconds = uSeconds;
	m_uMicroSeconds = uMicroSeconds;
}


void CSystemTime::SetTo( FLOAT32 fSeconds )
{
	m_uSeconds = GDE::UINT32( fSeconds );
	m_uMicroSeconds = GDE::UINT32( (fSeconds-m_uSeconds) * FLOAT32(uNUM_MICROSECONDS) );
}

GDE::UINT64 CSystemTime::GetAsUInt( void ) const
{
	return GDE::UINT64(m_uSeconds)*GDE::UINT64(uNUM_MICROSECONDS)+GDE::UINT64(m_uMicroSeconds);
}

void CSystemTime::SetToUInt( GDE::UINT64 uVal ) 
{
	m_uSeconds = GDE::UINT32(uVal/uNUM_MICROSECONDS);
	m_uMicroSeconds = GDE::UINT32(uVal%uNUM_MICROSECONDS);
}


CSystemTime CSystemTime::operator - ( const CSystemTime& Other ) const
{
	CSystemTime RetVal;
//#define _TEST( ... ) __VA_ARGS__
//	_TEST( int i = RetVal.GetMicroSeconds() );
	RetVal.SetTo( 0, 0 );
	if ( Other.m_uMicroSeconds > m_uMicroSeconds )
	{
		// we want to carry
		if ( Other.m_uSeconds < m_uSeconds )
		{
			GDE::UINT32 uOffset = Other.m_uMicroSeconds - m_uMicroSeconds;

			RetVal.SetTo(
				(m_uSeconds-Other.m_uSeconds)-1,
				uNUM_MICROSECONDS - uOffset );
		}
		else
		{
			assert_now( "error, unimplemented" );
			// have to wrap:
		}
	}
	else
	{
		// no need to carry
		if ( Other.m_uSeconds <= m_uSeconds )
		{
			RetVal.SetTo(
				m_uSeconds-Other.m_uSeconds,
				m_uMicroSeconds-Other.m_uMicroSeconds );
		}
		else
		{
			assert_now( "error, unimplemented" );
		}
	}
	return RetVal;
}

const CSystemTime& CSystemTime::operator += ( const CSystemTime& Other )
{
	m_uSeconds += Other.m_uSeconds;
	m_uMicroSeconds += Other.m_uMicroSeconds;
	const GDE::UINT32 uExtra = m_uMicroSeconds/uNUM_MICROSECONDS;
	m_uSeconds += uExtra;
	m_uMicroSeconds -= uExtra*uNUM_MICROSECONDS;
	return *this;
}

const CSystemTime& CSystemTime::operator %= ( const CSystemTime& Other )
{
	// so we need to modulo this time by the other one.
	// so if we modulo seconds and microseconds seperately it won't 
	// give the right answer will it?  NO.
	const GDE::UINT64 uTotalMicroS = GetAsUInt();
	const GDE::UINT64 uTotalMicroSOther = Other.GetAsUInt();
	const GDE::UINT64 uModuloMicroS = uTotalMicroS%uTotalMicroSOther;
	SetToUInt( uModuloMicroS );
	return *this;
}

bool CSystemTime::operator <= ( const CSystemTime& Other ) const
{
	return ( m_uSeconds < Other.m_uSeconds ||
				( m_uSeconds == Other.m_uSeconds &&
				  m_uMicroSeconds <= Other.m_uMicroSeconds ) );
}

bool CSystemTime::operator == ( const CSystemTime& Other ) const
{
	return ( m_uSeconds == Other.m_uSeconds && m_uMicroSeconds == Other.m_uMicroSeconds );
}

GDE::UINT32 CSystemTime::GetMicroSeconds( void ) const
{
	return m_uMicroSeconds;
}

FLOAT32  CSystemTime::GetSeconds( void ) const
{
	FLOAT32 fRV = FLOAT32( m_uSeconds ) + ( FLOAT32( m_uMicroSeconds )/FLOAT32(uNUM_MICROSECONDS) );
	return fRV;
}

FLOAT32 CSystemTime::GetSecondRatio( void )
{
	 return FLOAT32( m_uMicroSeconds )/FLOAT32(uNUM_MICROSECONDS);
}

//!< put the current thread to sleep for that much tume.
void CSystemTime::Sleep( const CSystemTime& Delay )
{
#ifdef _MSC_VER
	// system sleep seems better then the select below which just
	// times out immediately for short times - bah
	// sleep needs milliseconds:
	::Sleep( (Delay.m_uMicroSeconds/1000) + Delay.m_uSeconds * 1000 );
#else
	timeval t;
	Delay.FillTimeVal( t );
	// cheeky but should work:
	// mingw requires -lwsock32 to link with this..
# ifdef _PS2_
#  warning "Select not implemented on PS2"
# else
	select( 0, NULL, NULL, NULL, &t );
# endif
#endif
}

//!< put the current thread to sleep for that much time.
void CSystemTime::Sleep( FLOAT32 fSeconds )
{
	CSystemTime Delay;
	Delay.SetTo( fSeconds );
	Sleep( Delay );
}

GDE::UINT32 CSystemTime::GetMilliTicks( void )
{
#ifndef _MSC_VER
	//return timeGetTime()
	struct timeval tv;
	gettimeofday( &tv, NULL );
//		return tv.tv_sec * 1000 + (tv.tv_usec / 1000);
	return (tv.tv_sec&0x7FFFFFF) * 1000 + (tv.tv_usec / 1000);
#else
	return GetTickCount();
#endif
}

UPerfTime CSystemTime::GetPerfTimer( void )
{
#ifndef _MSC_VER
	//================================================== =
	// Use RDTSC To Read CPU Time Stamp Counter
	//================================================== =
	// Visual C++:
	//__asm rdtsc;
	UPerfTime s64Ret = 0;
# ifdef _PS2_
#  warning "Select not implemented on PS2"
# else
	__asm__ __volatile__ ("rdtsc" : "=A"(s64Ret):);
# endif
	return s64Ret;
#else
	LARGE_INTEGER uVal;
	QueryPerformanceCounter( &uVal );
	return uVal.QuadPart;
#endif
}

// how many of them per second - ?
GDE::UINT32 CSystemTime::GetPerfTimerFreq( GDE::UINT32 uTime )
{
#ifndef _MSC_VER
	GDE::UINT32 timeStart, timeStop;
	UPerfTime startTick, endTick;
	UPerfTime overhead;

	//================================================== =
	// Calculate CPU Tick Function Call Overhead
	//================================================== =
	// can be -ve despite being unsigned...
//		overhead = GetPerfTimer() - GetPerfTimer();
	startTick = GetPerfTimer();
	endTick = GetPerfTimer();
	overhead = endTick - startTick;

	//================================================== =
	// Calculate Starting Time And Start Tick
	//================================================== =
	timeStart = CSystemTime::GetMilliTicks();
	while(CSystemTime::GetMilliTicks() == timeStart)
	{
		timeStart = CSystemTime::GetMilliTicks();
	}

	while(1)
	{
		timeStop = CSystemTime::GetMilliTicks();
		if((timeStop - timeStart) > 1)
		{
			startTick = GetPerfTimer();
			break;
		}
	}

	//================================================== =
	// Calculate Stop Time And End Tick
	//================================================== =
	timeStart = timeStop;
	while(1)
	{
		timeStop = CSystemTime::GetMilliTicks();
		if((timeStop - timeStart) >= uTime)
		{
			endTick = GetPerfTimer();
			break;
		}
	}

	//================================================== =
	// Return The Processors Speed In Hertz
	//================================================== =
	GDE::UINT32 uTimeTaken = (timeStop - timeStart);///UPerfTime(uTime);
	DbgPrint( "Asked for %d delay, got %d\n", uTime, uTimeTaken );
	DbgPrint( "EndTick %qd, overhead %qd, startTick %qd\n", endTick, overhead, startTick );

	GDE::UINT32 uFreq = UPerfTime(UPerfTime(1000) * UPerfTime( (endTick - overhead) - startTick))/UPerfTime(uTimeTaken);

#else
// windows version.
	UNUSED_VAR( uTime );
	LARGE_INTEGER uVal;
	QueryPerformanceFrequency( &uVal );
	GDE::UINT32 uFreq = GDE::UINT32( uVal.QuadPart );
#endif

	DbgPrint( "Your speed is %d - %0.2f GHZ\n", uFreq, (FLOAT32( uFreq ) / (1000.0f*1000000.0f)) );

	return uFreq;
}


void CSystemTime::SetToDiff(
	UPerfTime uStart,
	UPerfTime uEnd,
	GDE::UINT32 uFreq)
{
	assert( uNUM_MICROSECONDS < uFreq );

	UPerfTime uDiff = uEnd - uStart;
	uDiff *= uNUM_MICROSECONDS;
	uDiff /= uFreq;
	m_uSeconds = GDE::UINT32( uDiff / uNUM_MICROSECONDS );
	uDiff -= m_uSeconds * uNUM_MICROSECONDS;
	m_uMicroSeconds = GDE::UINT32(uDiff);
//		DbgPrint( "Interval %d seconds, %d microseconds\n", m_uSeconds, m_uMicroSeconds );
}

void CSystemTime::FillTimeVal( timeval& t ) const
{
	t.tv_sec = m_uSeconds;
	t.tv_usec = m_uMicroSeconds;
}

CClock::CClock( void )
: m_uFrameNum( 0 )
, m_fFrameInterval( 0.0f )
, m_fSpeedOfTime( 1.0f )
, m_fMinFrameInterval( -1.0f )
{
	m_LastTime.Update();
}

void CClock::FrameUpdate( void )
{
	m_uFrameNum++;
#ifndef _PSP_TARGET
	CSystemTime ThisTime;
	ThisTime.Update();

	m_fFrameInterval = (ThisTime - m_LastTime).GetSeconds() * m_fSpeedOfTime;
//	if ( m_fMinFrameInterval > m_fFrameInterval )
//	{
////		CSystemTime::Sleep( m_fMinFrameInterval-m_fFrameInterval );
//		//const FLOAT32 fDelay = m_fMinFrameInterval-m_fFrameInterval;
//		//const UINT32 uMSecs = UINT32(fDelay*FLOAT32(uNUM_MICROSECONDS));
//		//gs_PreciseTimer.Wait( uMSecs )
//		ThisTime.Update();
//		m_fFrameInterval = (ThisTime - m_LastTime).GetSeconds() * m_fSpeedOfTime;	
//	}

	// yes - very bad, a busy waiting loop...
	while ( m_fMinFrameInterval > m_fFrameInterval )
	{
		CSystemTime::Sleep( 0 ); // at least allow another thread to do something...
		ThisTime.Update();
		m_fFrameInterval = (ThisTime - m_LastTime).GetSeconds() * m_fSpeedOfTime;	
	}

	m_LastTime = ThisTime;
/*	const FLOAT32 fMaxInterval = 0.5f;
	if ( m_fFrameInterval > fMaxInterval )
	{
		m_fFrameInterval = fMaxInterval;
	}
	*/
#else
	const FLOAT32 fONE_TICK = 0.000001f;
	// or might want to use the following if this doesn't work...
	// you'll have to figure.
	// unsigned int sceRtcGetTickResolution();
	u_long64 uThisFrameTicks = sceRtcGetAccumulativeTime();
	if ( m_uLastFrameTicks != 0 )
	{

		u_long64 uFrameInterval = uThisFrameTicks - m_uLastFrameTicks;
		m_fFrameInterval = FLOAT32( uFrameInterval ) / fONE_TICK;

		// might need to be:
		//int iFrameInterval = sceRtcCompareTick( (SceRtcTick*)&m_uLastFrameTicks, (SceRtcTick*)&uThisFrameTicks);
		//assert( iFrameInterval > 0 ); // if this fires then swap the args to the above.
		//m_fFrameInterval = FLOAT32( iFrameInterval ) / fONE_MICROSECOND;

	} else
	{
		m_fFrameInterval = 1.0f/60.0f; // assume the 1st frame is running at full framerate
	}

	m_uLastFrameTicks = uThisFrameTicks;
#endif
}

/**
 *	Set the last frame interval - great for forcing a fixed framerate to appear when rendering video for example.
 */
void CClock::FakeFrameInterval( FLOAT32 fFrameInterval )
{
	m_fFrameInterval = fFrameInterval;
}

FLOAT32 CClock::GetFrameInterval( void ) const
{
	return m_fFrameInterval;
}

GDE::UINT32 CClock::GetFrameNum( void ) const
{
	return m_uFrameNum;
}

FLOAT32 CClock::GetSpeedOfTime( void ) const
{
	return m_fSpeedOfTime;
}

void CClock::SetSpeedOfTime( FLOAT32 fV )
{
	m_fSpeedOfTime = fV;
}

/**
 *	returns a number that ranges 0-1, 0-1... in a cycle over the number of seconds given.
 */
FLOAT32 CClock::GetCyclicalOverTime( FLOAT32 fSeconds )
{
	CSystemTime ModuloTime = m_LastTime;
	CSystemTime Denom;
	Denom.SetTo( fSeconds );
	ModuloTime %= Denom;
	return ModuloTime.GetSeconds()/fSeconds;
}

/**
 *	@param fSeconds	The wavelength, in seconds of the sin pattern.
 *	@return a number that ranges -1.0 , 0, 1, 0, -1.0 (in a sinusoidal patteren)
 */
FLOAT32 CClock::GetSinusoidalOverTime( FLOAT32 fSeconds )
{
	return sinf( GetCyclicalOverTime( fSeconds ) * 2.0f * 3.141592653589793f );
}


/**
 *	Set a minimum frame interval, in update will sleep if it is longer than this.
 *	If the minimum is negative or zero then it is ignored!
 */
void CClock::SetMinFrameInterval( FLOAT32 fV )
{
	m_fMinFrameInterval = fV;
}

