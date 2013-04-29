//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// Copyright (c) 2012 Media Design School
//
// Author: Gary Buyn
//
// SVN Info
// $Author: MEDIADESIGN\gary.buyn $
// $Date: 2013-01-23 09:45:25 +1300 (Wed, 23 Jan 2013) $
// $Rev: 1110 $
//
#pragma once

#include "GameState.h"
#include "NetworkEntity.h"

namespace ScoreBoard
{
	typedef struct TScore
	{
		TScore();

		TScore(const std::string& _krPlayerName, unsigned int _uiScore);

		std::string playerName;

		unsigned int score;
	} TScore;

	void AddToHighScores(const TScore& _krScore);

	void ConnectToHighScores();

	void DisconnectFromHighScores();

	std::vector<TScore> GetHighScores();

	void Hide();

	void Init();

	bool operator<(const TScore& _krLhs, const TScore& _krRhs);

	void SaveHighScores(const std::vector<TScore>& _krScores);

	void Show();

	void Update(unsigned int _uiSnakeCount, std::map<RakNet::NetworkID, CNetworkEntity*>& _rPlayers,
		RakNet::NetworkID _playerId);
}
