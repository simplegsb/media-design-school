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
#include "Node.h"

using namespace std;

CNode::CNode(const CFVec2& _krvPosition) :
	m_vPosition(_krvPosition)
{
}

void CNode::AddEdge(const CEdge& _krEdge)
{
	m_edges.push_back(_krEdge);
}

const vector<CEdge>& CNode::GetEdges() const
{
	return m_edges;
}

const CFVec2& CNode::GetPosition() const
{
	return m_vPosition;
}
