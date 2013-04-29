#pragma once

#include "DecisionTreeNode.h"

namespace decisiontree
{
	class DecisionTree
	{
		public:
			DecisionTree();

			~DecisionTree();

			DecisionTreeNode* getRoot();

			void search();

			void setRoot(DecisionTreeNode* root);

		private:
			DecisionTreeNode* root;
	};
}
