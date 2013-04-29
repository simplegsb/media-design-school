#pragma once

#include <string>

#include "DecisionTreeNode.h"

namespace decisiontree
{
	class InputNode : public DecisionTreeNode
	{
		public:
			InputNode(const std::string& question);

			virtual ~InputNode();

			DecisionTreeNode* visit();

		private:
			const std::string question;

			std::string getAnswer();

			int getResponseCondition();
	};
}
