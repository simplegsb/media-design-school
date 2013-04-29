#include "MinMaxTreeNode.h"

using namespace std;

namespace tictactoe
{
	MinMaxTreeNode::MinMaxTreeNode(unsigned int x, unsigned int y) :
			children(), x(x), y(y)
	{
	}

	MinMaxTreeNode::~MinMaxTreeNode()
	{
		for (unsigned int index = 0; index < children.size(); index++)
		{
			delete children.at(index);
		}
	}

	void MinMaxTreeNode::addChild(MinMaxTreeNode* child)
	{
		children.push_back(child);
	}

	const vector<MinMaxTreeNode*>& MinMaxTreeNode::getChildren() const
	{
		return children;
	}

	const unsigned int MinMaxTreeNode::getX() const
	{
		return x;
	}

	const unsigned int MinMaxTreeNode::getY() const
	{
		return y;
	}
}
