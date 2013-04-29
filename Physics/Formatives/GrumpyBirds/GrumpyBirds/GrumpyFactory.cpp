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
#include <gazengine/GazEngine.h>
#include <gazengine/gdi/model/GDICircle.h>
#include <gazengine/gdi/model/GDIMesh.h>
#include <gazengine/math/Math.h>
#include <gazengine/model/Circle.h>

#include "BombBird.h"
#include "BoostBird.h"
#include "Box2DBody.h"
#include "EntityCategories.h"
#include "GrumpyConstants.h"
#include "GrumpyFactory.h"
#include "TripleBird.h"

using namespace std;

namespace GrumpyFactory
{
	void CreateBox(const TMaterial& _krMaterial, bool _bDynamic, bool _bOnSide, float _fX, float _fY, float _fWidth,
			float _fHeight);

	Entity* CreateBombBird(const Vector2& _krPosition, float _fRadius)
	{
		TMaterial material;
		material.density = 4.0f / (Math::PI * _fRadius * _fRadius);
		material.friction = 0.5f;
		material.restitution = 0.1f;

		GDICircle* pModel = new GDICircle(_krPosition, _fRadius);

		CBox2DBody* pBody = new CBox2DBody(material, pModel);
		pBody->SetPosition(_krPosition);

		Entity* pEntity = new Entity(EntityCategories::BIRD);
		CBird* pBird = new CBombBird;
		pEntity->addComponent(pBird);
		pBird->setEntity(pEntity);
		pEntity->addComponent(pBody);
		pBody->setEntity(pEntity);
		pEntity->addComponent(pModel);
		pModel->setEntity(pEntity);

		return pEntity;
	}

	Entity* CreateBombBirdFragment(const Vector2& _krPosition, float _fRadius)
	{
		TMaterial material;
		material.density = 4.0f / (Math::PI * _fRadius * _fRadius);
		material.friction = 0.5f;
		material.restitution = 0.1f;

		GDICircle* pModel = new GDICircle(_krPosition, _fRadius);

		CBox2DBody* pBody = new CBox2DBody(material, pModel);
		pBody->SetDynamic(true);
		pBody->SetPosition(_krPosition);

		Entity* pEntity = new Entity(EntityCategories::BIRD);
		pEntity->addComponent(pBody);
		pBody->setEntity(pEntity);
		pEntity->addComponent(pModel);
		pModel->setEntity(pEntity);

		return pEntity;
	}

	Entity* CreateBoostBird(const Vector2& _krPosition, float _fRadius)
	{
		TMaterial material;
		material.density = 2.0f / (Math::PI * _fRadius * _fRadius);
		material.friction = 0.5f;
		material.restitution = 0.1f;

		GDICircle* pModel = new GDICircle(_krPosition, _fRadius);

		CBox2DBody* pBody = new CBox2DBody(material, pModel);
		pBody->SetPosition(_krPosition);

		Entity* pEntity = new Entity(EntityCategories::BIRD);
		CBird* pBird = new CBoostBird;
		pEntity->addComponent(pBird);
		pBird->setEntity(pEntity);
		pEntity->addComponent(pBody);
		pBody->setEntity(pEntity);
		pEntity->addComponent(pModel);
		pModel->setEntity(pEntity);

		return pEntity;
	}

	void CreateBox(const TMaterial& _krMaterial, bool _bDynamic, bool _bOnSide, float _fX, float _fY, float _fWidth,
			float _fHeight)
	{
		vector<Vector2> vertices;
		if (_bOnSide)
		{
			vertices.push_back(Vector2(_fX - _fWidth, _fY - _fHeight));
			vertices.push_back(Vector2(_fX + _fWidth, _fY - _fHeight));
			vertices.push_back(Vector2(_fX + _fWidth, _fY + _fHeight));
			vertices.push_back(Vector2(_fX - _fWidth, _fY + _fHeight));
		}
		else
		{
			vertices.push_back(Vector2(_fX - _fHeight, _fY - _fWidth));
			vertices.push_back(Vector2(_fX + _fHeight, _fY - _fWidth));
			vertices.push_back(Vector2(_fX + _fHeight, _fY + _fWidth));
			vertices.push_back(Vector2(_fX - _fHeight, _fY + _fWidth));
		}
		GDIMesh* pModel = new GDIMesh(vertices, RGB(0, 0, 0));

		CBox2DBody* pBody = new CBox2DBody(_krMaterial, pModel);
		//pBody->SetPosition(Vector2(_fX, _fY));

		if (_bDynamic)
		{
			pBody->SetDynamic(true);
		}

		Entity* pBox = new Entity;
		pBox->addComponent(pBody);
		pBody->setEntity(pBox);
		pBox->addComponent(pModel);
		pModel->setEntity(pBox);
		GazEngine::addEntity(pBox);
	}

	void CreateBoxCube(const TMaterial& _krMaterial, bool _bDynamic, float _fX, float _fY)
	{
		CreateBox(_krMaterial, _bDynamic, true, _fX, _fY, 10.0f, 10.0f);
	}

	void CreateBoxCubeLarge(const TMaterial& _krMaterial, bool _bDynamic, float _fX, float _fY)
	{
		CreateBox(_krMaterial, _bDynamic, true, _fX, _fY, 20.0f, 20.0f);
	}

	void CreateBoxSkinny(const TMaterial& _krMaterial, bool _bDynamic, bool _bOnSide, float _fX, float _fY)
	{
		CreateBox(_krMaterial, _bDynamic, _bOnSide, _fX, _fY, 20.0f, 5.0f);
	}

	void CreateBoxSkinnyLong(const TMaterial& _krMaterial, bool _bDynamic, bool _bOnSide, float _fX, float _fY)
	{
		CreateBox(_krMaterial, _bDynamic, _bOnSide, _fX, _fY, 40.0f, 5.0f);
	}

	void CreateBoxSkinnyReallyLong(const TMaterial& _krMaterial, bool _bDynamic, bool _bOnSide, float _fX, float _fY)
	{
		CreateBox(_krMaterial, _bDynamic, _bOnSide, _fX, _fY, 50.0f, 5.0f);
	}

	void CreateBoxStandard(const TMaterial& _krMaterial, bool _bDynamic, bool _bOnSide, float _fX, float _fY)
	{
		CreateBox(_krMaterial, _bDynamic, _bOnSide, _fX, _fY, 20.0f, 10.0f);
	}

	void CreateMap()
	{
		float horizOffset = 300.0f;

		// Ground
		TMaterial ground;
		ground.density = 0.01f;
		ground.friction = 0.9f;
		ground.restitution = 0.0f;

		CreateBox(ground, false, true, 0.0f, static_cast<float>(WINDOW_HEIGHT), static_cast<float>(WINDOW_WIDTH), 150.0f);

		// Stone
		TMaterial stone;
		stone.density = 0.02f;
		stone.friction = 0.5f;
		stone.restitution = 0.05f;

		CreateBoxSkinny(stone, true, false, horizOffset + 200.0f, 330.0f);
		CreateBoxSkinny(stone, true, false, horizOffset + 230.0f, 330.0f);
		CreateBoxStandard(stone, true, true, horizOffset + 215.0f, 310.0f);

		CreateBoxSkinnyLong(stone, true, false, horizOffset + 280.0f, 320.0f);
		CreateBoxSkinnyLong(stone, true, false, horizOffset + 310.0f, 320.0f);
		CreateBoxStandard(stone, true, true, horizOffset + 295.0f, 270.0f);

		CreateBoxSkinnyReallyLong(stone, true, false, horizOffset + 360.0f, 320.0f);
		CreateBoxSkinnyReallyLong(stone, true, false, horizOffset + 370.0f, 320.0f);
		CreateBoxSkinnyReallyLong(stone, true, false, horizOffset + 420.0f, 320.0f);
		CreateBoxSkinnyReallyLong(stone, true, false, horizOffset + 430.0f, 320.0f);
		CreateBoxSkinnyLong(stone, true, true, horizOffset + 395.0f, 250.0f);
		CreateBoxStandard(stone, true, true, horizOffset + 375.0f, 230.0f);
		CreateBoxStandard(stone, true, true, horizOffset + 415.0f, 230.0f);

		CreateBoxStandard(stone, true, false, horizOffset + 485.0f, 320.0f);
		CreateBoxStandard(stone, true, false, horizOffset + 505.0f, 320.0f);
		CreateBoxCubeLarge(stone, true, horizOffset + 495.0f, 285.0f);
		CreateBoxStandard(stone, true, false, horizOffset + 485.0f, 250.0f);
		CreateBoxStandard(stone, true, false, horizOffset + 505.0f, 250.0f);
		CreateBoxCubeLarge(stone, true, horizOffset + 495.0f, 215.0f);

		CreateBoxStandard(stone, true, false, horizOffset + 545.0f, 320.0f);
		CreateBoxStandard(stone, true, false, horizOffset + 565.0f, 320.0f);
		CreateBoxCubeLarge(stone, true, horizOffset + 555.0f, 285.0f);
		CreateBoxStandard(stone, true, false, horizOffset + 545.0f, 250.0f);
		CreateBoxStandard(stone, true, false, horizOffset + 565.0f, 250.0f);
		CreateBoxCubeLarge(stone, true, horizOffset + 555.0f, 215.0f);

		CreateBoxSkinnyReallyLong(stone, true, true, horizOffset + 525.0f, 190.0f);
		CreateBoxStandard(stone, true, true, horizOffset + 495.0f, 170.0f);
		CreateBoxStandard(stone, true, true, horizOffset + 495.0f, 150.0f);
		CreateBoxCube(stone, true, horizOffset + 525.0f, 170.0f);
		CreateBoxCube(stone, true, horizOffset + 525.0f, 150.0f);
		CreateBoxStandard(stone, true, true, horizOffset + 555.0f, 170.0f);
		CreateBoxStandard(stone, true, true, horizOffset + 555.0f, 150.0f);

		// Glass
		TMaterial glass;
		glass.density = 0.001f;
		glass.friction = 0.5f;
		glass.restitution = 0.05f;

		CreateBoxStandard(glass, true, false, horizOffset + 215.0f, 270.0f);
		CreateBoxCube(glass, true, horizOffset + 215.0f, 240.0f);
		CreateBoxCube(glass, true, horizOffset + 295.0f, 240.0f);
		CreateBoxSkinnyReallyLong(glass, true, true, horizOffset + 255.0f, 230.0f);
		CreateBoxStandard(glass, true, false, horizOffset + 215.0f, 200.0f);
		CreateBoxStandard(glass, true, false, horizOffset + 295.0f, 200.0f);
		CreateBoxStandard(glass, true, false, horizOffset + 380.0f, 200.0f);
		CreateBoxStandard(glass, true, false, horizOffset + 415.0f, 200.0f);
		CreateBoxStandard(glass, true, true, horizOffset + 215.0f, 170.0f);
		CreateBoxSkinnyReallyLong(glass, true, true, horizOffset + 335.0f, 180.0f);
		CreateBoxSkinny(glass, true, true, horizOffset + 295.0f, 170.0f);
		CreateBoxCube(glass, true, horizOffset + 375.0f, 160.0f);
		CreateBoxSkinny(glass, true, true, horizOffset + 402.5f, 180.0f);
		CreateBoxStandard(glass, true, false, horizOffset + 215.0f, 140.0f);
		CreateBoxStandard(glass, true, false, horizOffset + 295.0f, 140.0f);
		CreateBoxStandard(glass, true, false, horizOffset + 375.0f, 130.0f);
		CreateBoxStandard(glass, true, false, horizOffset + 415.0f, 150.0f);
		CreateBoxCube(glass, true, horizOffset + 415.0f, 120.0f);
	}

	Entity* CreatePig(const Vector2& _krPosition, float _fRadius)
	{
		TMaterial material;
		material.density = 0.001f;
		material.friction = 0.5f;
		material.restitution = 0.05f;

		GDICircle* pModel = new GDICircle(_krPosition, _fRadius);

		CBox2DBody* pBody = new CBox2DBody(material, pModel);
		pBody->SetDynamic(true);
		pBody->SetPosition(_krPosition);

		Entity* pEntity = new Entity(EntityCategories::PIG);
		pEntity->addComponent(pBody);
		pBody->setEntity(pEntity);
		pEntity->addComponent(pModel);
		pModel->setEntity(pEntity);

		return pEntity;
	}

	Entity* CreateTripleBird(const Vector2& _krPosition, float _fRadius)
	{
		TMaterial material;
		material.density = 3.0f / (Math::PI * _fRadius * _fRadius);
		material.friction = 0.5f;
		material.restitution = 0.1f;

		GDICircle* pModel = new GDICircle(_krPosition, _fRadius);

		CBox2DBody* pBody = new CBox2DBody(material, pModel);
		pBody->SetPosition(_krPosition);

		Entity* pEntity = new Entity(EntityCategories::BIRD);
		CBird* pBird = new CTripleBird;
		pEntity->addComponent(pBird);
		pBird->setEntity(pEntity);
		pEntity->addComponent(pBody);
		pBody->setEntity(pEntity);
		pEntity->addComponent(pModel);
		pModel->setEntity(pEntity);

		return pEntity;
	}

	Entity* CreateTripleBirdFragment(const Vector2& _krPosition, float _fRadius)
	{
		TMaterial material;
		material.density = 3.0f / (Math::PI * _fRadius * _fRadius);
		material.friction = 0.5f;
		material.restitution = 0.1f;

		GDICircle* pModel = new GDICircle(_krPosition, _fRadius);

		CBox2DBody* pBody = new CBox2DBody(material, pModel);
		pBody->SetDynamic(true);
		pBody->SetPosition(_krPosition);

		Entity* pEntity = new Entity(EntityCategories::BIRD);
		pEntity->addComponent(pBody);
		pBody->setEntity(pEntity);

		return pEntity;
	}
}
