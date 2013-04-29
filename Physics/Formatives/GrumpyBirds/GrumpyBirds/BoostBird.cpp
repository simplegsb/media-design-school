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
#include "BoostBird.h"

CBoostBird::CBoostBird() :
	m_bFlying(false),
	m_bBoosting(false)
{
	Messages::registerRecipient(Events::MOUSE_BUTTON, this);
}

bool CBoostBird::Fly()
{
	m_bFlying = true;

	return m_bBoosting;
}

void CBoostBird::receive(unsigned short subject, const void* message)
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
	Vector2 linearVelocity = pBody->GetLinearVelocity();
	linearVelocity.normalise();
	linearVelocity *= 1000.0f;
	pBody->ApplyLinearImpulse(linearVelocity, pBody->GetPosition());

	m_bBoosting = true;
	Messages::deregisterRecipient(Events::MOUSE_BUTTON, this);
}
