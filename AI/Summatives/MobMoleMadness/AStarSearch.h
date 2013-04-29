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
#ifndef ASTARSEARCH_H_
#define ASTARSEARCH_H_

#include "Node.h"

// Must be included after Common.h or errors will ensue...
#include <map>

class CAStarSearch
{
	public:
		CAStarSearch();

		std::vector<CNode*> Search(CNode* _pStart, CNode* _pFinish);

	private:
		/**
		 * The nodes that have already been visited.
		 */
		std::vector<CNode*> m_closedNodes;

		/**
		 * The (estimated) total distances of the path when travelling via the (key) nodes discovered so far.
		 */
		std::map<CNode*, FLOAT32> m_distancesTotal;

		/**
		 * The shortest distances to the (key) nodes discovered so far.
		 */
		std::map<CNode*, FLOAT32> m_distancesTravelled;

		/**
		 * The nodes that have already been found but not yet visited.
		 */
		std::vector<CNode*> m_openNodes;

		/**
		 * The previous nodes on the shortest path to the (key) nodes discovered so far.
		 */
		std::map<CNode*, CNode*> m_previousNodes;

		FLOAT32 ComputeHeuristic(const CNode* _pNode, const CNode* _pFinish) const;

		CNode* GetClosestOpenNode() const;

		std::vector<CNode*> ReconstructPath(CNode* _pStart, CNode* _pFinish) const;
};

#endif /* ASTARSEARCH_H_ */
