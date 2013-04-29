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
#include <gazengine/input/MouseMoveEvent.h>
#include <gazengine/math/Math.h>
#include <gazengine/math/MathFunctions.h>

#include "GunnerEngine.h"
#include "Scene.h"

CGunnerEngine::CGunnerEngine(SimpleTree* _pRootNode, SimpleTree* _pTurretNode, SimpleTree* _pBarrelNode, float _fSpeed,
			float _fReloadTime) :
	m_fLastFireTime(0.0f),
	m_fMouseX(-1.0f),
	m_fMouseY(-1.0f),
	m_fReloadTime(_fReloadTime),
	m_fSpeed(_fSpeed),
	m_pBarrelNode(_pBarrelNode),
	m_pRootNode(_pRootNode),
	m_pTurretNode(_pTurretNode)
{
}

void CGunnerEngine::addEntity(Entity* /*_pEntity*/)
{
}

void CGunnerEngine::advance()
{
}

void CGunnerEngine::destroy()
{
	Messages::deregisterRecipient(Events::MOUSE_BUTTON, this);
	Messages::deregisterRecipient(Events::MOUSE_MOVE, this);
}

void CGunnerEngine::init()
{
	Messages::registerRecipient(Events::MOUSE_BUTTON, this);
	Messages::registerRecipient(Events::MOUSE_MOVE, this);
}

void CGunnerEngine::receive(unsigned short _usSubject, const void* _kpMessage)
{
	if (_usSubject == Events::MOUSE_BUTTON)
	{
		if (m_fLastFireTime > GazEngine::getTotalTime() - m_fReloadTime)
		{
			return;
		}

		const MouseButtonEvent* kpEvent = static_cast<const MouseButtonEvent*>(_kpMessage);

		if (kpEvent->button == Mouse::LEFT && kpEvent->buttonState == Button::UP)
		{
			Matrix44 barrelAbsoluteTransformation = m_pBarrelNode->getAbsoluteTransformation();
			Matrix44 barrelTransformation = m_pBarrelNode->getTransformation();

			Matrix44 transformation;
			setTranslation(transformation, Vector3(0.0f, 0.0f, 4.1f));
			float angleRight = getAngleBetween(getRight3(barrelTransformation), Vector3(1.0f, 0.0f, 0.0f));
			rotate(transformation, angleRight, getUp4(barrelTransformation));
			float angleUp = getAngleBetween(getUp3(barrelTransformation), Vector3(0.0f, 1.0f, 0.0f));
			rotate(transformation, angleUp, getRight4(barrelTransformation));
			rotate(transformation, Math::PI * 0.5f, getRight4(barrelTransformation));
			transformation = barrelAbsoluteTransformation * transformation;

			CreateTankShell(*m_pRootNode, barrelAbsoluteTransformation, transformation);

			m_fLastFireTime = GazEngine::getTotalTime();
		}
	}
	else if (_usSubject == Events::MOUSE_MOVE)
	{
		const MouseMoveEvent* kpEvent = static_cast<const MouseMoveEvent*>(_kpMessage);

		if (m_fMouseX != -1.0f)
		{
			rotate(m_pTurretNode->getTransformation(), (kpEvent->x - m_fMouseX) * m_fSpeed *
				GazEngine::getDeltaTime(), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		}

		if (m_fMouseY != -1.0f)
		{
			rotate(m_pBarrelNode->getTransformation(), (kpEvent->y - m_fMouseY) * m_fSpeed *
				GazEngine::getDeltaTime(), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		}

		m_fMouseX = static_cast<float>(kpEvent->x);
		m_fMouseY = static_cast<float>(kpEvent->y);
	}
}

void CGunnerEngine::removeEntity(const Entity& /*_krEntity*/)
{
}
