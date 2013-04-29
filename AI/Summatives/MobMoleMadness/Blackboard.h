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
#ifndef BLACKBOARD_H_
#define BLACKBOARD_H_

#include "Common.h"

#include "Acorn.h"
#include "Circle.h"
#include "Graph.h"
#include "Mole.h"
#include "Wall.h"

// Must be included after Common.h or errors will ensue...
#include <vector>

class CBlackboard
{
	public:
		CBlackboard();

		void AddAcorn(const CAcorn& _krAcorn, const CMole& _krFinder);

		void AddMole(const CMole& _krMole);

		void AddRepulsionZone(const CCircle& _krRepulsionZone);

		void AddWall(const CWall& _krWall);

		void AddWaypoint(const CFVec2& _krvPosition);

		void Clear();

		void ConnectToAllWithinRange(CNode& _krNode);

		void Draw() const;

		const std::vector<const CAcorn*>& GetAcorns() const;

		/**
		 * Retrieves the closest intersection time on the given whisker when intersected with all of the walls.
		 * 
		 * @param _krvWhiskerStart The start point of the whisker.
		 * @param _krvWhisker The whisker.
		 * 
		 * @return The closest intersection time, or -1.0f if no intersections were found.
		 */
		FLOAT32 GetClosestWallIntersectionTime(const CFVec2& _krvWhiskerStart, const CFVec2& _krvWhisker) const;

		/**
		 * Retrieves the singleton instance.
		 * 
		 * @return The singleton instance.
		 */
		static CBlackboard& GetInstance();

		const std::vector<const CMole*>& GetMoles() const;

		const CGraph& GetNavigationMesh() const;

		const std::vector<CCircle>& GetRepulsionZones() const;
		
		void RemoveAcorn(const CAcorn& _krAcorn);

	private:
		std::vector<const CAcorn*> m_acorns;

		std::vector<const CMole*> m_moles;

		CGraph m_navigationMesh;

		std::vector<CCircle> m_repulsionZones;

		std::vector<const CWall*> m_walls;
};

#endif /* BLACKBOARD_H_ */
