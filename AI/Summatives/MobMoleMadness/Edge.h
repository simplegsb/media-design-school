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
#ifndef EDGE_H_
#define EDGE_H_

#include "Common.h"

class CNode;

class CEdge
{
	public:
		CEdge(CNode* _pSource, CNode* _pDestination, FLOAT32 _fDistance);

		CNode* GetDestination() const;

		FLOAT32 GetDistance() const;

		CNode* GetSource() const;

	private:
		FLOAT32 m_fDistance;

		CNode* m_pDestination;

		CNode* m_pSource;
};

#endif /* EDGE_H_ */
