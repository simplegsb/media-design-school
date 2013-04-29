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

#include <gazengine/Component.h>
#include <gazengine/model/Model.h>

#include "Box2DVector.h"
#include "Material.h"

class CBox2DBody : public Component
{
	public:
		CBox2DBody(const TMaterial& _krMaterial, const Model* _kpModel);

		~CBox2DBody();

		void AddToWorld(b2World& _rWorld);

		void ApplyLinearImpulse(const Vector2& _krImpulse, const Vector2& _krPosition);

		const Vector2 GetLinearVelocity() const;

		const TMaterial& GetMaterial() const;

		const Vector2 GetPosition() const;

		void RemoveFromWorld(b2World& _rWorld);

		void SetDynamic(bool _bDynamic);

		void SetMaterial(const TMaterial& _krMaterial);

		void SetPosition(const Vector2& _krPosition);

	private:
		b2Body* m_pBody;

		b2BodyDef m_bodyDefinition;

		const Model* m_kpModel;

		TMaterial m_material;

		b2Fixture* m_pFixture;

		b2FixtureDef m_fixtureDefinition;

		b2Shape* m_pShape;
};
