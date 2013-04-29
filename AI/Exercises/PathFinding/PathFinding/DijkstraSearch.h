#pragma once

#include <map>

#include "Graph.h"

class DijkstraSearch
{
	public:
		std::vector<Node*> Search(const Graph& graph, Node* _pStart, Node* _pFinish);

	private:
		std::map<Node*, int> m_distances;

		std::map<Node*, Node*> m_previousNodes;

		Node* GetClosestNode(const std::vector<Node*>& nodes) const;
};
