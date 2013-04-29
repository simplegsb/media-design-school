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
#include <gazengine/model/Circle.h>

class CSnakeFeederEngine : public Engine
{
	public:
		CSnakeFeederEngine();

		void addEntity(Entity* _pEntity);

		void advance();

		void destroy();

		void init();

		void removeEntity(const Entity& _krEntity);

	private:
		Circle* m_pFoodModel;

		std::vector<Entity*> m_players;

		unsigned int m_uiFoodScore;
};
