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
#include <gazengine/GazEngine.h>
#include <gazengine/math/Math.h>
#include <gazengine/math/Vector2.h>

#include "BombBird.h"
#include "Box2DBody.h"
#include "GrumpyFactory.h"

CBombBird::CBombBird() :
	m_fFuseTime(5.0f)
{
}

bool CBombBird::Fly()
{
	m_fFuseTime -= GazEngine::getDeltaTime();

	if (m_fFuseTime > 0.0f)
	{
		return false;
	}

	Vector2 position = getEntity()->getSingleComponent<CBox2DBody>()->GetPosition();
	Vector2 offset(5.0f, 0.0f);

	GazEngine::removeEntity(*getEntity());

	unsigned int uiFragmentCount = 8;
	for (unsigned int uiIndex = 0; uiIndex < uiFragmentCount; uiIndex++)
	{
		Vector2 fragmentPosition = position + offset;
		Entity* pFragment = GrumpyFactory::CreateBombBirdFragment(fragmentPosition, 5.0f);
		CBox2DBody* pFragmentBody = pFragment->getSingleComponent<CBox2DBody>();
		pFragmentBody->ApplyLinearImpulse(offset * 100.0f, fragmentPosition);
		GazEngine::addEntity(pFragment);

		offset.rotate(2.0f * Math::PI * (1.0f / uiFragmentCount));
	}

	return true;
}
