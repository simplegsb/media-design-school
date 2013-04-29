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

#include <RakPeerInterface.h>

#include <gazengine/engine/Engine.h>
#include <gazengine/Messages.h>

class CLobbyClientEngine : public Engine, public Messages::Recipient
{
	public:
		CLobbyClientEngine();

		void addEntity(Entity* entity);

		void advance();

		void destroy();

		void init();

		void receive(const std::string& subject, const void* message);

		void removeEntity(const Entity& entity);

	private:
		std::map<std::string, RakNet::SystemAddress> m_detectedServers;

		float m_fTimeSinceServerPing;

		RakNet::RakPeerInterface* m_peer;

		std::string m_playerName;

		RakNet::SystemAddress m_server;
};
