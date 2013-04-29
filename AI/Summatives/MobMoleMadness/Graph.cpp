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
#include "Graph.h"

// Must be included after Common.h or errors will ensue...
#include <algorithm>

using namespace std;

CGraph::~CGraph()
{
	for (UINT32 uiIndex = 0; uiIndex < m_nodes.size(); uiIndex++)
	{
		delete m_nodes[uiIndex];
	}
}

void CGraph::Add(CNode* _pNode)
{
	m_nodes.push_back(_pNode);
}

void CGraph::Connect(CNode& _rA, CNode& _rB, FLOAT32 _fDistance)
{
	_rA.AddEdge(CEdge(&_rA, &_rB, _fDistance));
	_rB.AddEdge(CEdge(&_rB, &_rA, _fDistance));
}

const vector<CNode*>& CGraph::GetAll() const
{
	return m_nodes;
}

vector<CNode*> CGraph::GetAllWithinRange(const CFVec2& _krvPosition, FLOAT32 _fRange) const
{
	vector<CNode*> nodesWithinRange;

	for (UINT32 uiIndex = 0; uiIndex < m_nodes.size(); uiIndex++)
	{
		if ((_krvPosition - m_nodes[uiIndex]->GetPosition()).Magnitude() <= _fRange)
		{
			nodesWithinRange.push_back(m_nodes[uiIndex]);
		}
	}

	return nodesWithinRange;
}

CNode* CGraph::GetClosest(const CFVec2& _krvPosition) const
{
	CNode* pClosest = NULL;
	FLOAT32 fClosestDistance = 0.0f;

	for (UINT32 uiIndex = 0; uiIndex < m_nodes.size(); uiIndex++)
	{
		CNode* pNode = m_nodes[uiIndex];
		FLOAT32 fNodeDistance = (_krvPosition - pNode->GetPosition()).Magnitude();

		if (pClosest == NULL || fNodeDistance < fClosestDistance)
		{
			pClosest = pNode;
			fClosestDistance = fNodeDistance;
		}
	}

	return pClosest;
}
