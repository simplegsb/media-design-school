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
#ifndef CRITICALSCOPE_H_
#define CRITICALSCOPE_H_

#include <windows.h>

class CCriticalScope
{
	public:
		CCriticalScope(CRITICAL_SECTION& _criticalSection);

		~CCriticalScope();

	private:
		CRITICAL_SECTION& m_criticalSection;

		CCriticalScope(const CCriticalScope&);

		CCriticalScope& operator=(const CCriticalScope&);
};

#endif /* CRITICALSCOPE_H_ */
