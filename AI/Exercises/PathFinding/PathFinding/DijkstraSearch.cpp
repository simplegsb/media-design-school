#include "stdafx.h"

#include <algorithm>

#include "DijkstraSearch.h"

using namespace std;

Node* DijkstraSearch::GetClosestNode(const vector<Node*>& nodes) const
{
	int iDistance = 0;
	Node* pNode = NULL;

	for (map<Node*, int>::const_iterator iter = m_distances.begin(); iter != m_distances.end(); iter++)
	{
		if (find(nodes.begin(), nodes.end(), iter->first) != nodes.end() &&
			(pNode == NULL || iter->second < iDistance))
		{
			iDistance = iter->second;
			pNode = iter->first;
		}
	}

	return pNode;
}

vector<Node*> DijkstraSearch::Search(const Graph& graph, Node* _pStart, Node* _pFinish)
{
	m_distances.insert(pair<Node*, int>(_pStart, 0));
	vector<Node*> nodes = graph.GetAll();

	while (!nodes.empty())
	{
		Node* pNode = GetClosestNode(nodes);
		nodes.erase(remove(nodes.begin(), nodes.end(), pNode));

		if (pNode == _pFinish)
		{
			break;
		}

		for (unsigned int uiIndex = 0; uiIndex < pNode->GetOutgoingEdges().size(); uiIndex++)
		{
			Edge* pOutgoingEdge = pNode->GetOutgoingEdges().at(uiIndex);
			Node* pDestination = pOutgoingEdge->GetDestination();
			if (find(nodes.begin(), nodes.end(), pDestination) == nodes.end())
			{
				continue;
			}

			int iDistance = m_distances.find(pNode)->second + pOutgoingEdge->GetWeight();
            if (m_distances.find(pDestination) == m_distances.end())
			{
				m_distances.insert(pair<Node*, int>(pDestination, iDistance));
				m_previousNodes.insert(pair<Node*, Node*>(pDestination, pNode));
			}
			else if (iDistance < m_distances.find(pDestination)->second)
			{
				m_distances.find(pDestination)->second = iDistance;
				m_previousNodes.find(pDestination)->second = pNode;
			}
		}
	}

	vector<Node*> shortestPath;
	shortestPath.push_back(_pFinish);

	Node* pNode = _pFinish;
	while (pNode != _pStart)
	{
		pNode = m_previousNodes.find(pNode)->second;
		shortestPath.push_back(pNode);
	}

	reverse(shortestPath.begin(), shortestPath.end());

	return shortestPath;
}
