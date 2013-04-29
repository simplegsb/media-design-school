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
#pragma once

#include <gazengine/engine/Engine.h>

class CBirdEngine : public Engine
{
	public:
		CBirdEngine(float _fExplosionRange, float _fExplosionTime);

		void addEntity(Entity* _pEntity);

		void advance();

		void destroy();

		void init();

		void removeEntity(const Entity& _krEntity);

	private:
		std::vector<Entity*> m_birds;

		float m_fExplosionRange;

		float m_fExplosionTime;

		void explode(SimpleTree& _rNode, const Entity* _pBird, const Vector3& birdPosition);
};
