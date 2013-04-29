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
#include <gazengine/Messages.h>
#include <gazengine/physics/Body.h>

class CDriverEngine : public Engine, public Messages::Recipient
{
	public:
		CDriverEngine(Body* _pBody, float _fSpeed);

		void addEntity(Entity* _pEntity);

		void advance();

		void destroy();

		void init();

		void receive(unsigned short _usSubject, const void* _kpMessage);

		void removeEntity(const Entity& _krEntity);

	private:
		bool m_bMovingBackward;

		bool m_bMovingForward;

		bool m_bTurningLeft;

		bool m_bTurningRight;

		float m_fSpeed;

		Body* m_pBody;
};
