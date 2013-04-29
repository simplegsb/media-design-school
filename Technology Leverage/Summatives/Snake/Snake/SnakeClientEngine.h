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

#include <map>

#include <NetworkIDManager.h>
#include <RakPeerInterface.h>

#include <gazengine/engine/Engine.h>
#include <gazengine/Messages.h>

#include "GameState.h"
#include "NetworkEntity.h"
#include "ServerDetails.h"

class CSnakeClientEngine : public Engine, public Messages::Recipient
{
	public:
		CSnakeClientEngine();

		void addEntity(Entity* _pEntity);

		void advance();

		void destroy();

		void init();

		void receive(unsigned short _usSubject, const void* _kpMessage);

		void removeEntity(const Entity& _krEntity);

	private:
		bool m_bPlaying;

		std::map<std::string, TServerDetails> m_detectedServers;

		float m_fTimeSinceServerPing;

		RakNet::NetworkIDManager m_networkIdManager;

		Entity* m_pFood;

		RakNet::NetworkID m_playerId;

		std::string m_playerName;

		std::map<RakNet::NetworkID, CNetworkEntity*> m_players;

		RakNet::RakPeerInterface* m_pPeer;

		Entity* m_pPlayer;

		RakNet::SystemAddress m_server;

		void createSnakes(RakNet::Packet& _rPacket);

		void receivePackets();

		void removePlayer(RakNet::Packet& _rPacket);

		void sendInput();

		void sendJoin();

		TServerDetails unpackServerDetails(RakNet::Packet& _rPacket);

		void updateFood(const TGameState& _krGameState);

		void updateSnakes(const TGameState& _krGameState);
};
