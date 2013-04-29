#ifndef SIMPLEMINMAXTREE_H_
#define SIMPLEMINMAXTREE_H_

#include "MinMaxTree.h"

namespace tictactoe
{
	class SimpleMinMaxTree : public MinMaxTree
	{
		public:
			SimpleMinMaxTree(const Player& player1, const Player& player2);

			virtual ~SimpleMinMaxTree();

			void evaluate(const Player& activePlayer, const Player& otherPlayer);

		private:
			int nodesVisited;

			int evaluate(const MinMaxTreeNode&, const Player& activePlayer, const Player& otherPlayer);

			int getMaximum(const std::vector<MinMaxTreeNode*>& nodes, const Player& activePlayer, const Player& otherPlayer);

			int getMinimum(const std::vector<MinMaxTreeNode*>& nodes, const Player& activePlayer, const Player& otherPlayer);
	};
}

#endif /* SIMPLEMINMAXTREE_H_ */
