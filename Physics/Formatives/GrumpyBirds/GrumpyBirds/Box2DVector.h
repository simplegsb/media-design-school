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
// $Date: 2013-01-23 09:43:23 +1300 (Wed, 23 Jan 2013) $
// $
#pragma once

#include <Box2D/Box2D.h>

#include <gazengine/math/Vector2.h>

b2Vec2 ToB2Vec2(const Vector2& _krOriginal);

Vector2 ToVector2(const b2Vec2& _krOriginal);
