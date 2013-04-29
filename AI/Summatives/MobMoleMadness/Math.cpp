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
// $Date: 2013-01-23 09:40:30 +1300 (Wed, 23 Jan 2013) $
// $Rev: 1108 $
//
#include <cstdlib>
#include <time.h>

#include "Math.h"

namespace Math
{
	bool g_bRandomIsSeeded = false;

	float GetRandomFloat(float _fMin, float _fMax)
	{
		if (!g_bRandomIsSeeded)
		{
			srand((unsigned) time(NULL));
			g_bRandomIsSeeded = true;
		}

		return _fMin + (float) rand() / ((float) RAND_MAX / (_fMax - _fMin));
	}

	int GetRandomInt(int _iMin, int _iMax)
	{
		if (!g_bRandomIsSeeded)
		{
			srand((unsigned) time(NULL));
			g_bRandomIsSeeded = true;
		}

		return rand() % (_iMax - _iMin + 1) + _iMin;
	}
}
