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

#include <PxPhysicsAPI.h>

#include <gazengine/engine/Engine.h>
#include <gazengine/Messages.h>
#include <gazengine/model/Text.h>
#include <gazengine/physics/PhysXBody.h>

class CSlingshotEngine : public Engine, public Messages::Recipient
{
	public:
		CSlingshotEngine();

		void addEntity(Entity* _pEntity);

		void advance();

		void destroy();

		void init();

		void receive(unsigned short _usSubject, const void* _kpMessage);

		void removeEntity(const Entity& _krEntity);

	private:
		bool m_bAiming;

		bool m_bPlaying;

		float m_fPitch;

		float m_fPull;

		int m_iMouseX;

		int m_iMouseY;

		PhysXBody* m_pArm0Body;

		physx::PxD6Joint* m_pArm0Joint;

		PhysXBody* m_pArm1Body;

		physx::PxD6Joint* m_pArm1Joint;

		PhysXBody* m_pBirdBody;

		SimpleTree* m_pSlingshotBaseNode;

		unsigned int m_uiBirdCount;

		void buildSlingshot();

		void loadSlingshot();
};
