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
#include <gazengine/physics/PhysXBody.h>

namespace SceneFactory
{
	Entity* CreateBird(const Matrix44& _krTransformation, unsigned short _usCategory, float _fRadius);

	PhysXBody* CreateBox(SimpleTree* _pParentNode, const Matrix44& _krTransformation, const Vector3& _krHalfExtents,
		unsigned short _usCategory, bool _bCreateBody = true, bool _bDynamic = true);

	void CreateFence(const Matrix44& _krTransformation, unsigned int _uiSegmentCount, bool _bWithGate);

	PhysXBody* CreateGround(float _fAltitude);

	void CreateHouseRoof(SimpleTree& _rParentNode, const Matrix44& _krTransformation);

	void CreateHouseOneStory(SimpleTree& _rParentNode, const Matrix44& _krTransformation);

	void CreateHouseStory(SimpleTree& _rParentNode, const Matrix44& _krTransformation);

	void CreateHouseTwoStory(SimpleTree& _rParentNode, const Matrix44& _krTransformation);

	Body::Material CreateMaterial(unsigned short _usCategory);

	void CreatePig(const Matrix44& _krTransformation, float _fRadius);

	void CreatePillar(SimpleTree& _rParentNode, Matrix44 _transformation, unsigned short _usCategory);

	void CreateScene();

	PhysXBody* CreateSphere(SimpleTree* _pParentNode, const Matrix44& _krTransformation, unsigned short _usCategory,
		float _fRadius);

	Vector4 GetColour(unsigned short _usCategory);

	float GetDestructionForceThreshold(unsigned short _usCategory);
}
