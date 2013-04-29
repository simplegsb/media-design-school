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
#ifndef GRAPH_H_
#define GRAPH_H_

#include "Node.h"

class CGraph
{
	public:
		~CGraph();

		void Add(CNode* _pNode);

		void Connect(CNode& _rA, CNode& _rB, FLOAT32 _fDistance);

		const std::vector<CNode*>& GetAll() const;

		std::vector<CNode*> GetAllWithinRange(const CFVec2& _krvPosition, FLOAT32 _fRange) const;

		CNode* GetClosest(const CFVec2& _krvPosition) const;

	private:
		std::vector<CNode*> m_nodes;
};

#endif /* GRAPH_H_ */
