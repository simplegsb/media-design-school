#include "DecisionTree.h"

namespace decisiontree
{
	DecisionTree::DecisionTree() :
			root(NULL)
	{
	}

	DecisionTree::~DecisionTree()
	{
		if (root != NULL)
		{
			delete root;
		}
	}

	DecisionTreeNode* DecisionTree::getRoot()
	{
		return root;
	}

	void DecisionTree::search()
	{
		DecisionTreeNode* currentNode = root;

		while(currentNode != NULL)
		{
			currentNode = currentNode->visit();
		}
	}

	void DecisionTree::setRoot(DecisionTreeNode* root)
	{
		this->root = root;
	}
}
