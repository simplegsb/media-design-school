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
// $Date: 2013-01-29 11:58:25 +1300 (Tue, 29 Jan 2013) $
// $Rev: 1129 $
//
#pragma once

#include "Material.h"

namespace GrumpyFactory
{
	Entity* CreateBombBird(const Vector2& _krPosition, float _fRadius);

	Entity* CreateBombBirdFragment(const Vector2& _krPosition, float _fRadius);

	Entity* CreateBoostBird(const Vector2& _krPosition, float _fRadius);

	void CreateBoxCube(const TMaterial& _krMaterial, bool _bDynamic, float _fX, float _fY);

	void CreateBoxCubeLarge(const TMaterial& _krMaterial, bool _bDynamic, float _fX, float _fY);

	void CreateBoxSkinny(const TMaterial& _krMaterial, bool _bDynamic, bool _bOnSide, float _fX, float _fY);

	void CreateBoxSkinnyLong(const TMaterial& _krMaterial, bool _bDynamic, bool _bOnSide, float _fX, float _fY);

	void CreateBoxSkinnyReallyLong(const TMaterial& _krMaterial, bool _bDynamic, bool _bOnSide, float _fX, float _fY);

	void CreateBoxStandard(const TMaterial& _krMaterial, bool _bDynamic, bool _bOnSide, float _fX, float _fY);

	void CreateMap();

	Entity* CreatePig(const Vector2& _krPosition, float _fRadius);

	Entity* CreateTripleBird(const Vector2& _krPosition, float _fRadius);

	Entity* CreateTripleBirdFragment(const Vector2& _krPosition, float _fRadius);
}
