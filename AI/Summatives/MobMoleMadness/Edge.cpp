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
#include "Edge.h"

CEdge::CEdge(CNode* _pSource, CNode* _pDestination, FLOAT32 _fDistance) :
	m_fDistance(_fDistance),
	m_pDestination(_pDestination),
	m_pSource(_pSource)
{
}

CNode* CEdge::GetDestination() const
{
	return m_pDestination;
}

FLOAT32 CEdge::GetDistance() const
{
	return m_fDistance;
}

CNode* CEdge::GetSource() const
{
	return m_pSource;
}
