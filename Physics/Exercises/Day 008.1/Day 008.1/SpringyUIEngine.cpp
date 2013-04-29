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
#include <algorithm>
#include <cmath>

#include <gazengine/Events.h>
#include <gazengine/GazEngine.h>
#include <gazengine/input/MouseButtonEvent.h>

#include "EntityCategories.h"
#include "SpringyUIEngine.h"

using namespace std;

const float DRAG = 0.97f;

CSpringyUIEngine::CSpringyUIEngine() :
	m_menuItems(),
	m_previousPositions(),
	m_uiButtonIndex(0)
{
}

void CSpringyUIEngine::addEntity(Entity* entity)
{
	if (entity->getCategory() == EntityCategories::MENU_ITEM)
	{
		GDIMesh* pMenuItem = entity->getSingleComponent<GDIMesh>();
		m_menuItems.push_back(pMenuItem);
		m_previousPositions[pMenuItem].reserve(4);
		m_previousPositions[pMenuItem].push_back(pMenuItem->getVertices()[0]);
		m_previousPositions[pMenuItem].push_back(pMenuItem->getVertices()[1]);
		m_previousPositions[pMenuItem].push_back(pMenuItem->getVertices()[2]);
		m_previousPositions[pMenuItem].push_back(pMenuItem->getVertices()[3]);
	}
}

void CSpringyUIEngine::advance()
{
	float k = 20.0f;

	for (unsigned int uiIndex = 0; uiIndex < m_menuItems.size(); uiIndex++)
	{
		GDIMesh* pMenuItem = m_menuItems[uiIndex];
		float fHalfHeight = (pMenuItem->getVertices()[2].y - pMenuItem->getVertices()[0].y) / 2.0f;
		float fHalfWidth = (pMenuItem->getVertices()[1].x - pMenuItem->getVertices()[0].x) / 2.0f;
		Vector2 center(pMenuItem->getVertices()[0].x + fHalfWidth,
			pMenuItem->getVertices()[0].y + fHalfHeight);

		// Top left.
		{
			Vector2 equilibrium(-100.0f, -50.0f);
			Vector2 corner = pMenuItem->getVertices()[0];
			Vector2 toCorner = corner - center;

			// Hooke's law
			Vector2 accelDueToSpring = (toCorner - equilibrium) * -k; // Mass of 1

			// Verlet integration
			Vector2 aDtSq = accelDueToSpring * pow(GazEngine::getDeltaTime(), 2.0f);
			//Vector2 velocity = corner - tempCornerPrevious;
			//Vector2 newPosition = corner + velocity + aDtSq;
			Vector2 newCorner = pMenuItem->getVertices()[0] * (1 + DRAG) -
				m_previousPositions[pMenuItem][0] * DRAG + aDtSq;
			m_previousPositions[pMenuItem][0] = corner;
			pMenuItem->getVertices()[0] = newCorner;
		}

		// Top right.
		{
			Vector2 equilibrium(100.0f, -50.0f);
			Vector2 corner = pMenuItem->getVertices()[1];
			Vector2 toCorner = corner - center;

			// Hooke's law
			Vector2 accelDueToSpring = (toCorner - equilibrium) * -k; // Mass of 1

			// Verlet integration
			Vector2 aDtSq = accelDueToSpring * pow(GazEngine::getDeltaTime(), 2.0f);
			//Vector2 velocity = corner - tempCornerPrevious;
			//Vector2 newPosition = corner + velocity + aDtSq;
			Vector2 newCorner = pMenuItem->getVertices()[1] * (1 + DRAG) -
				m_previousPositions[pMenuItem][1] * DRAG + aDtSq;
			m_previousPositions[pMenuItem][1] = corner;
			pMenuItem->getVertices()[1] = newCorner;
		}

		// Bottom left.
		{
			Vector2 equilibrium(-100.0f, 50.0f);
			Vector2 corner = pMenuItem->getVertices()[2];
			Vector2 toCorner = corner - center;

			// Hooke's law
			Vector2 accelDueToSpring = (toCorner - equilibrium) * -k; // Mass of 1

			// Verlet integration
			Vector2 aDtSq = accelDueToSpring * pow(GazEngine::getDeltaTime(), 2.0f);
			//Vector2 velocity = corner - tempCornerPrevious;
			//Vector2 newPosition = corner + velocity + aDtSq;
			Vector2 newCorner = pMenuItem->getVertices()[2] * (1 + DRAG) -
				m_previousPositions[pMenuItem][2] * DRAG + aDtSq;
			m_previousPositions[pMenuItem][2] = corner;
			pMenuItem->getVertices()[2] = newCorner;
		}

		// Bottom right.
		{
			Vector2 equilibrium(100.0f, 50.0f);
			Vector2 corner = pMenuItem->getVertices()[3];
			Vector2 toCorner = corner - center;

			// Hooke's law
			Vector2 accelDueToSpring = (toCorner - equilibrium) * -k; // Mass of 1

			// Verlet integration
			Vector2 aDtSq = accelDueToSpring * pow(GazEngine::getDeltaTime(), 2.0f);
			//Vector2 velocity = corner - tempCornerPrevious;
			//Vector2 newPosition = corner + velocity + aDtSq;
			Vector2 newCorner = pMenuItem->getVertices()[3] * (1 + DRAG) -
				m_previousPositions[pMenuItem][3] * DRAG + aDtSq;
			m_previousPositions[pMenuItem][3] = corner;
			pMenuItem->getVertices()[3] = newCorner;
		}
	}
}

void CSpringyUIEngine::destroy()
{
	Messages::deregisterRecipient(Events::MOUSE_BUTTON, this);
}

void CSpringyUIEngine::init()
{
	Messages::registerRecipient(Events::MOUSE_BUTTON, this);
}

void CSpringyUIEngine::receive(unsigned short subject, const void* message)
{
	const MouseButtonEvent* pEvent = static_cast<const MouseButtonEvent*>(message);

	if (pEvent->button == Mouse::LEFT && pEvent->buttonState == Button::UP)
	{
		m_uiButtonIndex++;
		if (m_uiButtonIndex == m_menuItems.size())
		{
			m_uiButtonIndex = 0;
		}
		SelectButton(m_uiButtonIndex);
	}
}

void CSpringyUIEngine::removeEntity(const Entity& entity)
{
	if (entity.getCategory() == EntityCategories::MENU_ITEM)
	{
		GDIMesh* pMenuItem = entity.getSingleComponent<GDIMesh>();
		m_menuItems.erase(remove(m_menuItems.begin(), m_menuItems.end(), pMenuItem));
		m_previousPositions.erase(pMenuItem);
	}
}

void CSpringyUIEngine::SelectButton(unsigned int _uiIndex)
{
	GDIMesh* pMenuItem = m_menuItems[_uiIndex];
	float fHalfHeight = (pMenuItem->getVertices()[2].y - pMenuItem->getVertices()[0].y) / 2.0f;
	float fHalfWidth = (pMenuItem->getVertices()[1].x - pMenuItem->getVertices()[0].x) / 2.0f;
	Vector2 center(pMenuItem->getVertices()[0].x + fHalfWidth,
		pMenuItem->getVertices()[0].y + fHalfHeight);

	pMenuItem->getVertices()[0] = center + Vector2(-110, -60);
	pMenuItem->getVertices()[1] = center + Vector2(110, -60);
	pMenuItem->getVertices()[2] = center + Vector2(-110, 60);
	pMenuItem->getVertices()[3] = center + Vector2(110, 60);
	m_previousPositions[pMenuItem][0] = center + Vector2(-105, -55);
	m_previousPositions[pMenuItem][1] = center + Vector2(105, -55);
	m_previousPositions[pMenuItem][2] = center + Vector2(-105, 55);
	m_previousPositions[pMenuItem][3] = center + Vector2(105, 55);
}
