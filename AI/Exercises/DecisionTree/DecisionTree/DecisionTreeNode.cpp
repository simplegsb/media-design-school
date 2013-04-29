#include "DecisionTreeNode.h"

using namespace std;

namespace decisiontree
{
	DecisionTreeNode::DecisionTreeNode() :
			children()
	{
	}

	DecisionTreeNode::~DecisionTreeNode()
	{
		for (unsigned int index = 0; index < children.size(); index++)
		{
			delete children.at(index);
		}
	}

	void DecisionTreeNode::addChild(DecisionTreeNode* child, int condition)
	{
		children.insert(pair<int, DecisionTreeNode*>(condition, child));
	}

	DecisionTreeNode* DecisionTreeNode::getChild(int condition)
	{
		map<int, DecisionTreeNode*>::iterator child = children.find(condition);
		if (child == children.end())
		{
			return NULL;
		}

		return child->second;
	}

	const map<int, DecisionTreeNode*>& DecisionTreeNode::getChildren() const
	{
		return children;
	}
}
