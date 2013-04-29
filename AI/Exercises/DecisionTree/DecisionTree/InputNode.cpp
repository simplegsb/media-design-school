#include <iostream>

#include "Conditions.h"
#include "InputNode.h"

using namespace std;

namespace decisiontree
{
	InputNode::InputNode(const std::string& question) :
			question(question)
	{
	}

	InputNode::~InputNode()
	{
	}

	string InputNode::getAnswer()
	{
		cout << question << " (yes/no) > ";

		string answer;
		cin >> answer;

		return answer;
	}

	int InputNode::getResponseCondition()
	{
		string answer = getAnswer();

		while (answer != "no" && answer != "yes")
		{
			cout << "I'm going to need you to go ahead and type either 'yes' or 'no' (in lower case)." << endl;
			answer = getAnswer();
		}

		if (answer == "no")
		{
			return Conditions::NO;
		}

		return Conditions::YES;
	}

	DecisionTreeNode* InputNode::visit()
	{
		return getChild(getResponseCondition());
	}
}
