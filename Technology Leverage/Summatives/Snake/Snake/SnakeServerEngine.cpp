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

#include <gazengine/Componentizer.h>
#include <gazengine/GazEngine.h>

#include "EntityCategories.h"
#include "GameState.h"
#include "MessageIdentifiers.h"
#include "NetworkEntity.h"
#include "ScoreBoard.h"
#include "SnakeConstants.h"
#include "SnakeEvents.h"
#include "SnakeModel.h"
#include "SnakeServerEngine.h"

using namespace std;

CSnakeServerEngine::CSnakeServerEngine(const string& _krName) :
	m_bPlaying(false),
	m_name(_krName),
	m_networkIdManager(),
	m_pFood(NULL),
	m_playerAddresses(),
	m_players(),
	m_pPeer(NULL)
{
}

void CSnakeServerEngine::addEntity(Entity* _pEntity)
{
	if (_pEntity->getCategory() == EntityCategories::SERVER_FOOD)
	{
		m_pFood = _pEntity->getSingleComponent<Circle>();
	}
}

RakNet::NetworkID CSnakeServerEngine::addPlayer(const string& _krName, RakNet::SystemAddress& _rAddress)
{
	CNetworkEntity* pPlayer = new CNetworkEntity(EntityCategories::SERVER_PLAYER, _krName);
	pPlayer->SetNetworkIDManager(&m_networkIdManager);
	m_playerAddresses[pPlayer->GetNetworkID()] = _rAddress;
	m_players[pPlayer->GetNetworkID()] = pPlayer;

	updatePongData();

	return pPlayer->GetNetworkID();
}

void CSnakeServerEngine::advance()
{
	receivePackets();

	if (m_bPlaying)
	{
		packAndSendGameState(*m_pPeer, m_players, m_pFood);
	}
}

void CSnakeServerEngine::destroy()
{
	// Network entities need to be removed (and more specifically destructed) before
	// the peer is shutdown.
	/* NOT WORKING! Quick hack used in GazEngine::play() instead.
	for (map<RakNet::NetworkID, Entity*>::iterator playerIter = m_players.begin();
		playerIter != m_players.end(); playerIter++)
	{
		GazEngine::removeEntity(*playerIter->second);
	}*/

	Messages::deregisterRecipient(SnakeServerEvents::DEATH, this);
	Messages::deregisterRecipient(SnakeServerEvents::GAME_START, this);

	m_pPeer->Shutdown(1000);
	RakNet::RakPeerInterface::DestroyInstance(m_pPeer);

	ScoreBoard::DisconnectFromHighScores();
}

void CSnakeServerEngine::init()
{
	ScoreBoard::ConnectToHighScores();

	m_pPeer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor socketDescriptor(SnakeConstants::SERVER_PORT, NULL);
	m_pPeer->Startup(SnakeConstants::MAX_PLAYERS, &socketDescriptor, 1);
	m_pPeer->SetMaximumIncomingConnections(SnakeConstants::MAX_PLAYERS);

	updatePongData();

	Messages::registerRecipient(SnakeServerEvents::DEATH, this);
	Messages::registerRecipient(SnakeServerEvents::GAME_START, this);
}

void CSnakeServerEngine::receive(unsigned short _usSubject, const void* _kpMessage)
{
	if (_usSubject == SnakeServerEvents::DEATH)
	{
		const CNetworkEntity* pPlayer = static_cast<const CNetworkEntity*>(_kpMessage);

		RakNet::BitStream deathStream;
		deathStream.Write((RakNet::MessageID) MessageIdentifiers::ID_SNAKE_DEATH);
		// Naughty!!!
		deathStream.Write(const_cast<CNetworkEntity*>(pPlayer)->GetNetworkID());

		m_pPeer->Send(&deathStream, IMMEDIATE_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	else if (_usSubject == SnakeServerEvents::GAME_START)
	{
		m_pPeer->SetMaximumIncomingConnections(static_cast<unsigned short>(m_players.size()));
		float fPositionY = 100.0f;

		for (map<RakNet::NetworkID, Entity*>::iterator playerIter = m_players.begin();
			playerIter != m_players.end(); playerIter++)
		{
			Entity* pPlayer = playerIter->second;

			CSnakeCharmer* pSnakeCharmer = new CSnakeCharmer(5.0f);
			pPlayer->addComponent(pSnakeCharmer);
			pSnakeCharmer->setEntity(pPlayer);

			CSnakeModel* pModel = new CSnakeModel(5, 5, Vector2(fPositionY, 400.0f), 10.0f, RGB(0, 0, 0));
			pModel->setVisible(false);
			pPlayer->addComponent(pModel);
			pModel->setEntity(pPlayer);

			Componentizer<unsigned int>* pScore = new Componentizer<unsigned int>();
			pPlayer->addComponent(pScore);
			pScore->setEntity(pPlayer);

			GazEngine::addEntity(pPlayer);

			fPositionY += 100.0f;
		}

		m_bPlaying = true;

		sendGameStart();
	}
}

void CSnakeServerEngine::receivePackets()
{
	for (RakNet::Packet* pPacket = m_pPeer->Receive(); pPacket != NULL; pPacket = m_pPeer->Receive())
	{
		if (pPacket->data[0] == MessageIdentifiers::ID_SNAKE_INPUT)
		{
			RakNet::BitStream inputStream(pPacket->data, pPacket->length, false);
			inputStream.IgnoreBytes(1);
			RakNet::NetworkID playerId;
			inputStream.Read(playerId);
			CSnakeCharmer::EDirection direction;
			inputStream.Read(direction);

			if (m_players.find(playerId) != m_players.end())
			{
				m_players[playerId]->getSingleComponent<CSnakeCharmer>()->setDirection(direction);
			}
		}
		else if (pPacket->data[0] == MessageIdentifiers::ID_SNAKE_JOIN)
		{
			RakNet::BitStream joinStream(pPacket->data, pPacket->length, false);
			joinStream.IgnoreBytes(1);
			unsigned int uiPlayerNameLength;
			joinStream.Read(uiPlayerNameLength);
			string playerName;
			playerName.resize(uiPlayerNameLength);
			joinStream.Read(&playerName[0], uiPlayerNameLength);

			RakNet::NetworkID playerId = addPlayer(playerName, pPacket->systemAddress);

			RakNet::BitStream joinConfirmStream;
			joinConfirmStream.Write((RakNet::MessageID) MessageIdentifiers::ID_SNAKE_JOIN_CONFIRM);
			joinConfirmStream.Write(playerId);

			m_pPeer->Send(&joinConfirmStream, IMMEDIATE_PRIORITY, RELIABLE, 0, m_playerAddresses[playerId], false);
		}
		else if (pPacket->data[0] == MessageIdentifiers::ID_SNAKE_LEAVE)
		{
			RakNet::BitStream leaveStream(pPacket->data, pPacket->length, false);
			leaveStream.IgnoreBytes(1);
			RakNet::NetworkID playerId;
			leaveStream.Read(playerId);

			removePlayer(playerId);
		}

		m_pPeer->DeallocatePacket(pPacket);
	}
}

void CSnakeServerEngine::removeEntity(const Entity& _krEntity)
{
	if (_krEntity.getCategory() == EntityCategories::SERVER_FOOD)
	{
		m_pFood = NULL;
	}
	else if (_krEntity.getCategory() == EntityCategories::SERVER_PLAYER)
	{
		const CNetworkEntity& krNetworkEntity = static_cast<const CNetworkEntity&>(_krEntity);
		// Naughty!!!
		RakNet::NetworkID playerId = const_cast<CNetworkEntity&>(krNetworkEntity).GetNetworkID();

		m_playerAddresses.erase(m_playerAddresses.find(playerId));
		m_players.erase(m_players.find(playerId));
	}
}

void CSnakeServerEngine::removePlayer(RakNet::NetworkID playerId)
{
	GazEngine::removeEntity(*m_players[playerId]);

	updatePongData();
}

void CSnakeServerEngine::sendGameStart()
{
	RakNet::BitStream stream;
	stream.Write((RakNet::MessageID) MessageIdentifiers::ID_SNAKE_GAME_START);
	stream.Write(m_players.size());

	for (map<RakNet::NetworkID, Entity*>::iterator playerIter = m_players.begin();
		playerIter != m_players.end(); playerIter++)
	{
		stream.Write(playerIter->first);
		stream.Write(playerIter->second->getName().size());
		stream.Write(playerIter->second->getName().c_str(), playerIter->second->getName().size());
	}

	m_pPeer->Send(&stream, IMMEDIATE_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void CSnakeServerEngine::updatePongData()
{
	string pongData;

	unsigned int uiServerNameLength = m_name.size();
	pongData.append(reinterpret_cast<char*>(&uiServerNameLength), sizeof(unsigned int));
	pongData.append(m_name);

	unsigned int uiPlayerCount = m_players.size();
	pongData.append(reinterpret_cast<char*>(&uiPlayerCount), sizeof(unsigned int));

	for (map<RakNet::NetworkID, Entity*>::iterator playerIter = m_players.begin();
		playerIter != m_players.end(); playerIter++)
	{
		unsigned int uiPlayerNameLength = playerIter->second->getName().size();
		pongData.append(reinterpret_cast<char*>(&uiPlayerNameLength), sizeof(unsigned int));
		pongData.append(playerIter->second->getName());
	}

	vector<ScoreBoard::TScore> highScores = ScoreBoard::GetHighScores();
	unsigned int uiHighScoreCount = highScores.size();
	pongData.append(reinterpret_cast<char*>(&uiHighScoreCount), sizeof(unsigned int));

	for (unsigned int uiIndex = 0; uiIndex < uiHighScoreCount; uiIndex++)
	{
		unsigned int uiPlayerNameLength = highScores[uiIndex].playerName.size();
		pongData.append(reinterpret_cast<char*>(&uiPlayerNameLength), sizeof(unsigned int));
		pongData.append(highScores[uiIndex].playerName);
		pongData.append(reinterpret_cast<char*>(&highScores[uiIndex].score), sizeof(unsigned int));
	}

	m_pPeer->SetOfflinePingResponse(pongData.c_str(), pongData.size());
}
