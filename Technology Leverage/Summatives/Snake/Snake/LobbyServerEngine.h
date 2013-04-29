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

class CLobbyServerEngine : public Engine
{
	public:
		static const unsigned short MAX_PLAYERS = 16;

		static const unsigned int PORT = 15082;//12082;

		CLobbyServerEngine(const std::string& _name);

		void addEntity(Entity* entity);

		void advance();

		void destroy();

		void init();

		void removeEntity(const Entity& entity);

	private:
		std::string m_name;

		RakNet::RakPeerInterface* m_peer;

		std::map<std::string, RakNet::SystemAddress> m_players;
};
