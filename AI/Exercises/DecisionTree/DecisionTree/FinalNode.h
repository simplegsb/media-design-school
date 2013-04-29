#pragma once

#include <string>

#include "DecisionTreeNode.h"

namespace decisiontree
{
	class FinalNode : public DecisionTreeNode
	{
		public:
			FinalNode(const std::string& decision);

			virtual ~FinalNode();

			DecisionTreeNode* visit();

		private:
			const std::string decision;
	};
}
