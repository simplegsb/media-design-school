#include <iostream>

#include "SimpleMinMaxTree.h"

using namespace std;

namespace tictactoe
{
	SimpleMinMaxTree::SimpleMinMaxTree(const Player& player1, const Player& player2) :
			MinMaxTree(player1, player2), nodesVisited(0)
	{
	}

	SimpleMinMaxTree::~SimpleMinMaxTree()
	{
	}
	
	void SimpleMinMaxTree::evaluate(const Player& activePlayer, const Player& otherPlayer)
	{
		nodesVisited = 1;
		int maximumResult = -999;
		const MinMaxTreeNode* optimalMove = NULL;

		for (unsigned int index = 0; index < currentNode->getChildren().size(); index++)
		{
			const MinMaxTreeNode& child = *currentNode->getChildren().at(index);
			int currentResult = evaluate(child, otherPlayer, activePlayer);

			if (optimalMove == NULL || maximumResult < currentResult)
			{
				maximumResult = currentResult;
				optimalMove = &child;
			}
		}

		optimalMoveX = optimalMove->getX();
		optimalMoveY = optimalMove->getY();

		cout << endl;
		cout << "Visited " << nodesVisited << " node(s)." << endl;
	}

	int SimpleMinMaxTree::evaluate(const MinMaxTreeNode& node, const Player& activePlayer, const Player& otherPlayer)
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
			result = getMaximum(node.getChildren(), otherPlayer, activePlayer);
		}
		else
		{
			result = getMinimum(node.getChildren(), otherPlayer, activePlayer);
		}
		
		currentState.clear(node.getX(), node.getY());

		return result;
	}

	int SimpleMinMaxTree::getMaximum(const vector<MinMaxTreeNode*>& nodes, const Player& activePlayer, const Player& otherPlayer)
	{
		int maximumResult = 0;

		for (unsigned int index = 0; index < nodes.size(); index++)
		{
			int currentResult = evaluate(*nodes.at(index), activePlayer, otherPlayer);

			if (index == 0 || maximumResult < currentResult)
			{
				maximumResult = currentResult;
			}
		}

		return maximumResult;
	}

	int SimpleMinMaxTree::getMinimum(const vector<MinMaxTreeNode*>& nodes, const Player& activePlayer, const Player& otherPlayer)
	{
		int minimumResult = 0;

		for (unsigned int index = 0; index < nodes.size(); index++)
		{
			int currentResult = evaluate(*nodes.at(index), activePlayer, otherPlayer);

			if (index == 0 || minimumResult > currentResult)
			{
				minimumResult = currentResult;
			}
		}

		return minimumResult;
	}
}
