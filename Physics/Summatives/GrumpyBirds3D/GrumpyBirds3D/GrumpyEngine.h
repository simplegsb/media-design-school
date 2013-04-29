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
#include <gazengine/model/Text.h>

class CGrumpyEngine : public Engine, public Messages::Recipient
{
	public:
		CGrumpyEngine();

		void addEntity(Entity* _pEntity);

		void advance();

		void destroy();

		void init();

		void receive(unsigned short _usSubject, const void* _kpMessage);

		void removeEntity(const Entity& _krEntity);

	private:
		bool m_bGameEnded;

		bool m_bGameRestarted;

		bool m_bGameStarted;

		float m_fTimeLeft;

		float m_fTimeLimit;

		Text* m_pMessageText;

		unsigned int m_uiBirdCount;

		unsigned int m_uiPigCount;

		void buildScene();

		void restartGame();

		void startGame();
};
