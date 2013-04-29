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
#include <sstream>

#include <gazengine/Events.h>
#include <gazengine/GazEngine.h>
#include <gazengine/input/MouseButtonEvent.h>
#include <gazengine/math/MathFunctions.h>

#include "EntityCategories.h"
#include "Events.h"
#include "GrumpyEngine.h"
#include "SceneFactory.h"

using namespace physx;

CGrumpyEngine::CGrumpyEngine() :
	m_bGameEnded(false),
	m_bGameRestarted(false),
	m_bGameStarted(false),
	m_fTimeLeft(30),
	m_fTimeLimit(30),
	m_pMessageText(NULL),
	m_uiBirdCount(10),
	m_uiPigCount(0)
{
}

void CGrumpyEngine::addEntity(Entity* _pEntity)
{
	if (_pEntity->getCategory() == EntityCategories::PIG)
	{
		m_uiPigCount++;
	}
}

void CGrumpyEngine::advance()
{
	if (m_bGameStarted && !m_bGameEnded)
	{
		if (m_uiPigCount == 0)
		{
			if (m_bGameRestarted)
			{
				m_bGameRestarted = false;
				return;
			}

			m_pMessageText->setText("Congratulations, you've killed all the pigs! Click the mouse to play again.");
			m_bGameEnded = true;
		}
		else if (m_uiBirdCount == 0)
		{
			m_fTimeLeft -= GazEngine::getDeltaTime();

			stringstream timeLeft;
			timeLeft << "You've run out of birds! Time left: ";
			timeLeft << m_fTimeLeft;
			m_pMessageText->setText(timeLeft.str());

			if (m_fTimeLeft <= 0.0f)
			{
				m_pMessageText->setText("Nice try but you didn't kill all the pigs! Click the mouse to play again.");
				m_bGameEnded = true;
			}
		}
	}
}

void CGrumpyEngine::buildScene()
{
	SceneFactory::CreateScene();

	// Pigs
	Matrix44 transformation;
	setTranslation(transformation, Vector3(-15.0f, -6.0f, -60.0f));
	SceneFactory::CreatePig(transformation, 2.0f);
	setTranslation(transformation, Vector3(-15.0f, 6.0f, -60.0f));
	SceneFactory::CreatePig(transformation, 2.0f);
	setTranslation(transformation, Vector3(15.0f, -6.0f, -60.0f));
	SceneFactory::CreatePig(transformation, 2.0f);
	setTranslation(transformation, Vector3(15.0f, 6.0f, -60.0f));
	SceneFactory::CreatePig(transformation, 2.0f);

	// Message
	Entity* pMessages = new Entity;
	m_pMessageText = new Text(Vector2(0.0f, 0.0f), "Welcome to Grumpy Birds! Click the mouse to start!");
	pMessages->addComponent(m_pMessageText);
	m_pMessageText->setEntity(pMessages);
	GazEngine::addEntity(pMessages);

	SimpleTree* pNode = new SimpleTree;
	pNode->setModel(m_pMessageText);
	setTranslation(pNode->getTransformation(), Vector3(0.0f, 5.0f, 0.0f));
	GazEngine::addToWorld(pNode);
}

void CGrumpyEngine::destroy()
{
	Messages::deregisterRecipient(Events::BIRD_RELEASED, this);
	Messages::deregisterRecipient(Events::MOUSE_BUTTON, this);
}

void CGrumpyEngine::init()
{
	Messages::registerRecipient(Events::BIRD_RELEASED, this);
	Messages::registerRecipient(Events::MOUSE_BUTTON, this);

	buildScene();
}

void CGrumpyEngine::receive(unsigned short _usSubject, const void* _kpMessage)
{
	if (_usSubject == Events::BIRD_RELEASED)
	{
		m_uiBirdCount--;
		return;
	}

	const MouseButtonEvent* kpEvent = static_cast<const MouseButtonEvent*>(_kpMessage);

	if (!m_bGameStarted)
	{
		if (kpEvent->buttonState == Button::UP)
		{
			startGame();
		}

		return;
	}
	else if (m_bGameEnded)
	{
		if (kpEvent->buttonState == Button::UP)
		{
			restartGame();
		}

		return;
	}
}

void CGrumpyEngine::removeEntity(const Entity& _krEntity)
{
	if (_krEntity.getCategory() == EntityCategories::PIG)
	{
		m_uiPigCount--;
	}
}

void CGrumpyEngine::restartGame()
{
	m_bGameEnded = false;

	GazEngine::getWorldRepresentations()[0]->getChildren().clear();
	GazEngine::removeAllEntities();

	buildScene();
	m_bGameRestarted = true;

	startGame();
}

void CGrumpyEngine::startGame()
{
	m_pMessageText->setText("Scroll the mouse to pull back, hold the left mouse button down to aim and fire.");
	m_bGameStarted = true;
	m_fTimeLeft = m_fTimeLimit;
	m_uiBirdCount = 10;
	Messages::send(Events::GAME_START, NULL);
}
