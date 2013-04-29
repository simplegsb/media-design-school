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
#include <vector>

#include <RakPeerInterface.h>

#include <gazengine/model/Circle.h>

#include "SnakeModel.h"

typedef struct TSnakeState
{
	RakNet::NetworkID networkId;

	unsigned int score;

	unsigned int segmentPositionCount;

	std::vector<Vector2> segmentPositions;
} TSnakeState;

typedef struct TGameState
{
	bool foodExists;

	Vector2 foodPosition;

	unsigned int snakeCount;

	std::vector<TSnakeState> snakes;
} TGameState;

void packAndSendGameState(RakNet::RakPeerInterface& _krPeer, std::map<RakNet::NetworkID, Entity*>& _players,
		const Circle* _kpFoodModel);

TGameState unpackGameState(const RakNet::Packet& _krPacket);
