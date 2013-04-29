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
#pragma once

#include <list>

#include <gazengine/engine/Engine.h>
#include <gazengine/input/MouseButtonEvent.h>
#include <gazengine/input/MouseMoveEvent.h>
#include <gazengine/Messages.h>

#include "Bird.h"
#include "Box2DBody.h"
#include "Box2DEvents.h"

class CGrumpyEngine : public Engine, public Messages::Recipient
{
	public:
		typedef enum EBirdState
		{
			COMPLETE,
			FIRING,
			READY,
			SLINGING
		} EBirdState;

		CGrumpyEngine();

		void addBird(Entity* bird);

		void addEntity(Entity* entity);

		void advance();

		void checkPigCollisions(const TBox2DEvent* _pEvent);

		void destroy();

		void init();

		void moveBird(const MouseMoveEvent* _pEvent);

		void receive(unsigned short subject, const void* message);

		void removeEntity(const Entity& entity);

		void slingBird(const MouseButtonEvent* _pEvent);

	private:
		EBirdState m_birdState;

		float m_fFuseTime;

		Entity* m_pBird;

		std::list<Entity*> m_pigs;

		std::list<Entity*> m_remainingBirds;

		void spawnNextBird();
};
