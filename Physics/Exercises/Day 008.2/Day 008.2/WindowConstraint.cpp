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
#include "WindowConstraint.h"

CWindowConstraint::CWindowConstraint() :
	m_bodies()
{
}

void CWindowConstraint::AddBody(Body* _pBody)
{
	m_bodies.push_back(_pBody);
}

void CWindowConstraint::apply()
{
	for (unsigned int uiIndex = 0; uiIndex < m_bodies.size(); uiIndex++)
	{
		if (m_bodies[uiIndex]->getPosition().x < 0.0f)
		{
			m_bodies[uiIndex]->setPosition(Vector2(0.0f, m_bodies[uiIndex]->getPosition().y));
		}

		if (m_bodies[uiIndex]->getPosition().x > 800.0f)
		{
			m_bodies[uiIndex]->setPosition(Vector2(800.0f, m_bodies[uiIndex]->getPosition().y));
		}

		if (m_bodies[uiIndex]->getPosition().y < 0.0f)
		{
			m_bodies[uiIndex]->setPosition(Vector2(m_bodies[uiIndex]->getPosition().x, 0.0f));
		}

		if (m_bodies[uiIndex]->getPosition().y > 600.0f)
		{
			m_bodies[uiIndex]->setPosition(Vector2(m_bodies[uiIndex]->getPosition().x, 600.0f));
		}
	}
}
