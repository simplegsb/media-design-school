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
#include <algorithm>
#include <sstream>

#include "ScoreBoard.h"

#include <gazengine/Componentizer.h>
#include <gazengine/GazEngine.h>
#include <gazengine/model/Text.h>
#include <gazengine/odbc/ODBC.h>
#include <gazengine/odbc/ODBCExec.h>
#include <gazengine/odbc/ODBCQuery.h>

#include "SnakeConstants.h"

using namespace std;

namespace ScoreBoard
{
	Text* g_pScoreSummaryText = NULL;
	Text* g_pScoreText = NULL;

	void AddToHighScores(const TScore& _krScore)
	{
		if (!ODBC::CheckConnection())
		{
			return;
		}

		vector<TScore> highScores = GetHighScores();
		highScores.push_back(_krScore);
		sort(highScores.begin(), highScores.end());

		if (highScores.size() > SnakeConstants::MAX_HIGH_SCORES)
		{
			highScores.erase(highScores.begin());
		}

		SaveHighScores(highScores);
	}

	void ConnectToHighScores()
	{
		ODBC::Connect("snake", "snake", "snake");
	}

	void DisconnectFromHighScores()
	{
		ODBC::Disconnect();
	}

	vector<TScore> GetHighScores()
	{
		vector<TScore> highScores;
		if (!ODBC::CheckConnection())
		{
			return highScores;
		}

		ODBCQuery query(ODBC::GetConnection());
		query.Execute((SQLCHAR*) "select * from `high_score`");

		while (query.HasMoreRows())
		{
			highScores.push_back(TScore(query.GetString(2), query.GetUnsignedInt(3)));

			query.NextRow();
		}

		sort(highScores.begin(), highScores.end());

		return highScores;
	}

	void Hide()
	{
		g_pScoreText->setVisible(false);
	}

	void Init()
	{
		Entity* pScore = new Entity();
		g_pScoreText = new Text(Vector2(100, 100), "");
		g_pScoreText->setVisible(false);
		pScore->addComponent(g_pScoreText);
		g_pScoreText->setEntity(pScore);
		GazEngine::addEntity(pScore);

		Entity* pScoreSummary = new Entity();
		g_pScoreSummaryText = new Text(Vector2(10, 10), "");
		pScoreSummary->addComponent(g_pScoreSummaryText);
		g_pScoreSummaryText->setEntity(pScoreSummary);
		GazEngine::addEntity(pScoreSummary);
	}

	bool operator<(const TScore& _krLhs, const TScore& _krRhs)
	{
		return _krLhs.score < _krRhs.score;
	}

	void SaveHighScores(const vector<TScore>& _krScores)
	{
		if (!ODBC::CheckConnection())
		{
			return;
		}

		ODBCExec deleteExec(ODBC::GetConnection());
		deleteExec.Execute((SQLCHAR*) "delete from `high_score`");

		for (unsigned int uiIndex = 0; uiIndex < _krScores.size(); uiIndex++)
		{
			stringstream insertString;
			insertString << "insert into `high_score` (`player_name`, `score`) ";
			insertString << "values ('" << _krScores[uiIndex].playerName.c_str() << "', ";
			insertString << _krScores[uiIndex].score << ")";
			ODBCExec insertExec(ODBC::GetConnection());
			insertExec.Execute((SQLCHAR*) insertString.str().c_str());
		}
	}

	void Show()
	{
		g_pScoreText->setVisible(true);
	}

	void Update(unsigned int _uiSnakeCount, map<RakNet::NetworkID, CNetworkEntity*>& _rPlayers,
		RakNet::NetworkID _playerId)
	{
		string leaderName;
		string playerName;
		stringstream scoreStream;
		unsigned int uiLeaderScore = 0;
		unsigned int uiPlayerScore = 0;

		for (map<RakNet::NetworkID, CNetworkEntity*>::iterator playerIter = _rPlayers.begin();
			playerIter!= _rPlayers.end(); playerIter++)
		{
			CNetworkEntity* pPlayer = playerIter->second;
			unsigned int uiScore = pPlayer->getSingleComponent<Componentizer<unsigned int> >()->getValue();

			scoreStream << pPlayer->getName().c_str() << ": " << uiScore << endl;

			if (uiScore >= uiLeaderScore)
			{
				leaderName = pPlayer->getName();
				uiLeaderScore = uiScore;
			}

			if (pPlayer->GetNetworkID() == _playerId)
			{
				playerName = pPlayer->getName();
				uiPlayerScore = uiScore;
			}
		}

		g_pScoreText->setText(scoreStream.str());

		stringstream scoreSummaryStream;
		scoreSummaryStream << "Leader: " << uiLeaderScore << endl << playerName.c_str() << ": " << uiPlayerScore;
		g_pScoreSummaryText->setText(scoreSummaryStream.str());

		if (_uiSnakeCount == 0)
		{
			Entity* pWinner = new Entity();
			stringstream winnerString;
			winnerString << leaderName.c_str();
			winnerString << " is the winner!";
			pWinner->addComponent(new Text(Vector2(400.0f, 300.0f), winnerString.str()));
			GazEngine::addEntity(pWinner);
		}
	}

	TScore::TScore() :
		playerName(),
		score(0)
	{
	}

	TScore::TScore(const std::string& _krPlayerName, unsigned int _uiScore) :
		playerName(_krPlayerName),
		score(_uiScore)
	{
	}
}
