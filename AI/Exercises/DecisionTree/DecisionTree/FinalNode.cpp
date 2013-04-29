#include <iostream>

#include "FinalNode.h"

using namespace std;

namespace decisiontree
{
	FinalNode::FinalNode(const string& decision) :
			decision(decision)
	{
	}

	FinalNode::~FinalNode()
	{
	}

	DecisionTreeNode* FinalNode::visit()
	{
		cout << "Final decision: " << decision << endl;

		return NULL;
	}
}
