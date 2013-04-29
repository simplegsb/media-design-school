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
#ifndef PATHFINDINGAGENT_H_
#define PATHFINDINGAGENT_H_

#include "Common.h"

#include "FVector2.h"
#include "Mutex.h"
#include "Node.h"

// Must be included after Common.h or errors will ensue...
#include <vector>

class CMole;

class CPathFindingAgent
{
	public:
		CPathFindingAgent(const CMole& _krMole);

		void AdvanceDestinationOnPath(float _fDistance);

		void Draw() const;

		void FindDestinationOnPath();

		void FindPath();

		void FindPathOnSeparateThread();

		const CFVec2& GetDestination() const;

		void Reset();

		void Update(FLOAT32 _fTimeDelta);

	private:
		bool m_bEndReached;

		bool m_bFindingPath;

		bool m_bPathInvalid;

		FLOAT32 m_fPathDeltaTime;

		int m_iNextWaypointIndex;

		int m_iPreviousWaypointIndex;

		const CMole& m_krMole;

		CMutex m_mChangePathMutex;

		CMutex m_mFindPathMutex;

		std::vector<CNode*> m_path;

		CFVec2 m_vDestination;

		CPathFindingAgent(const CPathFindingAgent& original);

		CPathFindingAgent& operator=(const CPathFindingAgent& original);
};

#endif /* PATHFINDINGAGENT_H_ */
