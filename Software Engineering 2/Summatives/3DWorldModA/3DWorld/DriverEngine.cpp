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
#include <gazengine/input/KeyboardButtonEvent.h>
#include <gazengine/math/MathFunctions.h>

#include "DriverEngine.h"

CDriverEngine::CDriverEngine(Body* _pBody, float _fSpeed) :
	m_bMovingBackward(false),
	m_bMovingForward(false),
	m_bTurningLeft(false),
	m_bTurningRight(false),
	m_fSpeed(_fSpeed),
	m_pBody(_pBody)
{
}

void CDriverEngine::addEntity(Entity* /*_pEntity*/)
{
}

void CDriverEngine::advance()
{
	Vector3 up;
	up.X() = m_pBody->getTransformation()[4];
	up.Y() = m_pBody->getTransformation()[5];
	up.Z() = m_pBody->getTransformation()[6];
	up.normalize();
	Vector3 out;
	out.X() = m_pBody->getTransformation()[8];
	out.Y() = m_pBody->getTransformation()[9];
	out.Z() = m_pBody->getTransformation()[10];
	out.normalize();
	Vector3 position = getTranslation3(m_pBody->getTransformation());

	if (m_bMovingBackward)
	{
		m_pBody->applyForce(out * m_fSpeed * -1.0f, position + Vector3(0.0f, -0.75f, 1.0f));
	}
	if (m_bMovingForward)
	{
		m_pBody->applyForce(out * m_fSpeed, position + Vector3(0.0f, -0.75f, 1.0f));
	}
	if (m_bTurningLeft)
	{
		m_pBody->applyTorque(up * m_fSpeed);
	}
	if (m_bTurningRight)
	{
		m_pBody->applyTorque(up * m_fSpeed * -1.0f);
	}
}

void CDriverEngine::destroy()
{
	Messages::deregisterRecipient(Events::KEYBOARD_BUTTON, this);
}

void CDriverEngine::init()
{
	Messages::registerRecipient(Events::KEYBOARD_BUTTON, this);
}

void CDriverEngine::receive(unsigned short /*_usSubject*/, const void* _kpMessage)
{
	const KeyboardButtonEvent* kpEvent = static_cast<const KeyboardButtonEvent*>(_kpMessage);

	bool bButtonDown = false;
	if (kpEvent->buttonState == Button::DOWN)
	{
		bButtonDown = true;
	}

	if (kpEvent->button == Keyboard::ARROW_DOWN)
	{
		m_bMovingBackward = bButtonDown;
	}
	if (kpEvent->button == Keyboard::ARROW_LEFT)
	{
		m_bTurningLeft = bButtonDown;
	}
	if (kpEvent->button == Keyboard::ARROW_RIGHT)
	{
		m_bTurningRight = bButtonDown;
	}
	if (kpEvent->button == Keyboard::ARROW_UP)
	{
		m_bMovingForward = bButtonDown;
	}
}

void CDriverEngine::removeEntity(const Entity& /*_krEntity*/)
{
}
