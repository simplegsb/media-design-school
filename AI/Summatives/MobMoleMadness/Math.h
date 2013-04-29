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
#ifndef MATH_H_
#define MATH_H_

/**
 * Mathematical constants and functions.
 */
namespace Math
{
	const float PI = 3.14159265358979323f;

	float GetRandomFloat(float _fMin, float _fMax);

	int GetRandomInt(int _iMin, int _iMax);
}

#endif /* MATH_H_ */
