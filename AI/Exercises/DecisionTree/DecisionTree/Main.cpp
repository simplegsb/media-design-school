#include <iostream>

#include "Conditions.h"
#include "DecisionTree.h"
#include "FinalNode.h"
#include "InputNode.h"
#include "RandomNode.h"

using namespace decisiontree;
using namespace std;

int main(int argc, char** argv)
{
	DecisionTree tree;
	
	DecisionTreeNode* attack = new FinalNode("Attack.");
	DecisionTreeNode* enemyIsNotVisible = new RandomNode;
	DecisionTreeNode* enemyIsVisible = new RandomNode;
	DecisionTreeNode* enemyVisible = new InputNode("Is there an enemy visible?");
	DecisionTreeNode* furtherThanFiftyMetersAway = new RandomNode;
	DecisionTreeNode* heardNoise = new InputNode("Heard a noise?");
	DecisionTreeNode* investigate = new FinalNode("Investigate.");
	DecisionTreeNode* lessThanFiftyMetersAway = new InputNode("Was it less than 50m away?");
	DecisionTreeNode* patrolRoute = new FinalNode("Patrol Route.");
	DecisionTreeNode* raiseAlarmAndAttack = new FinalNode("Raise alarm and attack.");
	DecisionTreeNode* sourcePointSeen = new InputNode("Can the source point be seen?");
	
	tree.setRoot(heardNoise);
	heardNoise->addChild(patrolRoute, Conditions::NO);
	heardNoise->addChild(lessThanFiftyMetersAway, Conditions::YES);
	lessThanFiftyMetersAway->addChild(furtherThanFiftyMetersAway, Conditions::NO);
	lessThanFiftyMetersAway->addChild(sourcePointSeen, Conditions::YES);
	furtherThanFiftyMetersAway->addChild(investigate, 1);
	furtherThanFiftyMetersAway->addChild(patrolRoute, 4);
	sourcePointSeen->addChild(investigate, Conditions::NO);
	sourcePointSeen->addChild(enemyVisible, Conditions::YES);
	enemyVisible->addChild(enemyIsNotVisible, Conditions::NO);
	enemyVisible->addChild(enemyIsVisible, Conditions::YES);
	enemyIsNotVisible->addChild(investigate, 3);
	enemyIsNotVisible->addChild(patrolRoute, 2);
	enemyIsVisible->addChild(attack, 1);
	enemyIsVisible->addChild(raiseAlarmAndAttack, 1);

	tree.search();

	string endDecision;
	cin >> endDecision;

	return 0;
}
