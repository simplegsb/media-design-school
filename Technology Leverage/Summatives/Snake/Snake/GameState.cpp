#include <BitStream.h>
#include <RakPeerInterface.h>

#include <gazengine/Componentizer.h>

#include "GameState.h"
#include "MessageIdentifiers.h"

using namespace std;

void packAndSendGameState(RakNet::RakPeerInterface& _krPeer, map<RakNet::NetworkID, Entity*>& _players,
		const Circle* _kpFoodModel)
{
	RakNet::BitStream stream;
	stream.Write((RakNet::MessageID) MessageIdentifiers::ID_SNAKE_STATES);
	stream.Write(_players.size());

	for (map<RakNet::NetworkID, Entity*>::iterator playerIter = _players.begin();
		playerIter != _players.end(); playerIter++)
	{
		Entity* pSnake = playerIter->second;
		stream.Write(playerIter->first);
		stream.Write(pSnake->getSingleComponent<Componentizer<unsigned int> >()->getValue());

		vector<Vector2> segmentPositions = pSnake->getSingleComponent<CSnakeModel>()->getSegmentPositions();
		stream.Write(segmentPositions.size());

		for (unsigned int uiSegmentIndex = 0; uiSegmentIndex < segmentPositions.size(); uiSegmentIndex++)
		{
			stream.Write(segmentPositions[uiSegmentIndex]);
		}
	}

	if (_kpFoodModel == NULL)
	{
		stream.Write(false);
	}
	else
	{
		stream.Write(true);
		stream.Write(_kpFoodModel->getPosition());
	}

	_krPeer.Send(&stream, IMMEDIATE_PRIORITY, UNRELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

TGameState unpackGameState(const RakNet::Packet& _krPacket)
{
	RakNet::BitStream stream(_krPacket.data, _krPacket.length, false);
	stream.IgnoreBytes(1);

	TGameState gameState;
	stream.Read(gameState.snakeCount);
	gameState.snakes.resize(gameState.snakeCount);

	for (unsigned int uiSnakeIndex = 0; uiSnakeIndex < gameState.snakeCount; uiSnakeIndex++)
	{
		stream.Read(gameState.snakes[uiSnakeIndex].networkId);
		stream.Read(gameState.snakes[uiSnakeIndex].score);

		stream.Read(gameState.snakes[uiSnakeIndex].segmentPositionCount);
		gameState.snakes[uiSnakeIndex].segmentPositions.resize(gameState.snakes[uiSnakeIndex].segmentPositionCount);

		for (unsigned int uiSegmentIndex = 0;
			uiSegmentIndex < gameState.snakes[uiSnakeIndex].segmentPositionCount; uiSegmentIndex++)
		{
			stream.Read(gameState.snakes[uiSnakeIndex].segmentPositions[uiSegmentIndex]);
		}
	}

	stream.Read(gameState.foodExists);
	stream.Read(gameState.foodPosition);

	return gameState;
}