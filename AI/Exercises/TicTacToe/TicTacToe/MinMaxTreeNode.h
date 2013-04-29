#ifndef MINMAXTREENODE_H_
#define MINMAXTREENODE_H_

#include "Board.h"
#include "Player.h"

namespace tictactoe
{
	class MinMaxTreeNode
	{
		public:
			MinMaxTreeNode(unsigned int x, unsigned int y);

			~MinMaxTreeNode();

			void addChild(MinMaxTreeNode* child);

			const std::vector<MinMaxTreeNode*>& getChildren() const;

			const unsigned int getX() const;

			const unsigned int getY() const;

		private:
			std::vector<MinMaxTreeNode*> children;

			unsigned int x;

			unsigned int y;
	};
}

#endif /* MINMAXTREENODE_H_ */
