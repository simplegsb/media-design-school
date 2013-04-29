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
// $Date: 2013-01-25 10:40:08 +1300 (Fri, 25 Jan 2013) $
// $
#include "Box2DVector.h"

b2Vec2 ToB2Vec2(const Vector2& _krOriginal)
{
	return b2Vec2(_krOriginal.x, _krOriginal.y);
}

Vector2 ToVector2(const b2Vec2& _krOriginal)
{
	return Vector2(_krOriginal.x, _krOriginal.y);
}
