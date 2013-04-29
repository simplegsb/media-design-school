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
#ifndef NODE_H_
#define NODE_H_

#include "Common.h"

#include "Edge.h"
#include "FVector2.h"

// Must be included after Common.h or errors will ensue...
#include <vector>

class CNode
{
	public:
		CNode(const CFVec2& _krvPosition);

		void AddEdge(const CEdge& _krEdge);

		const std::vector<CEdge>& GetEdges() const;

		const CFVec2& GetPosition() const;

	private:
		std::vector<CEdge> m_edges;

		CFVec2 m_vPosition;
};

#endif /* NODE_H_ */
