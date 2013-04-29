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
#include <gazengine/math/Math.h>
#include <gazengine/math/MathFunctions.h>

#include "DriverEngine.h"

CDriverEngine::CDriverEngine(Body* _pBody, float _fAcceleration, float _fTopAngularSpeed, float _fTopLinearSpeed) :
	m_bMovingBackward(false),
	m_bMovingForward(false),
	m_bTurningLeft(false),
	m_bTurningRight(false),
	m_fAcceleration(_fAcceleration),
	m_fTopAngularSpeed(_fTopAngularSpeed),
	m_fTopLinearSpeed(_fTopLinearSpeed),
	m_pBody(_pBody)
{
}

void CDriverEngine::addEntity(Entity* /*_pEntity*/)
{
}

void CDriverEngine::advance()
{
	Vector3 up;
	up = getUp3(m_pBody->getTransformation());
	up.normalize();
	Vector3 out;
	out = getOut3(m_pBody->getTransformation());
	out.normalize();
	Vector3 position = getTranslation3(m_pBody->getTransformation());

	float fAngularSpeed = m_pBody->getAngularVelocity().getMagnitude();

	// If we get the actual magnitude the sign will be lost (due to the square root) and we won't be able to tell if
	// the tank is moving forward or backward.
	float fLinearSpeedSquared = m_pBody->getLinearVelocity().getMagnitudeSquared();
	float fTopLinearSpeedSquared = pow(m_fTopLinearSpeed, 2.0f);

	float fForceMagnitude = m_fAcceleration * m_pBody->getMass();
	float fSixtyDegreeY = sin(Math::PI / 3.0f * 2.0f);

	// Boost up hills!
	if (out.Y() > 0.0f)
	{
		fForceMagnitude *= 2.0f;
	}

	if (m_bMovingBackward)
	{
		if (out.Y() > -fSixtyDegreeY &&
			fLinearSpeedSquared > -fTopLinearSpeedSquared)
		{
			m_pBody->applyForce(out * fForceMagnitude * -1.0f, position);
		}
	}
	if (m_bMovingForward)
	{
		if (out.Y() < fSixtyDegreeY &&
			fLinearSpeedSquared < fTopLinearSpeedSquared)
		{
			m_pBody->applyForce(out * fForceMagnitude, position);
		}
	}
	if (m_bTurningLeft)
	{
		if (fAngularSpeed < m_fTopAngularSpeed)
		{
			m_pBody->applyTorque(up * fForceMagnitude * 10.0f);
		}
	}
	if (m_bTurningRight)
	{
		if (fAngularSpeed < m_fTopAngularSpeed)
		{
			m_pBody->applyTorque(up * fForceMagnitude * -10.0f);
		}
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
