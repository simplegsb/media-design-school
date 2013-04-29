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

#include <NetworkIDManager.h>
#include <RakPeerInterface.h>

#include <gazengine/engine/Engine.h>
#include <gazengine/Messages.h>
#include <gazengine/model/Circle.h>

#include "SnakeCharmer.h"
#include "SnakeModel.h"

class CSnakeServerEngine : public Engine, public Messages::Recipient
{
	public:
		CSnakeServerEngine(const std::string& _krName);

		void addEntity(Entity* _pEntity);

		void advance();

		void destroy();

		void init();

		void receive(unsigned short _usSubject, const void* _kpMessage);

		void removeEntity(const Entity& _krEntity);

	private:
		bool m_bPlaying;

		std::string m_name;

		RakNet::NetworkIDManager m_networkIdManager;

		Circle* m_pFood;

		std::map<RakNet::NetworkID, RakNet::SystemAddress> m_playerAddresses;

		std::map<RakNet::NetworkID, Entity*> m_players;

		RakNet::RakPeerInterface* m_pPeer;

		RakNet::NetworkID addPlayer(const std::string& _krName, RakNet::SystemAddress& _rAddress);

		void receivePackets();

		void removePlayer(RakNet::NetworkID playerId);

		void sendGameStart();

		void updatePongData();
};
