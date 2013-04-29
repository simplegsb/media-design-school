#ifndef MINMAXTREE_H_
#define MINMAXTREE_H_

#include "Board.h"
#include "Heuristic.h"
#include "MinMaxTreeNode.h"
#include "Player.h"

namespace tictactoe
{
	class MinMaxTree
	{
		public:
			MinMaxTree(const Player& player1, const Player& player2);

			virtual ~MinMaxTree();

			virtual void evaluate(const Player& activePlayer, const Player& otherPlayer) = 0;

			unsigned int getOptimalMoveX() const;

			unsigned int getOptimalMoveY() const;

			void moveTo(const Board& targetState, const Player& activePlayer);

		protected:
			MinMaxTreeNode* currentNode;

			const Player* currentPlayer;

			Board currentState;

			Heuristic heuristic;
			
			const Player* nonCurrentPlayer;

			unsigned int optimalMoveX;

			unsigned int optimalMoveY;

			MinMaxTreeNode root;
			
		private:
			void addChildrenRecursive(MinMaxTreeNode& node, const Player& activePlayer, const Player& otherPlayer);
	};
}

#endif /* MINMAXTREE_H_ */
