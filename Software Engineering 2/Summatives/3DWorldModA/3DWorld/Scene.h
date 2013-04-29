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
#include <gazengine/model/Mesh.h>
#include <gazengine/physics/Body.h>

void CreateCube(SimpleTree& _rRoot, const Matrix44& _rTransformation);

Model::Material CreateCubeModelMaterial();

Body::Material CreateObstacleBodyMaterial();

void CreatePrismRectangle(SimpleTree& _rRoot, const Matrix44& _rTransformation);

void CreatePrismRectangleCube(SimpleTree& _rRoot, const Matrix44& _rTransformation);

Model::Material CreatePrismRectangleCubeModelMaterial();

Model::Material CreatePrismRectangleModelMaterial();

void CreateShapes(SimpleTree& _rRoot);

Body* CreateTank(SimpleTree& _rRoot, const Matrix44& _rTransformation, SimpleTree* _pChassisCameraNode,
				 SimpleTree* _pBarrelCameraNode, SimpleTree** _pTurretNode, SimpleTree** _pBarrelNode,
				 SimpleTree* _pHeadlight0Node, SimpleTree* _pHeadlight1Node, SimpleTree* _pBarrelLightNode);

Body::Material CreateTankBodyMaterial();

Model::Material CreateTankModelMaterial();

Mesh* CreateTankPart(float length, float width, float height, const Vector4& color, bool extraSlopyFront);

void CreateWorld(SimpleTree& _rRoot);

Model::Material CreateWorldModelMaterial();
