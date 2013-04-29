#pragma once

#include <map>

namespace decisiontree
{
	class DecisionTreeNode
	{
		public:
			DecisionTreeNode();
			
			virtual ~DecisionTreeNode();

			void addChild(DecisionTreeNode* child, int condition);

			DecisionTreeNode* getChild(int condition);

			const std::map<int, DecisionTreeNode*>& getChildren() const;

			virtual DecisionTreeNode* visit() = 0;

		private:
			std::map<int, DecisionTreeNode*> children;
	};
}
