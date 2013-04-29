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
#include "Blackboard.h"
#include "Circle.h"
#include "Events.h"
#include "MMMContext.h"

// Must be included after Common.h or errors will ensue...
#include <algorithm>

using namespace std;

CBlackboard::CBlackboard() :
	m_acorns(),
	m_moles(),
	m_navigationMesh(),
	m_repulsionZones()
{
}

void CBlackboard::AddAcorn(const CAcorn& _krAcorn, const CMole& _krFinder)
{
	if (find(m_acorns.begin(), m_acorns.end(), &_krAcorn) == m_acorns.end())
	{
		m_acorns.push_back(&_krAcorn);
		Messages::send(ACORN_FOUND_EVENT, &_krFinder);
	}
}

void CBlackboard::AddMole(const CMole& _krMole)
{
	m_moles.push_back(&_krMole);
}

void CBlackboard::AddRepulsionZone(const CCircle& _krRepulsionZone)
{
	m_repulsionZones.push_back(_krRepulsionZone);
}

void CBlackboard::AddWall(const CWall& _krWall)
{
	m_walls.push_back(&_krWall);
}

void CBlackboard::AddWaypoint(const CFVec2& _krvPosition)
{
	CNode* pClosestExisting = m_navigationMesh.GetClosest(_krvPosition);

	if (pClosestExisting == NULL ||
		pClosestExisting->GetPosition().X() != _krvPosition.X() ||
		pClosestExisting->GetPosition().Y() != _krvPosition.Y())
	{
		CNode* pNode = new CNode(_krvPosition);
		ConnectToAllWithinRange(*pNode);
		m_navigationMesh.Add(pNode);
	}
}

void CBlackboard::Clear()
{
	m_acorns.clear();
	m_moles.clear();
	m_navigationMesh = CGraph();
	m_repulsionZones.clear();
	m_walls.clear();
}

void CBlackboard::ConnectToAllWithinRange(CNode& _krNode)
{
	vector<CNode*> nodesWithinRange = m_navigationMesh.GetAllWithinRange(_krNode.GetPosition(), 75.0f); // TODO MAGIC
	for (UINT32 uiIndex = 0; uiIndex < nodesWithinRange.size(); uiIndex++)
	{
		CNode* pOtherNode = nodesWithinRange.at(uiIndex);
		CFVec2 vDifference = pOtherNode->GetPosition() - _krNode.GetPosition();

		// Don't make a connection through a wall!!!
		if (GetClosestWallIntersectionTime(_krNode.GetPosition(), vDifference) == -1.0f)
		{
			m_navigationMesh.Connect(_krNode, *pOtherNode, vDifference.Magnitude());
		}
	}
}

void CBlackboard::Draw() const
{
	UINT32 knowledgeColour = 0xff00a2e8; // light blue

	for (UINT32 uiIndex = 0; uiIndex < m_acorns.size(); uiIndex++)
	{
		CCircle acornCircle;
		acornCircle.Initialise(m_acorns.at(uiIndex)->GetPosition(), fACORN_RADIUS);
		CMMMContext::GetInstance().DrawCircle(acornCircle, knowledgeColour);
	}

	const vector<CNode*>& waypoints = m_navigationMesh.GetAll();
	for (UINT32 uiWaypointIndex = 0; uiWaypointIndex < waypoints.size(); uiWaypointIndex++)
	{
		CCircle waypointCircle;
		waypointCircle.Initialise(waypoints.at(uiWaypointIndex)->GetPosition(), 5.0f);
		CMMMContext::GetInstance().DrawCircle(waypointCircle, knowledgeColour);

		const vector<CEdge>& edges = waypoints.at(uiWaypointIndex)->GetEdges();
		for (UINT32 uiEdgeIndex = 0; uiEdgeIndex < edges.size(); uiEdgeIndex++)
		{
			CMMMContext::GetInstance().DrawLine(edges.at(uiEdgeIndex).GetSource()->GetPosition(),
				edges.at(uiEdgeIndex).GetDestination()->GetPosition(), knowledgeColour);
		}
	}
}

const vector<const CAcorn*>& CBlackboard::GetAcorns() const
{
	return m_acorns;
}

FLOAT32 CBlackboard::GetClosestWallIntersectionTime(const CFVec2& _krvWhiskerStart, const CFVec2& _krvWhisker) const
{
	FLOAT32 fTime = -1.0f;
	FLOAT32 fTimeCurrent = 0.0f;

	for (unsigned int uiIndex = 0; uiIndex < m_walls.size(); uiIndex++)
	{
		if (m_walls.at(uiIndex)->GetLineIntersectionTime(_krvWhiskerStart, _krvWhisker, fTimeCurrent))
		{
			// If the intersetion is within the segment given.
			if (0.0f <= fTimeCurrent && fTimeCurrent <= 1.0f)
			{
				// If this is the closest intersection yet.
				if (fTime == -1.0f || fTimeCurrent < fTime)
				{
					fTime = fTimeCurrent;
				}
			}
		}
	}

	return fTime;
}

CBlackboard& CBlackboard::GetInstance()
{
	static CBlackboard instance;
	return instance;
}

const vector<const CMole*>& CBlackboard::GetMoles() const
{
	return m_moles;
}

const CGraph& CBlackboard::GetNavigationMesh() const
{
	return m_navigationMesh;
}

const std::vector<CCircle>& CBlackboard::GetRepulsionZones() const
{
	return m_repulsionZones;
}

void CBlackboard::RemoveAcorn(const CAcorn& _krAcorn)
{
	m_acorns.erase(remove(m_acorns.begin(), m_acorns.end(), &_krAcorn));
}
