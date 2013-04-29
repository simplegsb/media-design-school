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
#include <gazengine/graph/SimpleTree.h>
#include <gazengine/Messages.h>

class CGunnerEngine : public Engine, public Messages::Recipient
{
	public:
		CGunnerEngine(SimpleTree* _pTurretNode, SimpleTree* _pBarrelNode, float speed);

		void addEntity(Entity* entity);

		void advance();

		void destroy();

		void init();

		void receive(unsigned short subject, const void* message);

		void removeEntity(const Entity& entity);

	private:
		float m_fMouseX;

		float m_fMouseY;

		float m_fSpeed;

		SimpleTree* m_pBarrelNode;

		SimpleTree* m_pTurretNode;
};
