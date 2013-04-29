/**
 *	@file GDE\Core\PerfTools.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2011 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Core.h"
#include "PerfTools.h"
#include "Logger.h"

#ifndef __GNUC__
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#endif

using namespace GDE;

CLogger*	CPerformanceMonitor::sm_pLog = &GDE::CLogger::GetDebugPrintLog();

void CPerformanceMonitor::SetLog( CLogger* pLog )
{
	sm_pLog = pLog;
}

CLogger*	CPerformanceMonitor::GetLog( void )
{
	return sm_pLog;
}

CPerformanceMonitor::CPerformanceMonitor( const char* pDescription )
: m_pDescription( pDescription )
{
#ifndef __GNUC__
	QueryPerformanceFrequency( (LARGE_INTEGER*)&m_uFrequency );
	QueryPerformanceCounter( (LARGE_INTEGER*)&m_uStartTime );
#endif
}

CPerformanceMonitor::~CPerformanceMonitor( void )
{
#ifndef __GNUC__
	UINT64 uEndTime;
	QueryPerformanceCounter( (LARGE_INTEGER*)&uEndTime );
	UINT64 uElapsedTime = uEndTime - m_uStartTime;
	FLOAT64 fElapsedTime = FLOAT64( uElapsedTime )/FLOAT64( m_uFrequency );
	sm_pLog->PrintF( "Timing: %s : %0.6f\n",  m_pDescription, FLOAT32( fElapsedTime ) );
#endif
}
