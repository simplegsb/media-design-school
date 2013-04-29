#include "Common.h"
#include "GameBoard.h"

#include "GarysSimpleAgent.h"

using namespace std;

const int NEGATIVE_INFINITY = -999;
const int POSITIVE_INFINITY = 999;

GarysSimpleAgent::~GarysSimpleAgent()
{
}

void GarysSimpleAgent::Evaluate(CBoard& board, CBoard::EBoardPos player)
{
	int maximumResult = NEGATIVE_INFINITY;
	int optimalMove = -1;

	for (int column = 0; column < board.GetWidth(); column++)
	{
		// Pretty highlights to show progress.
		board.HighlightPossibleMove(column, player);

		CBoard* boardClone = board.CreateClone();
		
		if (boardClone->MakeMove(column, player))
		{
			int currentResult = Evaluate(*boardClone, player, player, 1);
			if (currentResult > maximumResult)
			{
				maximumResult = currentResult;
				optimalMove = column;
			}
		}

		boardClone->ReleaseClone();
	}

	SetChosenColumn(optimalMove);
}

int GarysSimpleAgent::Evaluate(const CBoard& board, CBoard::EBoardPos activePlayer, CBoard::EBoardPos currentPlayer, int depth)
{
	if (IsComplete(board) || depth == GetMaximumDepth())
	{
		// CBoard::EvaluateScore() is in favour of PLAYER1.
		if (activePlayer == CBoard::PLAYER1)
		{
			return board.EvaluateScore();
		}
		
		return board.EvaluateScore() * -1;
	}

	CBoard::EBoardPos nextPlayer = GetOtherPlayer(currentPlayer);
	if (nextPlayer == activePlayer)
	{
		return GetMaximum(board, activePlayer, nextPlayer, depth + 1);
	}
	
	return GetMinimum(board, activePlayer, nextPlayer, depth + 1);
}

int GarysSimpleAgent::GetMaximum(const CBoard& board, CBoard::EBoardPos activePlayer, CBoard::EBoardPos currentPlayer, int depth)
{
	int maximumResult = NEGATIVE_INFINITY;

	for (int column = 0; column < board.GetWidth(); column++)
	{
		CBoard* boardClone = board.CreateClone();
		
		if (boardClone->MakeMove(column, currentPlayer))
		{
			int currentResult = Evaluate(*boardClone, activePlayer, currentPlayer, depth);
			if (currentResult > maximumResult)
			{
				maximumResult = currentResult;
			}
		}

		boardClone->ReleaseClone();
	}

	return maximumResult;
}

int GarysSimpleAgent::GetMinimum(const CBoard& board, CBoard::EBoardPos activePlayer, CBoard::EBoardPos currentPlayer, int depth)
{
	int minimumResult = POSITIVE_INFINITY;

	for (int column = 0; column < board.GetWidth(); column++)
	{
		CBoard* boardClone = board.CreateClone();

		if (boardClone->MakeMove(column, currentPlayer))
		{
			int currentResult = Evaluate(*boardClone, activePlayer, currentPlayer, depth);
			if (currentResult < minimumResult)
			{
				minimumResult = currentResult;
			}
		}

		boardClone->ReleaseClone();
	}

	return minimumResult;
}

void GarysSimpleAgent::operator=(GarysSimpleAgent&)
{
}
