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
#include <algorithm>

#include "EntityCategories.h"
#include "SnakeCharmingEngine.h"

CSnakeCharmingEngine::CSnakeCharmingEngine() :
	m_snakeCharmers()
{
}

void CSnakeCharmingEngine::addEntity(Entity* _pEntity)
{
	if (_pEntity->getCategory() == EntityCategories::SERVER_PLAYER)
	{
		m_snakeCharmers.push_back(_pEntity->getSingleComponent<CSnakeCharmer>());
	}
}

void CSnakeCharmingEngine::advance()
{
	for (unsigned int uiIndex = 0; uiIndex < m_snakeCharmers.size(); uiIndex++)
	{
		m_snakeCharmers[uiIndex]->charm();
	}
}

void CSnakeCharmingEngine::destroy()
{
}

void CSnakeCharmingEngine::init()
{
}

void CSnakeCharmingEngine::removeEntity(const Entity& _krEntity)
{
	if (_krEntity.getCategory() == EntityCategories::SERVER_PLAYER)
	{
		for (std::vector<CSnakeCharmer*>::iterator snakeCharmerIter = m_snakeCharmers.begin();
			snakeCharmerIter != m_snakeCharmers.end(); snakeCharmerIter++)
		{
			if ((*snakeCharmerIter)->getEntity() == &_krEntity)
			{
				m_snakeCharmers.erase(snakeCharmerIter);
				return;
			}
		}
	}
}
