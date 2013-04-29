#pragma once

#include <map>
#include <queue>

#include "Node.h"

class BreadthFirstSearch
{
	public:
		std::vector<Node*> Search(Node* _pStart, Node* _pFinish);

	private:
		std::vector<Node*> m_closedNodes;

		std::queue<Edge*> m_openEdges;

		std::map<Node*, int> m_distances;
};
