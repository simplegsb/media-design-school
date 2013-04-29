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
#include <algorithm>

#include <BitStream.h>
#include <RakSleep.h>

#include "MessageIdentifiers.h"
#include "LobbyServerEngine.h"

CLobbyServerEngine::CLobbyServerEngine(const std::string& _name) :
	m_name(_name),
	m_peer(NULL)
{
}

void CLobbyServerEngine::addEntity(Entity* entity)
{
}

void CLobbyServerEngine::advance()
{
	for (RakNet::Packet* pPacket = m_peer->Receive(); pPacket != NULL; pPacket = m_peer->Receive())
	{
		if (pPacket->data[0] == MessageIdentifiers::ID_SNAKE_JOIN)
		{
			RakNet::BitStream bitStream(pPacket->data, pPacket->length, false);
			bitStream.IgnoreBytes(1);

			char playerNameRaw[128];
			bitStream.Read(playerNameRaw, 128);

			std::string playerName(playerNameRaw);
			m_players[playerName] = pPacket->systemAddress;
		}

		m_peer->DeallocatePacket(pPacket);
	}
}

void CLobbyServerEngine::destroy()
{
}

void CLobbyServerEngine::init()
{
	m_peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor socketDescriptor(PORT, 0);
	m_peer->Startup(MAX_PLAYERS, &socketDescriptor, 1);
	m_peer->SetMaximumIncomingConnections(MAX_PLAYERS);
	m_peer->SetOfflinePingResponse(m_name.c_str(), m_name.size());
}

void CLobbyServerEngine::removeEntity(const Entity& entity)
{
}
