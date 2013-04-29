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
#include <ctime>
#include <sstream>

#include <gazengine/GazEngine.h>
#include <gazengine/Messages.h>

#include "resource.h"
#include "ServerDetails.h"
#include "SnakeCharmingEngine.h"
#include "SnakeConstants.h"
#include "SnakeEvents.h"
#include "SnakeFeederEngine.h"
#include "SnakeServerEngine.h"
#include "SnakeWranglerEngine.h"

// Must be after SnakeServerEngine.h because it includes winsock.h and this includes windows.h.
#include "ServerBrowser.h"

using namespace std;

namespace ServerBrowser
{
	BOOL CALLBACK HandleEvent(HWND _window, UINT _uiMessage, WPARAM _wParam, LPARAM _lParam);
	string GetPlayerName();
	void Host();
	void Join();
	void Leave();
	void OnConnected(const void* _kpMessage);
	void OnDisconnected(const void* _kpMessage);
	void OnNotConnected(const void* _kpMessage);
	void OnServerUpdate(const void* _kpMessage);
	void OnStartGame(const void* _kpMessage);
	void RemoveDeadServers();
	void SelectServer();
	void Start();
	void UpdateHighScoreList();
	void UpdatePlayerList();

	map<string, time_t> g_detectedServerLastPongTimes;
	map<string, TServerDetails> g_detectedServers;
	bool g_bHosting = false;
	string g_localServerName;
	TServerDetails* g_pSelectedServer = NULL;
	HWND g_serverBrowser;

	HWND Create(HINSTANCE _instance)
	{
		g_serverBrowser = CreateDialog(_instance, MAKEINTRESOURCE(IDD_SERVER_BROWSER), NULL, HandleEvent);

		Messages::registerRecipient(SnakeClientEvents::GAME_STARTED, OnStartGame);
		Messages::registerRecipient(SnakeClientEvents::CONNECTED, OnConnected);
		Messages::registerRecipient(SnakeClientEvents::DISCONNECTED, OnDisconnected);
		Messages::registerRecipient(SnakeClientEvents::NOT_CONNECTED, OnNotConnected);
		Messages::registerRecipient(SnakeClientEvents::SERVER_DETAILS, OnServerUpdate);

		return g_serverBrowser;
	}

	void Dispose()
	{
		Messages::deregisterRecipient(SnakeClientEvents::GAME_STARTED, OnStartGame);
		Messages::deregisterRecipient(SnakeClientEvents::CONNECTED, OnConnected);
		Messages::deregisterRecipient(SnakeClientEvents::DISCONNECTED, OnDisconnected);
		Messages::registerRecipient(SnakeClientEvents::NOT_CONNECTED, OnNotConnected);
		Messages::deregisterRecipient(SnakeClientEvents::SERVER_DETAILS, OnServerUpdate);
	}

	string GetPlayerName()
	{
		wstring wPlayerName;
		wPlayerName.resize(128);
		GetDlgItemText(g_serverBrowser, IDPLAYER_NAME, &wPlayerName[0], 128);
		if (wPlayerName[0] == L'\0')
		{
			return string();
		}

		return string(wPlayerName.begin(), wPlayerName.end());
	}

	BOOL CALLBACK HandleEvent(HWND, UINT _uiMessage, WPARAM _wParam, LPARAM)
	{
		if (_uiMessage == WM_CLOSE)
		{
			GazEngine::stop();
			return true;
		}
		else if (_uiMessage == WM_COMMAND)
		{
			if (LOWORD(_wParam) == IDCANCEL)
			{
				GazEngine::stop();
				return true;
			}
			else if (LOWORD(_wParam) == IDHOST)
			{
				Host();
				return true;
			}
			else if (LOWORD(_wParam) == IDJOIN)
			{
				Join();
				return true;
			}
			else if (LOWORD(_wParam) == IDLEAVE)
			{
				Leave();
				return true;
			}
			else if (LOWORD(_wParam) == IDSERVER_LIST)
			{
				if (HIWORD(_wParam) == LBN_SELCHANGE)
				{
					SelectServer();
					return true;
				}
			}
			else if (LOWORD(_wParam) == IDSTART)
			{
				Start();
				return true;
			}
		}

		return false;
	}

	void Host()
	{
		string playerName = GetPlayerName();
		if (playerName.empty())
		{
			SetDlgItemText(g_serverBrowser, IDSTATUS,
				L"I'm going to need you to go ahead and name your player first.");
			return;
		}

		Messages::send(SnakeClientEvents::PLAYER_NAME, &playerName);

		wstring wServerName;
		wServerName.resize(128);
		GetDlgItemText(g_serverBrowser, IDGAME_NAME, &wServerName[0], 128);
		wServerName.resize(wcslen(&wServerName[0]) + 1);

		if (wServerName[0] == L'\0')
		{
			SetDlgItemText(g_serverBrowser, IDSTATUS,
				L"I'm going to need you to go ahead and name your game first.");
			return;
		}

		g_localServerName = string(wServerName.begin(), wServerName.end());
		GazEngine::addEngine(new CSnakeCharmingEngine);
		GazEngine::addEngine(new CSnakeWranglerEngine);
		GazEngine::addEngine(new CSnakeFeederEngine);
		GazEngine::addEngine(new CSnakeServerEngine(g_localServerName));
		SetDlgItemText(g_serverBrowser, IDSTATUS,
			L"Congrats! You're hosting a game!");

		EnableWindow(GetDlgItem(g_serverBrowser, IDHOST), FALSE);
	}

	void Join()
	{
		string playerName = GetPlayerName();
		if (playerName.empty())
		{
			SetDlgItemText(g_serverBrowser, IDSTATUS,
				L"I'm going to need you to go ahead and name your player first.");
			return;
		}

		Messages::send(SnakeClientEvents::PLAYER_NAME, &playerName);

		int iSelectionIndex = (int) SendDlgItemMessage(g_serverBrowser, IDSERVER_LIST, LB_GETCURSEL, 0, 0);
		if (iSelectionIndex == LB_ERR)
		{
			SetDlgItemText(g_serverBrowser, IDSTATUS,
				L"I'm going to need you to go ahead and select a server first.");
			return;
		}

		int iLength = (int) SendDlgItemMessage(g_serverBrowser, IDSERVER_LIST, LB_GETTEXTLEN, iSelectionIndex, 0);

		wstring wServerName;
		wServerName.resize(iLength + 1);
		SendDlgItemMessage(g_serverBrowser, IDSERVER_LIST, LB_GETTEXT, iSelectionIndex, (LPARAM) &wServerName[0]);

		string serverName(wServerName.begin(), wServerName.end());
		SetDlgItemText(g_serverBrowser, IDSTATUS,
			L"Joining...");
		Messages::send(SnakeClientEvents::JOIN, &serverName);

		EnableWindow(GetDlgItem(g_serverBrowser, IDJOIN), FALSE);
		EnableWindow(GetDlgItem(g_serverBrowser, IDSERVER_LIST), FALSE);

		if (serverName == g_localServerName)
		{
			g_bHosting = true;
		}
	}

	void Leave()
	{
		Messages::send(SnakeClientEvents::LEAVE, NULL);

		g_bHosting = false;
		EnableWindow(GetDlgItem(g_serverBrowser, IDJOIN), TRUE);
		EnableWindow(GetDlgItem(g_serverBrowser, IDLEAVE), FALSE);
		EnableWindow(GetDlgItem(g_serverBrowser, IDSTART), FALSE);
	}

	void OnConnected(const void*)
	{
		SetDlgItemText(g_serverBrowser, IDSTATUS,
			L"You're in! Waiting for the host to start...");

		EnableWindow(GetDlgItem(g_serverBrowser, IDLEAVE), TRUE);

		if (g_bHosting)
		{
			EnableWindow(GetDlgItem(g_serverBrowser, IDSTART), TRUE);
		}
	}

	void OnDisconnected(const void*)
	{
		SetDlgItemText(g_serverBrowser, IDSTATUS,
			L"You have left the server.");

		EnableWindow(GetDlgItem(g_serverBrowser, IDJOIN), TRUE);
		EnableWindow(GetDlgItem(g_serverBrowser, IDSERVER_LIST), TRUE);
	}

	void OnNotConnected(const void*)
	{
		SetDlgItemText(g_serverBrowser, IDSTATUS,
			L"Sorry, the server is full or the game has already started.");

		EnableWindow(GetDlgItem(g_serverBrowser, IDJOIN), TRUE);
		EnableWindow(GetDlgItem(g_serverBrowser, IDSERVER_LIST), TRUE);
	}

	void OnServerUpdate(const void* _kpMessage)
	{
		const TServerDetails* kpServerDetails = static_cast<const TServerDetails*>(_kpMessage);

		if (g_detectedServers.find(kpServerDetails->name) == g_detectedServers.end())
		{
			wstring wServerName(kpServerDetails->name.begin(), kpServerDetails->name.end());
			SendDlgItemMessage(g_serverBrowser, IDSERVER_LIST, LB_ADDSTRING, 0, (LPARAM) wServerName.c_str());
		}

		g_detectedServerLastPongTimes[kpServerDetails->name] = time(NULL);
		g_detectedServers[kpServerDetails->name] = *kpServerDetails;

		if (g_pSelectedServer == &g_detectedServers[kpServerDetails->name])
		{
			UpdatePlayerList();
			UpdateHighScoreList();
		}
	}

	void OnStartGame(const void*)
	{
		CloseWindow(g_serverBrowser);
	}

	void RemoveDeadServers()
	{
		map<string, TServerDetails>::iterator serverIter = g_detectedServers.begin();
		while (serverIter != g_detectedServers.end())
		{
			if (time(NULL) - g_detectedServerLastPongTimes[serverIter->first] >= 3)
			{
				wstring wServerName(serverIter->first.begin(), serverIter->first.end());
				unsigned int uiIndex = SendDlgItemMessage(g_serverBrowser, IDSERVER_LIST, LB_FINDSTRINGEXACT, (WPARAM) -1,
					(LPARAM) wServerName.c_str());
				SendDlgItemMessage(g_serverBrowser, IDSERVER_LIST, LB_DELETESTRING, uiIndex, 0);

				g_detectedServerLastPongTimes.erase(serverIter->first);
				serverIter = g_detectedServers.erase(serverIter);
			}
			else
			{
				serverIter++;
			}
		}
	}

	void SelectServer()
	{
		int iSelectionIndex = (int) SendDlgItemMessage(g_serverBrowser, IDSERVER_LIST, LB_GETCURSEL, 0, 0);
		if (iSelectionIndex == LB_ERR)
		{
			SendDlgItemMessage(g_serverBrowser, IDSERVER_LIST, LB_RESETCONTENT, 0, 0);
			return;
		}

		int iLength = (int) SendDlgItemMessage(g_serverBrowser, IDSERVER_LIST, LB_GETTEXTLEN, iSelectionIndex, 0);

		wstring wServerName;
		wServerName.resize(iLength + 1);
		SendDlgItemMessage(g_serverBrowser, IDSERVER_LIST, LB_GETTEXT, iSelectionIndex, (LPARAM) &wServerName[0]);

		string serverName(wServerName.begin(), wServerName.end());
		g_pSelectedServer = &g_detectedServers[serverName];

		UpdatePlayerList();
		UpdateHighScoreList();
	}

	void Start()
	{
		int iPlayerCount = (int) SendDlgItemMessage(g_serverBrowser, IDPLAYER_LIST, LB_GETCOUNT, 0, 0);

		if (iPlayerCount >= SnakeConstants::MIN_PLAYERS)
		{
			Messages::send(SnakeServerEvents::GAME_START, NULL);	
		}
		else
		{
			SetDlgItemText(g_serverBrowser, IDSTATUS,
				L"You can't start the game until there are at least 2 players.");
		}
	}

	void UpdateHighScoreList()
	{
		SendDlgItemMessage(g_serverBrowser, IDHIGH_SCORE_LIST, LB_RESETCONTENT, 0, 0);
		for (unsigned int uiIndex = g_pSelectedServer->highScores.size() - 1;
			uiIndex < g_pSelectedServer->highScores.size(); uiIndex--)
		{
			wstring wPlayerName(g_pSelectedServer->highScores[uiIndex].playerName.begin(),
				g_pSelectedServer->highScores[uiIndex].playerName.end());
			wstringstream wHighScore;
			wHighScore << wPlayerName.c_str() << ": " << g_pSelectedServer->highScores[uiIndex].score;
			SendDlgItemMessage(g_serverBrowser, IDHIGH_SCORE_LIST, LB_ADDSTRING, 0, (LPARAM) wHighScore.str().c_str());
		}
	}

	void UpdatePlayerList()
	{
		SendDlgItemMessage(g_serverBrowser, IDPLAYER_LIST, LB_RESETCONTENT, 0, 0);
		for (unsigned int uiIndex = 0; uiIndex < g_pSelectedServer->playerNames.size(); uiIndex++)
		{
			wstring wPlayerName(g_pSelectedServer->playerNames[uiIndex].begin(),
				g_pSelectedServer->playerNames[uiIndex].end());
			SendDlgItemMessage(g_serverBrowser, IDPLAYER_LIST, LB_ADDSTRING, 0, (LPARAM) wPlayerName.c_str());
		}
	}
}
