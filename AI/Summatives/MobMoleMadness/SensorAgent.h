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
#ifndef SENSORAGENT_H_
#define SENSORAGENT_H_

#include "Acorn.h"

#include <vector>

class CMole;

class CSensorAgent
{
	public:
		CSensorAgent(const CMole& _krMole, FLOAT32 _fRange);

		void Update();

	private:
		FLOAT32 m_fRange;

		const CMole& m_krMole;

		CSensorAgent(const CSensorAgent& _krOriginal);

		std::vector<CAcorn*> GetAcornsInRange() const;

		std::vector<CFVec2> GetWaypointsInRange() const;

		bool IsInRange(const CFVec2& _krvPosition) const;

		CSensorAgent& operator=(const CSensorAgent& _krOriginal);
};

#endif /* SENSORAGENT_H_ */
