#include "stdafx.h"

#include <cstdlib>
#include <time.h>

#include <windows.h>

#include "GarysBaseAgent.h"

using namespace std;

/**
 *  The data required by the thread created to evaluate the board.
 */
struct ThreadData
{
	GarysBaseAgent* agent;
	CBoard* board;
	CBoard::EBoardPos player;
};

// TODO: Figure out how to pass this to the thread!
ThreadData threadData;

DWORD WINAPI EvaluateThreadStart(LPVOID)
{
	threadData.agent->Evaluate(*threadData.board, threadData.player);
	return 0;
}

GarysBaseAgent::GarysBaseAgent() :
		chosenColumn(-1), maximumDepth(1)
{
	srand((unsigned) time(NULL));
}

GarysBaseAgent::~GarysBaseAgent()
{
}

int GarysBaseAgent::GetMaximumDepth()
{
	return maximumDepth;
}

CBoard::EBoardPos GarysBaseAgent::GetOtherPlayer(CBoard::EBoardPos player)
{
	if (player == CBoard::PLAYER1)
	{
		return CBoard::PLAYER2;
	}

	return CBoard::PLAYER1;
}

CBoard::EBoardPos GarysBaseAgent::GetPlayerEnum(int player)
{
	if (player == 0)
	{
		return CBoard::EMPTY;
	}

	if (player == 1)
	{
		return CBoard::PLAYER1;
	}

	if (player == 2)
	{
		return CBoard::PLAYER2;
	}
	
	return CBoard::BOARD_ERROR;
}

int GarysBaseAgent::GetRandomInt(int min, int max)
{ 
	return rand() % (max - min + 1) + min; 
}

bool GarysBaseAgent::IsComplete(const CBoard& board) const
{
	for (int column = 0; column < board.GetWidth(); column++)
	{
		if (board.GetBoardPos(column, 0) == CBoard::EMPTY)
		{
			return false;
		}
	}

	return true;
}

bool GarysBaseAgent::IsNextColumnChosen(int& chosenColumn)
{
	if (this->chosenColumn == -1)
	{
		return false;
	}

	chosenColumn = this->chosenColumn;
	return true;
}

void GarysBaseAgent::SetChosenColumn(int chosenColumn)
{
	this->chosenColumn = chosenColumn;
}

void GarysBaseAgent::SetMaximumDepth(int maximumDepth)
{
	this->maximumDepth = maximumDepth;
}

bool GarysBaseAgent::StartChooseNextColumn(CBoard* board, int playerId)
{
	chosenColumn = -1;

	// TODO: Figure out how to pass this to the thread!
	//ThreadData threadData; <-- Declared globally instead...
	threadData.agent = this;
	threadData.board = board;
	threadData.player = GetPlayerEnum(playerId);

	LPDWORD threadId = NULL;
	HANDLE evaluateThread = CreateThread(NULL, 0, &EvaluateThreadStart, &threadData, 0, threadId);
	if(evaluateThread == NULL)
    {
		// Failed to create the thread, fall back to the frame rate-killing single-threaded solution.
		EvaluateThreadStart(&threadData);
    }

	return true;
}
