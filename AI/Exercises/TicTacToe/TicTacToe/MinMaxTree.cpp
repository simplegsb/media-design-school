#include "MinMaxTree.h"

using namespace std;

namespace tictactoe
{
	MinMaxTree::MinMaxTree(const Player& player1, const Player& player2) :
			currentNode(NULL), currentPlayer(&player1), currentState(Board()), nonCurrentPlayer(&player2), optimalMoveX(0), optimalMoveY(0), root(0, 0)
	{
		currentNode = &root;
		addChildrenRecursive(*currentNode, player1, player2);
	}

	MinMaxTree::~MinMaxTree()
	{
	}

	void MinMaxTree::addChildrenRecursive(MinMaxTreeNode& node, const Player& activePlayer, const Player& otherPlayer)
	{
		for (unsigned int columnIndex = 0; columnIndex < 3; columnIndex++)
		{
			for (unsigned int rowIndex = 0; rowIndex < 3; rowIndex++)
			{
				if (currentState.get(columnIndex, rowIndex) == NULL_PLAYER_SYMBOL)
				{
					// Temporarily make a move.
					currentState.set(columnIndex, rowIndex, activePlayer.getSymbol());

					MinMaxTreeNode* child = new MinMaxTreeNode(columnIndex, rowIndex);
					node.addChild(child);

					// No need to add nodes to the tree after an end condition has been met.
					if (!currentState.lineOfThreeExists(activePlayer.getSymbol()))
					{
						addChildrenRecursive(*child, otherPlayer, activePlayer);
					}
					
					// Revert the temporary move.
					currentState.clear(columnIndex, rowIndex);
				}
			}
		}
	}

	unsigned int MinMaxTree::getOptimalMoveX() const
	{
		return optimalMoveX;
	}

	unsigned int MinMaxTree::getOptimalMoveY() const
	{
		return optimalMoveY;
	}

	void MinMaxTree::moveTo(const Board& targetState, const Player& activePlayer)
	{
		for (unsigned int index = 0; index < currentNode->getChildren().size(); index++)
		{
			MinMaxTreeNode& child = *currentNode->getChildren().at(index);

			// Make a move.
			currentState.set(child.getX(), child.getY(), activePlayer.getSymbol());

			if (currentState == targetState)
			{
				const Player* tempPlayer = currentPlayer;
				currentPlayer = nonCurrentPlayer;
				nonCurrentPlayer = tempPlayer;

				currentNode = &child;
				break;
			}
			
			// Revert the move (it was not the one requested).
			currentState.clear(child.getX(), child.getY());
		}
	}
}
