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
// $Date: 2013-01-23 09:45:25 +1300 (Wed, 23 Jan 2013) $
// $Rev: 1110 $
//
#include <gazengine/Events.h>
#include <gazengine/GazEngine.h>
#include <gazengine/input/KeyboardButtonEvent.h>

#include "SnakeCharmer.h"
#include "SnakeModel.h"

CSnakeCharmer::CSnakeCharmer(float _fSpeed) :
	m_button(Keyboard::W),
	m_direction(UP),
	m_fMoveDelta(0.0f),
	m_fSpeed(_fSpeed),
	m_tempDirection(UP)
{
	Messages::registerRecipient(Events::KEYBOARD_BUTTON, this);
}

CSnakeCharmer::~CSnakeCharmer()
{
	Messages::deregisterRecipient(Events::KEYBOARD_BUTTON, this);
}

void CSnakeCharmer::charm()
{
	m_fMoveDelta += GazEngine::getDeltaTime();

	if (m_fMoveDelta >= 1.0f / m_fSpeed)
	{
		m_fMoveDelta = 0.0f;
		CSnakeModel* pModel = static_cast<CSnakeModel*>(getEntity()->getSingleComponent<Model>());

		updateDirection();
		pModel->moveTailToHead();
		pModel->switchTailToHead();

		if (m_direction == DOWN)
		{
			pModel->moveHeadDown();
		}
		else if (m_direction == LEFT)
		{
			pModel->moveHeadLeft();
		}
		else if (m_direction == RIGHT)
		{
			pModel->moveHeadRight();
		}
		else if (m_direction == UP)
		{
			pModel->moveHeadUp();
		}
	}
}

CSnakeCharmer::EDirection CSnakeCharmer::getDirection()
{
	return m_direction;
}

void CSnakeCharmer::receive(unsigned short, const void* _kpMessage)
{
	const KeyboardButtonEvent* kpKeyboardEvent = static_cast<const KeyboardButtonEvent*>(_kpMessage);

	if (kpKeyboardEvent->buttonState == Button::DOWN)
	{
		if (kpKeyboardEvent->button == Keyboard::A)
		{
			m_tempDirection = LEFT;
		}
		else if (kpKeyboardEvent->button == Keyboard::D)
		{
			m_tempDirection = RIGHT;
		}
		else if (kpKeyboardEvent->button == Keyboard::S)
		{
			m_tempDirection = DOWN;
		}
		else if (kpKeyboardEvent->button == Keyboard::W)
		{
			m_tempDirection = UP;
		}
	}
}

void CSnakeCharmer::setDirection(EDirection _direction)
{
	m_tempDirection = _direction;
}

void CSnakeCharmer::speedUp()
{
	m_fSpeed++;
}

void CSnakeCharmer::updateDirection()
{
	if ((m_tempDirection == LEFT || m_tempDirection == RIGHT) &&
		(m_direction != DOWN && m_direction != UP))
	{
		return;
	}

	if ((m_tempDirection == DOWN || m_tempDirection == UP) &&
		(m_direction != LEFT && m_direction != RIGHT))
	{
		return;
	}

	m_direction = m_tempDirection;
}
