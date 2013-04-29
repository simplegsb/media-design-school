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
#include <gazengine/Events.h>
#include <gazengine/GazEngine.h>
#include <gazengine/math/Math.h>
#include <gazengine/Messages.h>

#include "Box2DBody.h"
#include "EntityCategories.h"
#include "GrumpyConstants.h"
#include "GrumpyEngine.h"
#include "GrumpyFactory.h"

using namespace std;

CGrumpyEngine::CGrumpyEngine() :
	m_birdState(READY),
	m_pBird(NULL),
	m_pigs(),
	m_remainingBirds()
{
}

void CGrumpyEngine::addBird(Entity* bird)
{
	m_remainingBirds.push_back(bird);
}

void CGrumpyEngine::addEntity(Entity* entity)
{
	if (entity->getCategory() == EntityCategories::PIG)
	{
		m_pigs.push_back(entity);
	}
}

void CGrumpyEngine::advance()
{
	if (m_birdState == FIRING)
	{
		if (m_pBird->getSingleComponent<CBird>()->Fly())
		{
			m_birdState = COMPLETE;
			spawnNextBird();
		}
	}
}

void CGrumpyEngine::checkPigCollisions(const TBox2DEvent* _pEvent)
{
	for (list<Entity*>::iterator pigIter = m_pigs.begin();
		pigIter != m_pigs.end(); pigIter++)
	{
		if (_pEvent->contact->GetFixtureA()->GetUserData() == *pigIter ||
			_pEvent->contact->GetFixtureB()->GetUserData() == *pigIter)
		{
			float totalImpulse = 0.0f;
			for (int32 index = 0; index < _pEvent->impulse->count; index++)
			{
				totalImpulse += _pEvent->impulse->normalImpulses[index];
			}

			if (totalImpulse > 20.0f)
			{
				GazEngine::removeEntity(**pigIter);
			}
		}
	}
}

void CGrumpyEngine::destroy()
{
}

void CGrumpyEngine::init()
{
	Messages::registerRecipient(Box2DEvents::POST_SOLVE, this);
	Messages::registerRecipient(Events::MOUSE_BUTTON, this);
	Messages::registerRecipient(Events::MOUSE_MOVE, this);

	spawnNextBird();
}

void CGrumpyEngine::moveBird(const MouseMoveEvent* _pEvent)
{
	Vector2 position;
	position.x = static_cast<float>(_pEvent->x);
	position.y = static_cast<float>(_pEvent->y);

	m_pBird->getSingleComponent<CBox2DBody>()->SetPosition(position);
}

void CGrumpyEngine::receive(unsigned short subject, const void* message)
{
	if (subject == Box2DEvents::POST_SOLVE)
	{
		checkPigCollisions(static_cast<const TBox2DEvent*>(message));
	}
	else if (subject == Events::MOUSE_BUTTON)
	{
		slingBird(static_cast<const MouseButtonEvent*>(message));
	}
	else if (subject == Events::MOUSE_MOVE)
	{
		if (m_birdState == SLINGING)
		{
			moveBird(static_cast<const MouseMoveEvent*>(message));
		}
	}
}

void CGrumpyEngine::removeEntity(const Entity& entity)
{
	if (entity.getCategory() == EntityCategories::PIG)
	{
		m_pigs.erase(remove(m_pigs.begin(), m_pigs.end(), &entity));
	}
}

void CGrumpyEngine::slingBird(const MouseButtonEvent* _pEvent)
{
	if (_pEvent->button == Mouse::LEFT)
	{
		if (_pEvent->buttonState == Button::DOWN)
		{
			Vector2 mousePosition(static_cast<float>(_pEvent->x), static_cast<float>(_pEvent->y));
			if ((mousePosition - BIRD_START_POSITION).length() < 20.0f)
			{
				m_birdState = SLINGING;
			}
		}
		else if (_pEvent->buttonState == Button::UP)
		{
			if (m_birdState == SLINGING)
			{
				m_birdState = FIRING;

				Vector2 slingPosition;
				slingPosition.x = static_cast<float>(_pEvent->x);
				slingPosition.y = static_cast<float>(_pEvent->y);
				Vector2 birdVelocity = BIRD_START_POSITION - slingPosition;
				birdVelocity *= 2.5f;

				CBox2DBody * pBirdBody = m_pBird->getSingleComponent<CBox2DBody>();
				pBirdBody->SetDynamic(true);
				pBirdBody->ApplyLinearImpulse(birdVelocity, pBirdBody->GetPosition());
			}
		}
	}
}

void CGrumpyEngine::spawnNextBird()
{
	if (!m_remainingBirds.empty())
	{
		m_pBird = m_remainingBirds.front();
		m_remainingBirds.pop_front();

		GazEngine::addEntity(m_pBird);
		m_birdState = READY;
	}
}
