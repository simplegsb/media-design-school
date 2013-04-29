#ifndef ALPHABETAPRUNINGMINMAXTREE_H_
#define ALPHABETAPRUNINGMINMAXTREE_H_

#include "MinMaxTree.h"

namespace tictactoe
{
	class AlphaBetaPruningMinMaxTree : public MinMaxTree
	{
		public:
			AlphaBetaPruningMinMaxTree(const Player& player1, const Player& player2);

			virtual ~AlphaBetaPruningMinMaxTree();

			void evaluate(const Player& activePlayer, const Player& otherPlayer);

		private:
			int nodesVisited;

			int evaluate(const MinMaxTreeNode&, const Player& activePlayer, const Player& otherPlayer, int alpha, int beta);

			int getMaximum(const std::vector<MinMaxTreeNode*>& nodes, const Player& activePlayer, const Player& otherPlayer, int alpha, int beta);

			int getMinimum(const std::vector<MinMaxTreeNode*>& nodes, const Player& activePlayer, const Player& otherPlayer, int alpha, int beta);
	};
}

#endif /* ALPHABETAPRUNINGMINMAXTREE_H_ */
