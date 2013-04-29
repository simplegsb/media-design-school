//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// Copyright (c) 2012 Media Design School
//
// SVN Info
// $Author: MEDIADESIGN\gary.buyn $
// $Date: 2013-01-23 09:43:23 +1300 (Wed, 23 Jan 2013) $
// $Rev: 1109 $
//
#include "FloorConstraint.h"

CFloorConstraint::CFloorConstraint(float _fAltitude) :
	m_bodies(),
	m_fAltitude(_fAltitude)
{
}

void CFloorConstraint::AddBody(Body* _pBody)
{
	m_bodies.push_back(_pBody);
}

void CFloorConstraint::apply()
{
	for (unsigned int uiIndex = 0; uiIndex < m_bodies.size(); uiIndex++)
	{
		/*float displacementMagnitude = m_bodies[uiIndex]->getPosition().y - m_fAltitude;
		if (displacementMagnitude > 0.0f)
		{
			Vector2 displacement(0.0f, -1.0f);
			displacement *= displacementMagnitude;
			m_bodies[uiIndex]->applyForce(displacement, m_bodies[uiIndex]->getPosition());
		}*/

		if (m_bodies[uiIndex]->getPosition().y > m_fAltitude)
		{
			m_bodies[uiIndex]->setPosition(Vector2(m_bodies[uiIndex]->getPosition().x, m_fAltitude));
		}
	}
}
