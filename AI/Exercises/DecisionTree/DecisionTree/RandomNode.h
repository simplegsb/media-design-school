#pragma once

#include "DecisionTreeNode.h"

namespace decisiontree
{
	class RandomNode : public DecisionTreeNode
	{
		public:
			RandomNode();

			virtual ~RandomNode();

			DecisionTreeNode* visit();

		private:
			int getConditionsTotal() const;
			
			int getRandomInt(int min, int max) const;
	};
}
