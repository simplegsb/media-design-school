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
