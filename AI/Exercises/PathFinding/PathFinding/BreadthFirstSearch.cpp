#include "stdafx.h"

#include "BreadthFirstSearch.h"

using namespace std;

vector<Node*> BreadthFirstSearch::Search(Node* _pStart, Node* _pFinish)
{
	Edge* pRootEdge = new Edge(NULL, _pStart, 1);
	m_openEdges.push(pRootEdge);
	m_distances.insert(pair<Node*, int>(_pStart, 0));

	while (!m_openEdges.empty())
	{
		Edge* pEdge = m_openEdges.front();
		Node* pNode = pEdge->GetDestination();
		m_openEdges.pop();

		if (pNode == _pFinish)
		{
			break;
		}

		int iPreviousDistance = 0;
		if (pEdge->GetSource() != NULL)
		{
			iPreviousDistance = m_distances.find(pEdge->GetSource())->second;
		}
		m_distances.insert(pair <Node*, int>(pNode, iPreviousDistance + 1));

		for (unsigned int uiIndex = 0; uiIndex < pNode->GetOutgoingEdges().size(); uiIndex++)
		{
			Edge* pOutgoingEdge = pNode->GetOutgoingEdges().at(uiIndex);
			if (find(m_closedNodes.begin(), m_closedNodes.end(), pOutgoingEdge->GetDestination())
				== m_closedNodes.end())
			{
				m_closedNodes.push_back(pNode);
				m_openEdges.push(pOutgoingEdge);
			}
		}
	}

	vector<Node*> shortestPath;
	shortestPath.push_back(_pFinish);

	Node* pNode = _pFinish;
	while (pNode != _pStart)
	{
		Node* pBestNode = NULL;
		int fBestScore = m_closedNodes.size();

		for (unsigned int uiIndex = 0; uiIndex < pNode->GetOutgoingEdges().size(); uiIndex++)
		{
			Edge* pEdge = pNode->GetOutgoingEdges().at(uiIndex);
			if (m_distances.find(pEdge->GetDestination()) != m_distances.end() &&
				m_distances.find(pEdge->GetDestination())->second < fBestScore)
			{
				pBestNode = pEdge->GetDestination();
				fBestScore = m_distances.find(pEdge->GetDestination())->second;
			}
		}

		shortestPath.push_back(pBestNode);
		pNode = pBestNode;
	}

	reverse(shortestPath.begin(), shortestPath.end());

	delete pRootEdge;

	return shortestPath;
}
