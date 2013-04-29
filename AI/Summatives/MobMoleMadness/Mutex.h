//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// Copyright (c) 2012 Media Design School
//
// Author: Gary Buyn
//
// SVN Info
// $Author: MEDIADESIGN\gary.buyn $
// $Date: 2013-01-23 09:40:30 +1300 (Wed, 23 Jan 2013) $
// $Rev: 1108 $
//
#ifndef MUTEX_H_
#define MUTEX_H_

#include <windows.h>

class CMutex
{
	public:
		CMutex() :
			m_bLocked(false)
		{
		}

		bool IsLocked() const
		{
			return m_bLocked;
		}

		void Lock()
		{
			while (m_bLocked)
			{
				Sleep(10);
			}

			m_bLocked = true;
		}

		void Unlock()
		{
			m_bLocked = false;
		}

	private:
		bool m_bLocked;
};

class CScopedLock
{
	public:
		CScopedLock(CMutex& _mutex) :
			m_mutex(_mutex)
		{
			m_mutex.Lock();
		}

		~CScopedLock()
		{
			m_mutex.Unlock();
		}

	private:
		CMutex m_mutex;
};

#endif /* MUTEX_H_ */
