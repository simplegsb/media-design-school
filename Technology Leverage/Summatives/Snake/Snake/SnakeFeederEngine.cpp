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

#include <gazengine/Componentizer.h>
#include <gazengine/GazEngine.h>
#include <gazengine/gdi/model/GDICircle.h>
#include <gazengine/math/Intersection.h>
#include <gazengine/math/Math.h>

#include "EntityCategories.h"
#include "SnakeCharmer.h"
#include "SnakeFeederEngine.h"
#include "SnakeModel.h"

CSnakeFeederEngine::CSnakeFeederEngine() :
	m_pFoodModel(NULL),
	m_players(),
	m_uiFoodScore(1)
{
}

void CSnakeFeederEngine::addEntity(Entity* _pEntity)
{
	if (_pEntity->getCategory() == EntityCategories::SERVER_PLAYER)
	{
		m_players.push_back(_pEntity);
	}
}

void CSnakeFeederEngine::advance()
{
	for (unsigned int uiIndex = 0; uiIndex < m_players.size(); uiIndex++)
	{
		CSnakeModel* pModel = m_players[uiIndex]->getSingleComponent<CSnakeModel>();

		if (Intersection::intersect(pModel->getHeadBox(), *m_pFoodModel))
		{
			pModel->grow();
			m_players[uiIndex]->getSingleComponent<Componentizer<unsigned int> >()->getValue()
				+= m_uiFoodScore;
			GazEngine::removeEntity(*m_pFoodModel->getEntity());

			m_pFoodModel = NULL;
			m_uiFoodScore++;

			break;
		}
	}

	if (m_pFoodModel == NULL)
	{
		for (unsigned int uiIndex = 0; uiIndex < m_players.size(); uiIndex++)
		{
			m_players[uiIndex]->getSingleComponent<CSnakeCharmer>()->speedUp();
		}

		Entity* pFood = new Entity(EntityCategories::SERVER_FOOD);
		m_pFoodModel = new GDICircle(Vector2(Math::getRandomFloat(100.0f, 700.0f),
			Math::getRandomFloat(100.0f, 500.0f)), 5.0f);
		m_pFoodModel->setVisible(false);
		pFood->addComponent(m_pFoodModel);
		m_pFoodModel->setEntity(pFood);

		GazEngine::addEntity(pFood);
	}
}

void CSnakeFeederEngine::destroy()
{
}

void CSnakeFeederEngine::init()
{
}

void CSnakeFeederEngine::removeEntity(const Entity& _krEntity)
{
	if (_krEntity.getCategory() == EntityCategories::SERVER_PLAYER)
	{
		m_players.erase(remove(m_players.begin(), m_players.end(), &_krEntity));
	}
}
