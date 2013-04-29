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

#include <gazengine/graph/SimpleTree.h>

void CreateCube(SimpleTree& _rRoot, const Matrix44& _rTransformation);

void CreatePrismRectangle(SimpleTree& _rRoot, const Matrix44& _rTransformation);

void CreatePrismRectangleCube(SimpleTree& _rRoot, const Matrix44& _rTransformation);

void CreateShapes(SimpleTree& _rRoot);

void CreateWorld(SimpleTree& _rRoot);
