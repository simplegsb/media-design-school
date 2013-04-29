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

#include "Cloth.h"

using namespace std;

const float DRAG = 0.99f;
const float MASS = 5.0f;
const float RESTING_DISTANCE = 20.0f;
const float STIFFNESS = 0.8f;
const Vector2 TOP_LEFT(200.0f, 100.0f);

CCloth::CCloth(unsigned int _uiHeight, unsigned int _uiWidth) :
	m_links(),
	m_models(),
	m_points()
{
	Messages::registerRecipient(Events::MOUSE_BUTTON, this);

	m_points.reserve(_uiHeight * _uiWidth);
	m_links.reserve((_uiHeight * _uiWidth * 2) - _uiHeight - _uiWidth);
	m_models.reserve((_uiHeight * _uiWidth * 2) - _uiHeight - _uiWidth);

	for (unsigned int uiColumn = 0; uiColumn < _uiWidth; uiColumn++)
	{
		for (unsigned int uiRow = 0; uiRow < _uiHeight; uiRow++)
		{
			TPoint point;
			if (uiRow == 0)
			{
				point.dynamic = false;
			}
			else
			{
				point.dynamic = true;
			}
			point.position = TOP_LEFT + Vector2(uiColumn * RESTING_DISTANCE, uiRow * RESTING_DISTANCE);
			point.previousPosition = point.position;
			m_points.push_back(point);

			if (uiColumn > 0)
			{
				TLink link;
				link.pointA = &m_points[uiColumn * _uiHeight + uiRow];
				link.pointB = &m_points[(uiColumn - 1) * _uiHeight + uiRow];
				m_links.push_back(link);

				m_models.push_back(new Line2(link.pointA->position, link.pointB->position));
			}

			if (uiRow > 0)
			{
				TLink link;
				link.pointA = &m_points[uiColumn * _uiHeight + uiRow];
				link.pointB = &m_points[uiColumn * _uiHeight + uiRow - 1];
				m_links.push_back(link);

				m_models.push_back(new Line2(link.pointA->position, link.pointB->position));
			}
		}
	}
}

CCloth::~CCloth()
{
	Messages::deregisterRecipient(Events::MOUSE_BUTTON, this);
}

void CCloth::AddModelsToGazEngine()
{
	Entity* pModels = new Entity;

	for (unsigned int uiIndex = 0; uiIndex < m_models.size(); uiIndex++)
	{
		pModels->addComponent(m_models[uiIndex]);
	}

	GazEngine::addEntity(pModels);
}

void CCloth::receive(unsigned short subject, const void* message)
{
	const MouseButtonEvent* pEvent = static_cast<const MouseButtonEvent*>(message);

	if (pEvent->buttonState != Button::UP)
	{
		return;
	}

	Vector2 mousePosition(pEvent->x, pEvent->y);

	if (pEvent->button == Mouse::LEFT)
	{
		for (unsigned int uiIndex = 0; uiIndex < m_points.size(); uiIndex++)
		{
			TPoint& rPoint = m_points[uiIndex];
			if (!rPoint.dynamic)
			{
				continue;
			}

			Vector2 toMouseClick = mousePosition - rPoint.position;
			toMouseClick *= 0.01f;
			rPoint.position += toMouseClick;
		}
	}
	else if (pEvent->button == Mouse::RIGHT)
	{
		vector<TPoint>::iterator pointIter = m_points.begin();
		while (pointIter != m_points.end())
		{
			if (pointIter->dynamic)
			{
				if ((pointIter->position - mousePosition).length() < 25.0f)
				{
					vector<TLink>::iterator linkIter = m_links.begin();
					vector<Line2*>::iterator modelIter = m_models.begin();
					while (linkIter != m_links.end())
					{
						if (linkIter->pointA == &*pointIter || linkIter->pointB == &*pointIter)
						{
							(*modelIter)->setVisible(false);

							// Hrmmm... this will be slow with a vector...
							linkIter = m_links.erase(linkIter);
							modelIter = m_models.erase(modelIter);
							continue;
						}

						linkIter++;
						modelIter++;
					}

					// Hrmmm... this will be slow with a vector...
					pointIter = m_points.erase(pointIter);
					continue;
				}
			}

			pointIter++;
		}
	}
}

void CCloth::SatisfyConstraints()
{
	for (unsigned int uiIndex = 0; uiIndex < m_links.size(); uiIndex++)
	{
		TLink& rLink = m_links[uiIndex];

		Vector2 aToB = rLink.pointB->position - rLink.pointA->position;
		float fAToBLength = aToB.length();
		float fDifference = (RESTING_DISTANCE - fAToBLength) / fAToBLength;

		Vector2 adjustment = aToB * 0.5 * STIFFNESS * fDifference;

		if (rLink.pointA->dynamic)
		{
			rLink.pointA->position -= adjustment;
		}

		if (rLink.pointB->dynamic)
		{
			rLink.pointB->position += adjustment;
		}
	}
}

void CCloth::UpdateModels()
{
	for (unsigned int uiIndex = 0; uiIndex < m_links.size(); uiIndex++)
	{
		TLink& rLink = m_links[uiIndex];
		Line2* pLine = m_models[uiIndex];

		pLine->setPointA(rLink.pointA->position);
		pLine->setPointB(rLink.pointB->position);
	}
}

void CCloth::VerletIntegration()
{
	Vector2 accelDueToGravity(0.0f, 10.0f * MASS);
	Vector2 aDtSq = accelDueToGravity * pow(GazEngine::getDeltaTime(), 2.0f);

	for (unsigned int uiIndex = 0; uiIndex < m_points.size(); uiIndex++)
	{
		TPoint& rPoint = m_points[uiIndex];
		if (!rPoint.dynamic)
		{
			continue;
		}

		//Vector2 velocity = rPoint.position - rPoint.previousPosition;
		//Vector2 newPosition = rPoint.position + velocity + aDtSq;
		Vector2 newPosition = rPoint.position * (1 + DRAG) - rPoint.previousPosition * DRAG + aDtSq;

		rPoint.previousPosition = rPoint.position;
		rPoint.position = newPosition;
	}
}
