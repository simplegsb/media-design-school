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
#pragma once

#include <gazengine/engine/Engine.h>

#include "SnakeModel.h"

class CSnakeWranglerEngine : public Engine
{
	public:
		CSnakeWranglerEngine();

		void addEntity(Entity* _pEntity);

		void advance();

		void destroy();

		void init();

		void removeEntity(const Entity& _krEntity);

	private:
		std::vector<CSnakeModel*> m_snakeModels;

		bool intersect(const AABB2& _krA, const AABB2& _krB);

		bool isCollidingWithSnake(const AABB2& _krSnakeHead, unsigned int _uiSnakeIndex);

		bool isOutsideOfMap(const AABB2& _krSnakeHead);
};
