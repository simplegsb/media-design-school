#include <exception>

#include "RandomNode.h"
#include "time.h"

using namespace std;

namespace decisiontree
{
	RandomNode::RandomNode()
	{
		srand(time(NULL));
	}

	RandomNode::~RandomNode()
	{
	}

	DecisionTreeNode* RandomNode::visit()
	{
		int conditionRandom = getRandomInt(1, getConditionsTotal());
		int conditionsCumulative = 0;
		
		for (map<int, DecisionTreeNode*>::const_iterator child = getChildren().begin(); child != getChildren().end(); child++)
		{
			conditionsCumulative += child->first;
			if (conditionRandom <= conditionsCumulative)
			{
				return child->second;
			}
		}

		throw exception();
	}

	int RandomNode::getConditionsTotal() const
	{
		int conditionsTotal = 0;

		for (map<int, DecisionTreeNode*>::const_iterator child = getChildren().begin(); child != getChildren().end(); child++)
		{
			conditionsTotal += child->first;
		}

		return conditionsTotal;
	}

	int RandomNode::getRandomInt(int min, int max) const
	{ 
		return rand() % (max - min + 1) + min; 
	}
}
