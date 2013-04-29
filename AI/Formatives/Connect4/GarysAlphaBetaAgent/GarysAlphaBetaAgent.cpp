#include "Common.h"
#include "GameBoard.h"

#include "GarysAlphaBetaAgent.h"

const int NEGATIVE_INFINITY = -999;
const int POSITIVE_INFINITY = 999;

GarysAlphaBetaAgent::~GarysAlphaBetaAgent()
{
}

void GarysAlphaBetaAgent::Evaluate(CBoard& board, CBoard::EBoardPos player)
{
	int alpha = NEGATIVE_INFINITY;
	int optimalMove = -1;

	for (int column = 0; column < board.GetWidth(); column++)
	{
		// Pretty highlights to show progress.
		board.HighlightPossibleMove(column, player);

		CBoard* boardClone = board.CreateClone();
		
		if (boardClone->MakeMove(column, player))
		{
			int currentResult = Evaluate(*boardClone, player, player, 1, alpha, POSITIVE_INFINITY);
			if (currentResult > alpha)
			{
				alpha = currentResult;
				optimalMove = column;
			}
		}

		boardClone->ReleaseClone();
	}

	SetChosenColumn(optimalMove);
}

int GarysAlphaBetaAgent::Evaluate(const CBoard& board, CBoard::EBoardPos activePlayer, CBoard::EBoardPos currentPlayer, int depth, int alpha, int beta)
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
		return GetMaximum(board, activePlayer, nextPlayer, depth + 1, alpha, beta);
	}
	
	return GetMinimum(board, activePlayer, nextPlayer, depth + 1, alpha, beta);
}

int GarysAlphaBetaAgent::GetMaximum(const CBoard& board, CBoard::EBoardPos activePlayer, CBoard::EBoardPos currentPlayer, int depth, int alpha, int beta)
{
	for (int column = 0; column < board.GetWidth(); column++)
	{
		CBoard* boardClone = board.CreateClone();
		
		if (boardClone->MakeMove(column, currentPlayer))
		{
			int currentResult = Evaluate(*boardClone, activePlayer, currentPlayer, depth, alpha, beta);
			if (currentResult > alpha)
			{
				alpha = currentResult;
				if (alpha >= beta)
				{
					boardClone->ReleaseClone();
					break;
				}
			}
		}

		boardClone->ReleaseClone();
	}

	return alpha;
}

int GarysAlphaBetaAgent::GetMinimum(const CBoard& board, CBoard::EBoardPos activePlayer, CBoard::EBoardPos currentPlayer, int depth, int alpha, int beta)
{
	for (int column = 0; column < board.GetWidth(); column++)
	{
		CBoard* boardClone = board.CreateClone();

		if (boardClone->MakeMove(column, currentPlayer))
		{
			int currentResult = Evaluate(*boardClone, activePlayer, currentPlayer, depth, alpha, beta);
			if (currentResult < beta)
			{
				beta = currentResult;
				if (beta <= alpha)
				{
					boardClone->ReleaseClone();
					break;
				}
			}
		}

		boardClone->ReleaseClone();
	}

	return beta;
}

void GarysAlphaBetaAgent::operator=(GarysAlphaBetaAgent&)
{
}
