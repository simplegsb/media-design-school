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

#include <RakPeerInterface.h>

#include "ScoreBoard.h"

typedef struct TServerDetails
{
	RakNet::SystemAddress address;

	std::vector<ScoreBoard::TScore> highScores;

	std::string name;

	std::vector<std::string> playerNames;
} TServerDetails;
