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
// $Rev: 1109 $
//
#pragma once

#include <gazengine/math/Vector.h>

#include "EntityCategories.h"

const Vector4 BALL_COLOUR(1.0f, 0.0f, 0.0f, 1.0f);
const Vector3 BALL_POSITION = Vector3(0.0f, 0.0f, 0.0f);
const float BALL_SIZE = 5.0f;

const float DESTRUCTION_FORCE_THRESHOLD = 50000.0f;

const float DISTANCE_TO_WALL = 75.0f;

const Vector3 FLOOR_POSITION = Vector3(0.0f, -35.0f, 0.0f);

Entity* CreateBall(float _fRadius, SimpleTree& _rParentNode);

Entity* CreateFloor(SimpleTree& _rParentNode);

void CreateWall(float _fBlockSize, SimpleTree& _rParentNode);

Entity* CreateWallBlock(const Vector3& _rPosition, float _fBlockSize, SimpleTree& _rParentNode);

Entity* CreateWallBlock(const Matrix44& _rTransformation, float _fBlockSize, SimpleTree& _rParentNode);

void CreateWallLayer(const Vector3& _rPosition, float _fBlockSize, SimpleTree& _rParentNode);
