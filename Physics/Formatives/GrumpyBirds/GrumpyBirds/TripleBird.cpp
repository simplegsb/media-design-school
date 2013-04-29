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
#include <gazengine/Events.h>
#include <gazengine/GazEngine.h>
#include <gazengine/input/MouseButtonEvent.h>
#include <gazengine/math/Math.h>
#include <gazengine/math/Vector2.h>
#include <gazengine/Messages.h>

#include "Box2DBody.h"
#include "GrumpyFactory.h"
#include "TripleBird.h"

CTripleBird::CTripleBird() :
	m_bFlying(false),
	m_bSplit(false)
{
	Messages::registerRecipient(Events::MOUSE_BUTTON, this);
}

bool CTripleBird::Fly()
{
	m_bFlying = true;

	return m_bSplit;
}

void CTripleBird::receive(unsigned short subject, const void* message)
{
	if (!m_bFlying)
	{
		return;
	}

	const MouseButtonEvent* pEvent = static_cast<const MouseButtonEvent*>(message);
	if (pEvent->button != Mouse::LEFT ||
		pEvent->buttonState != Button::UP)
	{
		return;
	}

	CBox2DBody* pBody = getEntity()->getSingleComponent<CBox2DBody>();
	Vector2 position = pBody->GetPosition();
	Vector2 linearVelocity = pBody->GetLinearVelocity();
	Vector2 offset0(5.0f, 0.0f);
	Vector2 offset1 = offset0;
	offset1.rotate(Math::PI * 0.25f);
	Vector2 offset2 = offset0;
	offset2.rotate(Math::PI * 0.25f);

	GazEngine::removeEntity(*getEntity());

	Vector2 fragmentPosition0 = position + offset0;
	Entity* pFragment0 = GrumpyFactory::CreateTripleBirdFragment(fragmentPosition0, 10.0f);

	offset0.normalise();
	offset0 *= linearVelocity.length();
	CBox2DBody* pFragmentBody0 = pFragment0->getSingleComponent<CBox2DBody>();
	pFragmentBody0->ApplyLinearImpulse(offset0, fragmentPosition0);
	GazEngine::addEntity(pFragment0);

	Vector2 fragmentPosition1 = position + offset1;
	Entity* pFragment1 = GrumpyFactory::CreateTripleBirdFragment(fragmentPosition1, 10.0f);

	offset1.normalise();
	offset1 *= linearVelocity.length();
	CBox2DBody* pFragmentBody1 = pFragment1->getSingleComponent<CBox2DBody>();
	pFragmentBody1->ApplyLinearImpulse(offset1, fragmentPosition1);
	GazEngine::addEntity(pFragment1);

	Vector2 fragmentPosition2 = position + offset2;
	Entity* pFragment2 = GrumpyFactory::CreateTripleBirdFragment(fragmentPosition2, 10.0f);

	offset2.normalise();
	offset2 *= linearVelocity.length();
	CBox2DBody* pFragmentBody2 = pFragment2->getSingleComponent<CBox2DBody>();
	pFragmentBody2->ApplyLinearImpulse(offset2, fragmentPosition2);
	GazEngine::addEntity(pFragment2);

	m_bSplit = true;
	Messages::deregisterRecipient(Events::MOUSE_BUTTON, this);
}
