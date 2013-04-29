//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// Copyright (c) 2012 Media Design School
//
// SVN Info
// $Author: MEDIADESIGN\gary.buyn $
// $Date: 2012-09-24 08:28:44 +1200 (Mon, 24 Sep 2012) $
// $Rev: 68 $
//
#include <windows.h>

#include "CriticalScope.h"

CCriticalScope::CCriticalScope(CRITICAL_SECTION& _criticalSection) :
	m_criticalSection(_criticalSection)
{
	EnterCriticalSection(&m_criticalSection);
}

CCriticalScope::~CCriticalScope()
{
	LeaveCriticalSection(&m_criticalSection);
}
