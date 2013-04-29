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
#include "Common.h"

#include <windows.h>

#include "AStarSearch.h"
#include "Blackboard.h"
#include "MMMContext.h"
#include "Mole.h"
#include "PathFindingAgent.h"

using namespace std;

const FLOAT32 fREPATH_FREQUENCY = 2.0f;

DWORD WINAPI FindPathThread(LPVOID _data)
{
	CPathFindingAgent* pAgent = static_cast<CPathFindingAgent*>(_data);
	pAgent->FindPath();

	return 0;
}

CPathFindingAgent::CPathFindingAgent(const CMole& _krMole) :
	m_bEndReached(false),
	m_bFindingPath(false),
	m_bPathInvalid(false),
	m_fPathDeltaTime(0.0f),
	m_iNextWaypointIndex(-1),
	m_iPreviousWaypointIndex(-1),
	m_krMole(_krMole),
	m_mChangePathMutex(),
	m_mFindPathMutex(),
	m_path(),
	m_vDestination()
{
}

void CPathFindingAgent::AdvanceDestinationOnPath(float _fDistance)
{
	if (m_iPreviousWaypointIndex == -1)
	{
		m_iPreviousWaypointIndex = 0;
		m_iNextWaypointIndex = 1;
	}

	CFVec2 vAdvancement =
		m_path.at(m_iNextWaypointIndex)->GetPosition() -
		m_path.at(m_iPreviousWaypointIndex)->GetPosition();
	vAdvancement.Normalise();
	vAdvancement *= _fDistance;
	CFVec2 vDestinationCopy = m_vDestination;
	vDestinationCopy += vAdvancement; 

	float fDistancePastNextWaypoint =
		(vDestinationCopy - m_path.at(m_iPreviousWaypointIndex)->GetPosition()).Magnitude() -
		(m_path.at(m_iNextWaypointIndex)->GetPosition() - m_path.at(m_iPreviousWaypointIndex)->GetPosition()).Magnitude();

	if (fDistancePastNextWaypoint > 0.0f)
	{
		m_vDestination = m_path.at(m_iNextWaypointIndex)->GetPosition();

		if (m_path.size() > (unsigned) m_iNextWaypointIndex + 1)
		{
			m_iPreviousWaypointIndex++;
			m_iNextWaypointIndex++;

			AdvanceDestinationOnPath(fDistancePastNextWaypoint);
		}
	}
	else
	{
		m_vDestination += vAdvancement;
	}
}

void CPathFindingAgent::Draw() const
{
	if (m_path.size() > 1)
	{
		for (UINT32 uiIndex = 0; uiIndex < m_path.size() - 1; uiIndex++)
		{
			CCircle waypointCircle;
			waypointCircle.Initialise(m_path[uiIndex]->GetPosition(), 5.0f);
			CMMMContext::GetInstance().DrawCircle(waypointCircle, 0xFFBFBD2B);
			CMMMContext::GetInstance().DrawLine(m_path[uiIndex]->GetPosition(), m_path[uiIndex + 1]->GetPosition(), 0xFFBFBD2B);
		}
		
		CCircle waypointCircle;
		waypointCircle.Initialise(m_path.back()->GetPosition(), 5.0f);
		CMMMContext::GetInstance().DrawCircle(waypointCircle, 0xFFBFBD2B);
		
		CCircle destinationCircle;
		destinationCircle.Initialise(m_vDestination, fGRID_SIZE);
		CMMMContext::GetInstance().DrawCircle(destinationCircle, 0xFFBFBD2B);
	}
}

void CPathFindingAgent::FindDestinationOnPath()
{
	if (m_iPreviousWaypointIndex == -1)
	{
		m_vDestination = m_path.at(0)->GetPosition();
	}

	if ((m_vDestination - m_krMole.GetPosition()).Magnitude() <= fGRID_SIZE * 1.5f)
	{
		AdvanceDestinationOnPath(fGRID_SIZE * 1.5f);
	}
}

void CPathFindingAgent::FindPath()
{
	CScopedLock findPathLock(m_mFindPathMutex);

	bool bValidatePath = m_bPathInvalid;
	const CGraph& navigationMesh = CBlackboard::GetInstance().GetNavigationMesh();

	CAStarSearch searcher;
	vector<CNode*> newPath = searcher.Search(navigationMesh.GetClosest(m_krMole.GetPosition()),
		navigationMesh.GetClosest(m_krMole.GetDestination()));

	CScopedLock changePathLock(m_mChangePathMutex);

	m_iNextWaypointIndex = -1;
	m_iPreviousWaypointIndex = -1;
	m_path = newPath;

	if (bValidatePath)
	{
		m_bPathInvalid = false;
	}
}

void CPathFindingAgent::FindPathOnSeparateThread()
{
	LPDWORD pThreadId = NULL;
	HANDLE findPathThread = CreateThread(NULL, 0, &FindPathThread, this, 0, pThreadId);
	if(findPathThread == NULL)
    {
		// Failed to create the thread, fall back to the frame rate-killing single-threaded solution.
		FindPath();
    }
}

const CFVec2& CPathFindingAgent::GetDestination() const
{
	return m_vDestination;
}

void CPathFindingAgent::Reset()
{
	CScopedLock changePathLock(m_mChangePathMutex);

	m_bEndReached = false;
	m_bPathInvalid = true;
	m_iNextWaypointIndex = -1;
	m_iPreviousWaypointIndex = -1;
	m_path.clear();
}

void CPathFindingAgent::Update(FLOAT32 _fTimeDelta)
{
	m_fPathDeltaTime += _fTimeDelta;

	// No waypoints discovered? Not much point in path finding then...
	if (CBlackboard::GetInstance().GetNavigationMesh().GetAll().empty())
	{
		m_vDestination = m_krMole.GetDestination();
		return;
	}

	if (!m_mFindPathMutex.IsLocked() &&
		m_fPathDeltaTime > 1.0f / fREPATH_FREQUENCY)
	{
		FindPath();
		// An as yet unpredictable error was occuring when multi-threading so to be safe this is submitted with
		// multi-threading disabled.
		//FindPathOnSeparateThread();
	}

	CScopedLock changePathLock(m_mChangePathMutex);

	if (m_path.size() <= 1 ||
		m_bEndReached ||
		m_bPathInvalid)
	{
		m_vDestination = m_krMole.GetDestination();
	}
	else
	{
		if ((m_path.back()->GetPosition() - m_krMole.GetPosition()).Magnitude() <= fGRID_SIZE * 1.5f)
		{
			m_bEndReached = true;
		}

		FindDestinationOnPath();
	}
}
