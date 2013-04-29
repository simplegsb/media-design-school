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
#include <gazengine/Componentizer.h>
#include <gazengine/GazEngine.h>
#include <gazengine/Messages.h>

#include "EntityCategories.h"
#include "ScoreBoard.h"
#include "SnakeEvents.h"
#include "SnakeWranglerEngine.h"

using namespace std;

CSnakeWranglerEngine::CSnakeWranglerEngine()
{
}

void CSnakeWranglerEngine::addEntity(Entity* _pEntity)
{
	if (_pEntity->getCategory() == EntityCategories::SERVER_PLAYER)
	{
		m_snakeModels.push_back(_pEntity->getSingleComponent<CSnakeModel>());
	}
}

void CSnakeWranglerEngine::advance()
{
	vector<const Entity*> toBeWrangled;

	for (unsigned int uiSnakeIndex = 0; uiSnakeIndex < m_snakeModels.size(); uiSnakeIndex++)
	{
		AABB2 snakeHead = m_snakeModels[uiSnakeIndex]->getHeadBox();

		if (isOutsideOfMap(snakeHead) || isCollidingWithSnake(snakeHead, uiSnakeIndex))
		{
			toBeWrangled.push_back(m_snakeModels[uiSnakeIndex]->getEntity());
		}
	}

	for (unsigned int uiWrangleIndex = 0; uiWrangleIndex < toBeWrangled.size(); uiWrangleIndex++)
	{
		string playerName = toBeWrangled[uiWrangleIndex]->getName();
		unsigned int uiScore =
			toBeWrangled[uiWrangleIndex]->getSingleComponent<Componentizer<unsigned int> >()->getValue();
		ScoreBoard::AddToHighScores(ScoreBoard::TScore(playerName, uiScore));

		Messages::send(SnakeServerEvents::DEATH, toBeWrangled[uiWrangleIndex]);
		GazEngine::removeEntity(*toBeWrangled[uiWrangleIndex]);
	}
}

void CSnakeWranglerEngine::destroy()
{
}

void CSnakeWranglerEngine::init()
{
}

bool CSnakeWranglerEngine::isCollidingWithSnake(const AABB2& _krSnakeHead, unsigned int _uiSnakeIndex)
{
	for (unsigned int uiOtherSnakeIndex = 0; uiOtherSnakeIndex < m_snakeModels.size(); uiOtherSnakeIndex++)
	{
		vector<AABB2> boxes = m_snakeModels[uiOtherSnakeIndex]->getBoxes();
		for (unsigned int uiSegmentIndex = 0; uiSegmentIndex < boxes.size(); uiSegmentIndex++)
		{
			// It would be silly to collide with one's own head.
			if (uiOtherSnakeIndex == _uiSnakeIndex &&
				uiSegmentIndex == 0)
			{
				continue;
			}

			if (intersect(_krSnakeHead, boxes[uiSegmentIndex]))
			{
				return true;
			}
		}
	}

	return false;
}

bool CSnakeWranglerEngine::intersect(const AABB2& _krA, const AABB2& _krB)
{
	float fAMaxX = _krA.center.x + _krA.halfDimension;
	float fAMinX = _krA.center.x - _krA.halfDimension;
	float fAMaxY = _krA.center.y + _krA.halfDimension;
	float fAMinY = _krA.center.y - _krA.halfDimension;

	float fBMaxX = _krB.center.x + _krB.halfDimension;
	float fBMinX = _krB.center.x - _krB.halfDimension;
	float fBMaxY = _krB.center.y + _krB.halfDimension;
	float fBMinY = _krB.center.y - _krB.halfDimension;

	if (fAMinX >= fBMaxX || fBMinX >= fAMaxX || fAMinY >= fBMaxY || fBMinY >= fAMaxY)
	{
		return false;
	}

	return true;
}

bool CSnakeWranglerEngine::isOutsideOfMap(const AABB2& _krSnakeHead)
{
	return _krSnakeHead.center.x < 0.0f
		|| _krSnakeHead.center.y < 0.0f
		|| _krSnakeHead.center.x > 800.0f
		|| _krSnakeHead.center.y > 600.0f;
}

void CSnakeWranglerEngine::removeEntity(const Entity& _krEntity)
{
	if (_krEntity.getCategory() == EntityCategories::SERVER_PLAYER)
	{
		for (std::vector<CSnakeModel*>::iterator snakeModelIter = m_snakeModels.begin();
			snakeModelIter != m_snakeModels.end(); snakeModelIter++)
		{
			if ((*snakeModelIter)->getEntity() == &_krEntity)
			{
				m_snakeModels.erase(snakeModelIter);
				return;
			}
		}
	}
}
