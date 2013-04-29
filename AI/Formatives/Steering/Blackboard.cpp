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
// $Date: 2012-09-24 08:28:44 +1200 (Mon, 24 Sep 2012) $
// $Rev: 68 $
//
#include "Blackboard.h"

#include "GDE/Input/InputDevice.h"
#include "GDE/Input/InputChangeSubject.h"
#include "GDE/Input/InputStates.h"

CBlackboard::CBlackboard() :
	m_vMousePosition(), m_repulsionZones()
{
}

void CBlackboard::AddCat(const CCat& _krCat)
{
	m_cats.push_back(&_krCat);
}

void CBlackboard::AddRepulsionZone(const CCircle& _krRepulsionZone)
{
	m_repulsionZones.push_back(_krRepulsionZone);
}

void CBlackboard::AddWall(const CWall& _krWall)
{
	m_walls.push_back(_krWall);
}

const std::vector<const CCat*>& CBlackboard::GetCats() const
{
	return m_cats;
}

FLOAT32 CBlackboard::GetClosestWallIntersectionTime(const CFVec2& _krvWhiskerStart, const CFVec2& _krvWhisker) const
{
	FLOAT32 fTime = -1.0f;
	FLOAT32 fTimeCurrent = 0.0f;

	for (unsigned int uiIndex = 0; uiIndex < m_walls.size(); uiIndex++)
	{
		if (m_walls.at(uiIndex).GetLineIntersectionTime(_krvWhiskerStart, _krvWhisker, fTimeCurrent))
		{
			// If the intersetion is within the segment given.
			if (0.0f <= fTimeCurrent && fTimeCurrent <= 1.0f)
			{
				// If this is the closest intersection yet.
				if (fTime == -1.0f || fTimeCurrent < fTime)
				{
					fTime = fTimeCurrent;
				}
			}
		}
	}

	return fTime;
}

CBlackboard& CBlackboard::GetInstance()
{
	static CBlackboard instance;
	return instance;
}

const CFVec2& CBlackboard::GetMousePosition() const
{
	return m_vMousePosition;
}

const std::vector<CCircle>& CBlackboard::GetRepulsionZones() const
{
	return m_repulsionZones;
}

void CBlackboard::NotifyChange(GDE::ISubject* _pSubject, GDE::UINT32 _uiMessage)
{
	const GDE::CInputDevice& Device = reinterpret_cast<GDE::IInputChangeSubject*>(_pSubject)->GetDevice();	
	const GDE::SInputState& State = Device.GetState(_uiMessage);
	m_vMousePosition.X(State.m_fValue);
	m_vMousePosition.Y(State.m_fValue2);
}
