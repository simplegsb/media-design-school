//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// Copyright (c) 2012 Media Design School
//
// Author: Gary Buyn
//
// SVN Info
// $Author: MEDIADESIGN\gary.buyn $
// $Date: 2013-01-23 09:40:30 +1300 (Wed, 23 Jan 2013) $
// $Rev: 1108 $
//
#include "AStarSearch.h"

// Must be included after Common.h or errors will ensue...
#include <algorithm>

using namespace std;

CAStarSearch::CAStarSearch() :
	m_closedNodes(),
	m_distancesTotal(),
	m_distancesTravelled(),
	m_openNodes(),
	m_previousNodes()
{
}

FLOAT32 CAStarSearch::ComputeHeuristic(const CNode* _pNode, const CNode* _pFinish) const
{
	return (_pFinish->GetPosition() - _pNode->GetPosition()).Magnitude();
}

CNode* CAStarSearch::GetClosestOpenNode() const
{
	FLOAT32 fDistance = 0;
	CNode* pNode = NULL;

	for (map<CNode*, FLOAT32>::const_iterator iter = m_distancesTotal.begin();
		iter != m_distancesTotal.end(); iter++)
	{
		if (find(m_openNodes.begin(), m_openNodes.end(), iter->first) != m_openNodes.end() &&
			(pNode == NULL || iter->second < fDistance))
		{
			fDistance = iter->second;
			pNode = iter->first;
		}
	}

	return pNode;
}

vector<CNode*> CAStarSearch::ReconstructPath(CNode* _pStart, CNode* _pFinish) const
{
	vector<CNode*> path;
	path.push_back(_pFinish);

	CNode* pNode = _pFinish;
	while (pNode != _pStart)
	{
		pNode = m_previousNodes.find(pNode)->second;
		path.push_back(pNode);
	}

	reverse(path.begin(), path.end());
	return path;
}

vector<CNode*> CAStarSearch::Search(CNode* _pStart, CNode* _pFinish)
{
	m_openNodes.push_back(_pStart);
 
	m_distancesTravelled[_pStart] = 0;
	m_distancesTotal[_pStart] = ComputeHeuristic(_pStart, _pFinish);
 
	while (!m_openNodes.empty())
	{
		CNode* pNode = GetClosestOpenNode();
		if (pNode == _pFinish)
		{
			return ReconstructPath(_pStart, _pFinish);
		}
 
		m_openNodes.erase(remove(m_openNodes.begin(), m_openNodes.end(), pNode));
		m_closedNodes.push_back(pNode);

		for (UINT32 uiEdgeIndex = 0; uiEdgeIndex < pNode->GetEdges().size(); uiEdgeIndex++)
		{
			const CEdge& krEdge = pNode->GetEdges()[uiEdgeIndex];
			CNode* pDestination = krEdge.GetDestination();

			if (find(m_closedNodes.begin(), m_closedNodes.end(), pDestination) != m_closedNodes.end())
			{
                 continue;
			}

			FLOAT32 fDistanceTravelled = m_distancesTravelled[pNode] + krEdge.GetDistance();
 
			if (find(m_openNodes.begin(), m_openNodes.end(), pDestination) == m_openNodes.end() ||
				fDistanceTravelled < m_distancesTravelled[pDestination])
			{
				if (find(m_openNodes.begin(), m_openNodes.end(), pDestination) == m_openNodes.end())
				{
                     m_openNodes.push_back(pDestination);
				}

				// Although std::vector::operator[] creates an instance using the default constructor when a new key is
				// used, this is needed because VLD seems to think it creates a memory leak. Maybe it does?
				if (m_previousNodes.find(pDestination) == m_previousNodes.end())
				{
					m_previousNodes.insert(pair<CNode*, CNode*>(pDestination, NULL));
					m_distancesTravelled.insert(pair<CNode*, FLOAT32>(pDestination, 0.0f));
					m_distancesTotal.insert(pair<CNode*, FLOAT32>(pDestination, 0.0f));
				}

				m_previousNodes[pDestination] = pNode;
				m_distancesTravelled[pDestination] = fDistanceTravelled;
				m_distancesTotal[pDestination] = fDistanceTravelled + ComputeHeuristic(pDestination, _pFinish);
			}
		}
	}

	return vector<CNode*>();
}
