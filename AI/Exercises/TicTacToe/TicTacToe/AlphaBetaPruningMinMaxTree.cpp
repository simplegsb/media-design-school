#include <iostream>

#include "AlphaBetaPruningMinMaxTree.h"

using namespace std;

namespace tictactoe
{
	AlphaBetaPruningMinMaxTree::AlphaBetaPruningMinMaxTree(const Player& player1, const Player& player2) :
			MinMaxTree(player1, player2)
	{
	}

	AlphaBetaPruningMinMaxTree::~AlphaBetaPruningMinMaxTree()
	{
	}
	
	void AlphaBetaPruningMinMaxTree::evaluate(const Player& activePlayer, const Player& otherPlayer)
	{
		nodesVisited = 1;
		int alpha = -999;
		int beta = +999;
		const MinMaxTreeNode* optimalMove = NULL;

		for (unsigned int index = 0; index < currentNode->getChildren().size(); index++)
		{
			const MinMaxTreeNode& child = *currentNode->getChildren().at(index);
			int currentResult = evaluate(child, otherPlayer, activePlayer, alpha, beta);

			if (currentResult > alpha)
			{
				alpha = currentResult;
				optimalMove = &child;
				if (alpha >= beta)
				{
					break;
				}
			}
		}

		optimalMoveX = optimalMove->getX();
		optimalMoveY = optimalMove->getY();
		
		cout << endl;
		cout << "Visited " << nodesVisited << " node(s)." << endl;
	}

	int AlphaBetaPruningMinMaxTree::evaluate(const MinMaxTreeNode& node, const Player& activePlayer, const Player& otherPlayer, int alpha, int beta)
	{
		nodesVisited++;
		int result = 0;
		currentState.set(node.getX(), node.getY(), otherPlayer.getSymbol());

		if (node.getChildren().empty())
		{
			result = heuristic.apply(currentState, *currentPlayer, *nonCurrentPlayer);
		}
		else if (activePlayer == *currentPlayer)
		{
			result = getMaximum(node.getChildren(), otherPlayer, activePlayer, alpha, beta);
		}
		else
		{
			result = getMinimum(node.getChildren(), otherPlayer, activePlayer, alpha, beta);
		}
		
		currentState.clear(node.getX(), node.getY());

		return result;
	}

	int AlphaBetaPruningMinMaxTree::getMaximum(const vector<MinMaxTreeNode*>& nodes, const Player& activePlayer, const Player& otherPlayer, int alpha, int beta)
	{
		for (unsigned int index = 0; index < nodes.size(); index++)
		{
			int currentResult = evaluate(*nodes.at(index), activePlayer, otherPlayer, alpha, beta);

			if (currentResult > alpha)
			{
				alpha = currentResult;
				if (alpha >= beta)
				{
					break;
				}
			}
		}

		return alpha;
	}

	int AlphaBetaPruningMinMaxTree::getMinimum(const vector<MinMaxTreeNode*>& nodes, const Player& activePlayer, const Player& otherPlayer, int alpha, int beta)
	{
		for (unsigned int index = 0; index < nodes.size(); index++)
		{
			int currentResult = evaluate(*nodes.at(index), activePlayer, otherPlayer, alpha, beta);

			if (currentResult < beta)
			{
				beta = currentResult;
				if (beta <= alpha)
				{
					break;
				}
			}
		}

		return beta;
	}
}
