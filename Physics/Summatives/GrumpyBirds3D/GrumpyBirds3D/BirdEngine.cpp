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
#include <algorithm>

#include <gazengine/Componentizer.h>
#include <gazengine/GazEngine.h>
#include <gazengine/math/MathFunctions.h>
#include <gazengine/physics/Body.h>

#include "BirdEngine.h"
#include "EntityCategories.h"
#include "SceneFactory.h"

CBirdEngine::CBirdEngine(float _fExplosionRange, float _fExplosionTime) :
	m_birds(),
	m_fExplosionRange(_fExplosionRange),
	m_fExplosionTime(_fExplosionTime)
{
}

void CBirdEngine::addEntity(Entity* _pEntity)
{
	if (_pEntity->getCategory() == EntityCategories::BIRD)
	{
		m_birds.push_back(_pEntity);
	}
}

void CBirdEngine::advance()
{
	for (unsigned int uiIndex = 0; uiIndex < m_birds.size(); uiIndex++)
	{
		if (!m_birds[uiIndex]->getSingleComponent<Componentizer<bool> >()->getValue())
		{
			continue;
		}

		Componentizer<float>* pTimer = m_birds[uiIndex]->getSingleComponent<Componentizer<float> >();
		pTimer->getValue() += GazEngine::getDeltaTime();

		if (pTimer->getValue() >= m_fExplosionTime)
		{
			SimpleTree* pWorld = GazEngine::getWorldRepresentations()[0];
			Vector3 birdPosition = getTranslation3(m_birds[uiIndex]->getSingleComponent<Body>()->getTransformation());
			explode(*pWorld, m_birds[uiIndex], birdPosition);

			float fSpeed = 100.0f;
			float fHalfRadius = 0.5f;

			Matrix44 birdTransformation = m_birds[uiIndex]->getSingleComponent<Body>()->getTransformation();

			Matrix44 birdFragment0Transformation = birdTransformation;
			getTranslation3(birdFragment0Transformation).X() -= fHalfRadius;
			Entity* birdFragment0 = SceneFactory::CreateBird(birdFragment0Transformation,
				EntityCategories::BIRD_FRAGMENT, fHalfRadius);
			birdFragment0->getSingleComponent<Body>()->setLinearVelocity(Vector3(-fSpeed, 0.0f, 0.0f));

			Matrix44 birdFragment1Transformation = birdTransformation;
			getTranslation3(birdFragment1Transformation).X() += fHalfRadius;
			Entity* birdFragment1 = SceneFactory::CreateBird(birdFragment1Transformation,
				EntityCategories::BIRD_FRAGMENT, fHalfRadius);
			birdFragment1->getSingleComponent<Body>()->setLinearVelocity(Vector3(fSpeed, 0.0f, 0.0f));

			Matrix44 birdFragment2Transformation = birdTransformation;
			getTranslation3(birdFragment2Transformation).Y() -= fHalfRadius;
			Entity* birdFragment2 = SceneFactory::CreateBird(birdFragment2Transformation,
				EntityCategories::BIRD_FRAGMENT, fHalfRadius);
			birdFragment2->getSingleComponent<Body>()->setLinearVelocity(Vector3(0.0f, -fSpeed, 0.0f));

			Matrix44 birdFragment3Transformation = birdTransformation;
			getTranslation3(birdFragment3Transformation).Y() += fHalfRadius;
			Entity* birdFragment3 = SceneFactory::CreateBird(birdFragment3Transformation,
				EntityCategories::BIRD_FRAGMENT, fHalfRadius);
			birdFragment3->getSingleComponent<Body>()->setLinearVelocity(Vector3(0.0f, fSpeed, 0.0f));

			Matrix44 birdFragment4Transformation = birdTransformation;
			getTranslation3(birdFragment4Transformation).Z() -= fHalfRadius;
			Entity* birdFragment4 = SceneFactory::CreateBird(birdFragment4Transformation,
				EntityCategories::BIRD_FRAGMENT, fHalfRadius);
			birdFragment4->getSingleComponent<Body>()->setLinearVelocity(Vector3(0.0f, 0.0f, -fSpeed));

			Matrix44 birdFragment5Transformation = birdTransformation;
			getTranslation3(birdFragment5Transformation).Z() += fHalfRadius;
			Entity* birdFragment5 = SceneFactory::CreateBird(birdFragment5Transformation,
				EntityCategories::BIRD_FRAGMENT, fHalfRadius);
			birdFragment5->getSingleComponent<Body>()->setLinearVelocity(Vector3(0.0f, 0.0f, fSpeed));

			GazEngine::removeEntity(*m_birds[uiIndex]);
		}
	}
}

void CBirdEngine::destroy()
{
}

void CBirdEngine::explode(SimpleTree& _rNode, const Entity* _pBird, const Vector3& birdPosition)
{
	if (_rNode.getModel() != NULL)
	{
		Vector3 toNode = getTranslation3(_rNode.getAbsoluteTransformation()) - birdPosition;
		float fDistanceToNode = toNode.getMagnitude();

		if (_rNode.getModel()->getEntity() != _pBird && fDistanceToNode <= m_fExplosionRange)
		{
			toNode.normalize();
			toNode *= fDistanceToNode / m_fExplosionRange * 50000.0f;

			Body* pBody = _rNode.getModel()->getEntity()->getSingleComponent<Body>();
			if (pBody != NULL)
			{
				pBody->applyForce(toNode, birdPosition);
			}
		}
	}

	for (unsigned int index = 0; index < _rNode.getChildren().size(); index++)
	{
		explode(*_rNode.getChildren()[index], _pBird, birdPosition);
	}
}

void CBirdEngine::init()
{
}

void CBirdEngine::removeEntity(const Entity& _krEntity)
{
	if (_krEntity.getCategory() == EntityCategories::BIRD)
	{
		m_birds.erase(remove(m_birds.begin(), m_birds.end(), &_krEntity));
	}
}
