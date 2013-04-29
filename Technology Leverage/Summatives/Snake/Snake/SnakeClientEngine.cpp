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
#include <sstream>

#include <BitStream.h>

#include <gazengine/Componentizer.h>
#include <gazengine/Events.h>
#include <gazengine/GazEngine.h>
#include <gazengine/input/KeyboardButtonEvent.h>
#include <gazengine/gdi/model/GDICircle.h>

#include "EntityCategories.h"
#include "MessageIdentifiers.h"
#include "ScoreBoard.h"
#include "ServerBrowser.h"
#include "SnakeClientEngine.h"
#include "SnakeConstants.h"
#include "SnakeEvents.h"
#include "SnakeModel.h"
#include "SnakeServerEngine.h"

using namespace std;

CSnakeClientEngine::CSnakeClientEngine() :
	m_bPlaying(false),
	m_detectedServers(),
	m_fTimeSinceServerPing(0.0f),
	m_networkIdManager(),
	m_pFood(NULL),
	m_playerId(0),
	m_playerName(),
	m_players(),
	m_pPeer(NULL),
	m_pPlayer(NULL),
	m_server()
{
}

void CSnakeClientEngine::addEntity(Entity*)
{
}

void CSnakeClientEngine::advance()
{
	receivePackets();

	if (m_bPlaying)
	{
		if (m_pPlayer->getSingleComponent<CSnakeCharmer>() != NULL)
		{
			sendInput();
		}
	}
	else
	{
		m_fTimeSinceServerPing += GazEngine::getDeltaTime();
		if (m_fTimeSinceServerPing > SnakeConstants::SERVER_PING_FREQUENCY)
		{
			m_pPeer->Ping(SnakeConstants::BROADCAST_ADDRESS.c_str(), SnakeConstants::SERVER_PORT, false);
			ServerBrowser::RemoveDeadServers();
			m_fTimeSinceServerPing = 0;
		}
	}
}

void CSnakeClientEngine::createSnakes(RakNet::Packet& _rPacket)
{
	RakNet::BitStream stream(_rPacket.data, _rPacket.length, false);
	stream.IgnoreBytes(1);
	unsigned int uiSnakeCount;
	stream.Read(uiSnakeCount);

	for (unsigned int uiIndex = 0; uiIndex < uiSnakeCount; uiIndex++)
	{
		RakNet::NetworkID networkId;
		stream.Read(networkId);
		unsigned int uiNameLength;
		stream.Read(uiNameLength);
		string name;
		name.resize(uiNameLength);
		stream.Read(&name[0], uiNameLength);

		CNetworkEntity* pPlayer = new CNetworkEntity(EntityCategories::CLIENT_PLAYER, name);
		pPlayer->SetNetworkIDManager(&m_networkIdManager);
		pPlayer->SetNetworkID(networkId);
		m_players[networkId] = pPlayer;

		Componentizer<unsigned int>* pScore = new Componentizer<unsigned int>(0);
		pPlayer->addComponent(pScore);
		pScore->setEntity(pPlayer);

		if (networkId == m_playerId)
		{
			m_pPlayer = pPlayer;

			CSnakeModel* pModel = new CSnakeModel(5, 5, Vector2(), 10.0f, RGB(0, 255, 0));
			pPlayer->addComponent(pModel);
			pModel->setEntity(pPlayer);

			Component* pSnakeCharmer = new CSnakeCharmer(5.0f);
			pPlayer->addComponent(pSnakeCharmer);
			pSnakeCharmer->setEntity(pPlayer);
		}
		else
		{
			CSnakeModel* pModel = new CSnakeModel(5, 5, Vector2(), 10.0f, RGB(255, 0, 0));
			pPlayer->addComponent(pModel);
			pModel->setEntity(pPlayer);
		}

		GazEngine::addEntity(pPlayer);
	}
}

void CSnakeClientEngine::destroy()
{
	Messages::deregisterRecipient(Events::KEYBOARD_BUTTON, this);
	Messages::deregisterRecipient(SnakeClientEvents::PLAYER_NAME, this);
	Messages::deregisterRecipient(SnakeClientEvents::JOIN, this);
	Messages::deregisterRecipient(SnakeClientEvents::LEAVE, this);

	m_pPeer->Shutdown(1000);
	RakNet::RakPeerInterface::DestroyInstance(m_pPeer);
}

void CSnakeClientEngine::init()
{
	m_pPeer = RakNet::RakPeerInterface::GetInstance();

	RakNet::SocketDescriptor socketDescriptor;
	m_pPeer->Startup(1, &socketDescriptor, 1);

	Messages::registerRecipient(Events::KEYBOARD_BUTTON, this);
	Messages::registerRecipient(SnakeClientEvents::PLAYER_NAME, this);
	Messages::registerRecipient(SnakeClientEvents::JOIN, this);
	Messages::registerRecipient(SnakeClientEvents::LEAVE, this);
}

void CSnakeClientEngine::receive(unsigned short _usSubject, const void* _kpMessage)
{
	if (_usSubject == Events::KEYBOARD_BUTTON)
	{
		const KeyboardButtonEvent* pEvent = static_cast<const KeyboardButtonEvent*>(_kpMessage);
		if (pEvent->button == Keyboard::TAB)
		{
			if (pEvent->buttonState == Button::DOWN)
			{
				ScoreBoard::Show();
			}
			else if (pEvent->buttonState == Button::UP)
			{
				ScoreBoard::Hide();
			}
		}
	}
	else if (_usSubject == SnakeClientEvents::PLAYER_NAME)
	{
		m_playerName = *static_cast<const string*>(_kpMessage);
	}
	else if (_usSubject == SnakeClientEvents::JOIN)
	{
		const string* pServerName = static_cast<const string*>(_kpMessage);
		m_server = m_detectedServers[*pServerName].address;
		m_pPeer->Connect(m_server.ToString(false), m_server.GetPort(), NULL, 0);
	}
	else if (_usSubject == SnakeClientEvents::LEAVE)
	{
		RakNet::BitStream stream;
		stream.Write((RakNet::MessageID) MessageIdentifiers::ID_SNAKE_LEAVE);
		stream.Write(m_playerName.c_str(), m_playerName.size());
		m_pPeer->Send(&stream, IMMEDIATE_PRIORITY, RELIABLE, 0, m_server, false);
		m_pPeer->CloseConnection(m_server, true);

		m_bPlaying = false;
	}
}

void CSnakeClientEngine::receivePackets()
{
	for (RakNet::Packet* pPacket = m_pPeer->Receive(); pPacket != NULL; pPacket = m_pPeer->Receive())
	{
		if (pPacket->data[0] == ID_CONNECTION_ATTEMPT_FAILED)
		{
			Messages::send(SnakeClientEvents::DISCONNECTED, NULL);
		}
		else if (pPacket->data[0] == ID_CONNECTION_REQUEST_ACCEPTED)
		{
			sendJoin();
		}
		else if (pPacket->data[0] == ID_DISCONNECTION_NOTIFICATION)
		{
			Messages::send(SnakeClientEvents::DISCONNECTED, NULL);
		}
		else if (pPacket->data[0] == ID_NO_FREE_INCOMING_CONNECTIONS)
		{
			Messages::send(SnakeClientEvents::NOT_CONNECTED, NULL);
		}
		else if (pPacket->data[0] == MessageIdentifiers::ID_SNAKE_DEATH)
		{
			removePlayer(*pPacket);
		}
		else if (pPacket->data[0] == MessageIdentifiers::ID_SNAKE_GAME_START)
		{
			ScoreBoard::Init();
			createSnakes(*pPacket);

			Messages::send(SnakeClientEvents::GAME_STARTED, NULL);
			m_bPlaying = true;
		}
		else if (pPacket->data[0] == MessageIdentifiers::ID_SNAKE_JOIN_CONFIRM)
		{
			RakNet::BitStream joinConfirmStream(pPacket->data, pPacket->length, false);
			joinConfirmStream.IgnoreBytes(1);
			RakNet::NetworkID playerId;
			joinConfirmStream.Read(playerId);

			m_playerId = playerId;
			Messages::send(SnakeClientEvents::CONNECTED, NULL);
		}
		else if (pPacket->data[0] == MessageIdentifiers::ID_SNAKE_STATES)
		{
			TGameState gameState = unpackGameState(*pPacket);

			updateFood(gameState);
			updateSnakes(gameState);

			ScoreBoard::Update(gameState.snakeCount, m_players, m_playerId);

			if (gameState.snakeCount == 0)
			{
				m_bPlaying = false;
			}
		}
		else if (pPacket->data[0] == ID_CONNECTED_PONG ||
			pPacket->data[0] == ID_UNCONNECTED_PONG)
		{
			TServerDetails serverDetails = unpackServerDetails(*pPacket);

			m_detectedServers[serverDetails.name] = serverDetails;
			Messages::send(SnakeClientEvents::SERVER_DETAILS, &m_detectedServers[serverDetails.name]);
		}

		m_pPeer->DeallocatePacket(pPacket);
	}
}

void CSnakeClientEngine::removeEntity(const Entity&)
{
}

void CSnakeClientEngine::removePlayer(RakNet::Packet& _rPacket)
{
	RakNet::BitStream stream(_rPacket.data, _rPacket.length, false);
	stream.IgnoreBytes(1);
	RakNet::NetworkID playerId;
	stream.Read(playerId);

	m_players[playerId]->getSingleComponent<CSnakeModel>()->setVisible(false);
}

void CSnakeClientEngine::sendInput()
{
	m_pPlayer->getSingleComponent<CSnakeCharmer>()->updateDirection();

	RakNet::BitStream stream;
	stream.Write((RakNet::MessageID) MessageIdentifiers::ID_SNAKE_INPUT);
	stream.Write(m_playerId);
	stream.Write(m_pPlayer->getSingleComponent<CSnakeCharmer>()->getDirection());

	m_pPeer->Send(&stream, IMMEDIATE_PRIORITY, UNRELIABLE, 0, m_server, false);
}

void CSnakeClientEngine::sendJoin()
{
	RakNet::BitStream stream;
	stream.Write((RakNet::MessageID) MessageIdentifiers::ID_SNAKE_JOIN);
	stream.Write(m_playerName.size());
	stream.Write(m_playerName.c_str(), m_playerName.size());
	m_pPeer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, m_server, false);
}

TServerDetails CSnakeClientEngine::unpackServerDetails(RakNet::Packet& _rPacket)
{
	TServerDetails serverDetails;
	serverDetails.address = _rPacket.systemAddress;

	RakNet::BitStream stream(_rPacket.data, _rPacket.length, false);
	stream.IgnoreBytes(1);

	RakNet::TimeMS time;
	stream.Read(time);

	unsigned int uiServerNameLength;
	stream.Read(reinterpret_cast<char*>(&uiServerNameLength), sizeof(unsigned int));
	serverDetails.name.resize(uiServerNameLength);
	stream.Read(&serverDetails.name[0], uiServerNameLength);

	unsigned int uiPlayerCount;
	stream.Read(reinterpret_cast<char*>(&uiPlayerCount), sizeof(unsigned int));
	serverDetails.playerNames.resize(uiPlayerCount);
	for (unsigned int uiIndex = 0; uiIndex < uiPlayerCount; uiIndex++)
	{
		unsigned int uiPlayerNameLength;
		stream.Read(reinterpret_cast<char*>(&uiPlayerNameLength), sizeof(unsigned int));
		serverDetails.playerNames[uiIndex].resize(uiPlayerNameLength);
		stream.Read(&serverDetails.playerNames[uiIndex][0], uiPlayerNameLength);
	}

	unsigned int uiHighScoreCount;
	stream.Read(reinterpret_cast<char*>(&uiHighScoreCount), sizeof(unsigned int));
	serverDetails.highScores.resize(uiHighScoreCount);
	for (unsigned int uiIndex = 0; uiIndex < uiHighScoreCount; uiIndex++)
	{
		unsigned int uiPlayerNameLength;
		stream.Read(reinterpret_cast<char*>(&uiPlayerNameLength), sizeof(unsigned int));
		serverDetails.highScores[uiIndex].playerName.resize(uiPlayerNameLength);
		stream.Read(&serverDetails.highScores[uiIndex].playerName[0], uiPlayerNameLength);
		stream.Read(reinterpret_cast<char*>(&serverDetails.highScores[uiIndex].score), sizeof(unsigned int));
	}

	return serverDetails;
}

void CSnakeClientEngine::updateFood(const TGameState& _krGameState)
{
	if (_krGameState.foodExists)
	{
		if (m_pFood == NULL)
		{
			m_pFood = new Entity(EntityCategories::CLIENT_FOOD);
			m_pFood->addComponent(new GDICircle(_krGameState.foodPosition, 5.0f));
			GazEngine::addEntity(m_pFood);
		}
		else
		{
			m_pFood->getSingleComponent<Circle>()->setPosition(_krGameState.foodPosition);
		}
	}
	else
	{
		if (m_pFood != NULL)
		{
			GazEngine::removeEntity(*m_pFood);
			m_pFood = NULL;
		}
	}
}

void CSnakeClientEngine::updateSnakes(const TGameState& _krGameState)
{
	for (unsigned int uiIndex = 0; uiIndex < _krGameState.snakeCount; uiIndex++)
	{
		Entity* pPlayer = m_players[_krGameState.snakes[uiIndex].networkId];
		CSnakeModel* pModel = pPlayer->getSingleComponent<CSnakeModel>();
		pModel->setSegmentPositions(_krGameState.snakes[uiIndex].segmentPositions,
			_krGameState.snakes[uiIndex].segmentPositionCount);

		Componentizer<unsigned int>* pScore =
			pPlayer->getSingleComponent<Componentizer<unsigned int> >();
		pScore->setValue(_krGameState.snakes[uiIndex].score);
	}
}
