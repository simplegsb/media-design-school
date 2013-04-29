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
#include "Events.h"
#include "MMMContext.h"
#include "Mole.h"
#include "SensorAgent.h"
#include "Squirrel.h"

// Must be included after Common.h or errors will ensue...
#include <algorithm>
#include <cmath>

using namespace std;

CSensorAgent::CSensorAgent(const CMole& _krMole, FLOAT32 _fRange) :
	m_fRange(_fRange),
	m_krMole(_krMole)
{
}

vector<CAcorn*> CSensorAgent::GetAcornsInRange() const
{
	vector<CAcorn*> closeAcorns;

	CAcorn* pAcorns = NULL;
	UINT32 uiAcornCount = 0;
	CMMMContext::GetInstance().GetAcorns(pAcorns, uiAcornCount);

	for (UINT32 uiIndex = 0; uiIndex < uiAcornCount; uiIndex++)
	{
		if (IsInRange(pAcorns[uiIndex].GetPosition()) &&
			pAcorns[uiIndex].GetState() == CAcorn::ES_Available)
		{
			closeAcorns.push_back(&pAcorns[uiIndex]);
		}
	}

	return closeAcorns;
}

vector<CFVec2> CSensorAgent::GetWaypointsInRange() const
{
	vector<CFVec2> waypoints;
	FLOAT32 fTopLeftX = m_krMole.GetPosition().X() - m_fRange;
	FLOAT32 fTopLeftY = m_krMole.GetPosition().Y() - m_fRange;
	FLOAT32 fBottomRightX = m_krMole.GetPosition().X() + m_fRange;
	FLOAT32 fBottomRightY = m_krMole.GetPosition().Y() + m_fRange;

	for (FLOAT32 fX = ceil(fTopLeftX / fGRID_SIZE) * fGRID_SIZE; fX <= fBottomRightX; fX += fGRID_SIZE)
	{
		for (FLOAT32 fY = ceil(fTopLeftY / fGRID_SIZE) * fGRID_SIZE; fY <= fBottomRightY; fY += fGRID_SIZE)
		{
			CFVec2 vWaypoint = SFVec2(fX, fY);
			if ((vWaypoint - m_krMole.GetPosition()).Magnitude() <= m_fRange)
			{
				waypoints.push_back(vWaypoint);
			}
		}
	}

	return waypoints;
}

bool CSensorAgent::IsInRange(const CFVec2& _krvPosition) const
{
	return (m_krMole.GetPosition() - _krvPosition).Magnitude() <= m_fRange;
}

void CSensorAgent::Update()
{
	CBlackboard& rBlackboard = CBlackboard::GetInstance();

	const vector<const CAcorn*>& knownAcorns = rBlackboard.GetAcorns();
	for (UINT32 uiIndex = 0; uiIndex < knownAcorns.size(); uiIndex++)
	{
		if (IsInRange(knownAcorns[uiIndex]->GetPosition()) &&
			knownAcorns[uiIndex]->GetState() != CAcorn::ES_Available)
		{
			Messages::send(TARGET_LOST_EVENT, knownAcorns[uiIndex]);
		}
	}

	vector<CAcorn*> acornsInRange = GetAcornsInRange();
	for (UINT32 uiIndex = 0; uiIndex < acornsInRange.size(); uiIndex++)
	{
		rBlackboard.AddAcorn(*acornsInRange[uiIndex], m_krMole);
	}

	vector<CFVec2> waypointsInRange = GetWaypointsInRange();
	for (UINT32 uiIndex = 0; uiIndex < waypointsInRange.size(); uiIndex++)
	{
		rBlackboard.AddWaypoint(waypointsInRange[uiIndex]);
	}
}
