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
#include <gazengine/gdi/model/GDIMesh.h>
#include <gazengine/model/Circle.h>

#include "Box2DBody.h"

CBox2DBody::CBox2DBody(const TMaterial& _krMaterial, const Model* _kpModel) :
	m_bodyDefinition(),
	m_fixtureDefinition(),
	m_kpModel(_kpModel),
	m_material(),
	m_pBody(NULL),
	m_pFixture(NULL),
	m_pShape(NULL)
{
	SetMaterial(_krMaterial);

	const Circle* pCircle = dynamic_cast<const Circle*>(_kpModel);
	if (pCircle != NULL)
	{
		b2CircleShape* pCircleShape = new b2CircleShape;
		pCircleShape->m_radius = pCircle->getRadius();
		m_fixtureDefinition.shape = pCircleShape;
		m_pShape = pCircleShape;
	}

	// Dependency on GDI!!!... Only works for GDI!!!
	const GDIMesh* pMesh = dynamic_cast<const GDIMesh*>(_kpModel);
	if (pMesh != NULL)
	{
		b2Vec2* pVertices = new b2Vec2[pMesh->getVertices().size()];
		for (unsigned int index = 0; index < pMesh->getVertices().size(); index++)
		{
			pVertices[index] = ToB2Vec2(pMesh->getVertices()[index]);
		}

		b2PolygonShape* pPolygonShape = new b2PolygonShape;
		pPolygonShape->Set(pVertices, pMesh->getVertices().size());
		m_fixtureDefinition.shape = pPolygonShape;
		m_pShape = pPolygonShape;

		delete pVertices;
	}
}

CBox2DBody::~CBox2DBody()
{
	delete m_pShape;
}

void CBox2DBody::AddToWorld(b2World& _rWorld)
{
	m_pBody = _rWorld.CreateBody(&m_bodyDefinition);
	m_pBody->SetUserData(getEntity());
	m_pFixture = m_pBody->CreateFixture(&m_fixtureDefinition);
	m_pFixture->SetUserData(getEntity());
}

void CBox2DBody::ApplyLinearImpulse(const Vector2& _krImpulse, const Vector2& _krPosition)
{
	if (m_pBody == NULL)
	{
		m_bodyDefinition.linearVelocity = ToB2Vec2(_krImpulse);
	}
	else
	{
		m_pBody->ApplyLinearImpulse(ToB2Vec2(_krImpulse), ToB2Vec2(_krPosition));
	}
}

const Vector2 CBox2DBody::GetLinearVelocity() const
{
	if (m_pBody == NULL)
	{
		return ToVector2(m_bodyDefinition.linearVelocity);
	}

	return ToVector2(m_pBody->GetLinearVelocity());
}

const TMaterial& CBox2DBody::GetMaterial() const
{
	return m_material;
}

const Vector2 CBox2DBody::GetPosition() const
{
	if (m_pBody == NULL)
	{
		return ToVector2(m_bodyDefinition.position);
	}

	return ToVector2(m_pBody->GetPosition());
}

void CBox2DBody::RemoveFromWorld(b2World& _rWorld)
{
	_rWorld.DestroyBody(m_pBody);
}

void CBox2DBody::SetDynamic(bool _bDynamic)
{
	if (_bDynamic)
	{
		if (m_pBody == NULL)
		{
			m_bodyDefinition.type = b2_dynamicBody;
		}
		else
		{
			m_pBody->SetType(b2_dynamicBody);
		}
	}
	else
	{
		if (m_pBody == NULL)
		{
			m_bodyDefinition.type = b2_staticBody;
		}
		else
		{
			m_pBody->SetType(b2_staticBody);
		}
	}
}

void CBox2DBody::SetMaterial(const TMaterial& _krMaterial)
{
	m_material = _krMaterial;

	m_fixtureDefinition.density = _krMaterial.density;
	m_fixtureDefinition.friction = _krMaterial.friction;
	m_fixtureDefinition.restitution = _krMaterial.restitution;
}

void CBox2DBody::SetPosition(const Vector2& _krPosition)
{
	if (m_pBody == NULL)
	{
		m_bodyDefinition.position = ToB2Vec2(_krPosition);
	}
	else
	{
		m_pBody->SetTransform(ToB2Vec2(_krPosition), m_pBody->GetAngle());
	}
}
