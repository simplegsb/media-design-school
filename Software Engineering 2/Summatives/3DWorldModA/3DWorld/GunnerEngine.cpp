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
#include <gazengine/input/MouseMoveEvent.h>
#include <gazengine/math/MathFunctions.h>

#include "GunnerEngine.h"

CGunnerEngine::CGunnerEngine(SimpleTree* _pTurretNode, SimpleTree* _pBarrelNode, float _fSpeed) :
	m_fMouseX(-1.0f),
	m_fMouseY(-1.0f),
	m_fSpeed(_fSpeed),
	m_pBarrelNode(_pBarrelNode),
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
	Messages::deregisterRecipient(Events::MOUSE_MOVE, this);
}

void CGunnerEngine::init()
{
	Messages::registerRecipient(Events::MOUSE_MOVE, this);
}

void CGunnerEngine::receive(unsigned short /*_usSubject*/, const void* _kpMessage)
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

void CGunnerEngine::removeEntity(const Entity& /*_krEntity*/)
{
}
