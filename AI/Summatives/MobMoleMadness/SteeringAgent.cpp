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
#include "GDE/Renderer/Renderer.h"
#include "GDE/Renderer/Vertex.h"

#include "Common.h"

#include "Blackboard.h"
#include "Math.h"
#include "MMMContext.h"
#include "Mole.h"
#include "SteeringAgent.h"

using namespace std;

CSteeringAgent::CSteeringAgent(const CMole& _krMole) :
	m_krMole(_krMole)
{
	// Bearing.
	m_vBearing = fv2EMPTY;

	// Repulsion.
	m_fRepulsionBufferDynamic = 50.0f;
	m_fRepulsionBufferStatic = 30.0f;

	// Speed and acceleration.
	m_fAcceleration = 100.0f; // Changes linearly
	m_fDeceleration = m_fAcceleration * 5.0f; // Changes linearly
	m_fRotationalAcceleration = 50.0f; // Changes linearly
	m_fRotationalDeceleration = 50.0f; // Changes linearly
	m_fRotationalSpeed = 0.0f;
	m_fSpeed = 0.0f;
	m_fSpeedMax = 150.0f;
	m_fSteeringLock = 5.0f;

	// Stuck state.
	m_bStuck = false;
	m_fStuckCheckDelta = 0.0f;
	m_fStuckRotation = 0.0f;
	m_fStuckTimeThreshold = 0.1f;
	m_vStuckCheckPosition = m_krMole.GetPosition();

	// Whiskers.
	m_vWhiskerFront = SFVec2(0.0f, -1.0f);
	m_vWhiskersLeft[0] = SFVec2(0.0f, -50.0f);
	m_vWhiskersLeft[0].RotateZ(fDegsToRad(-35.0f));
	m_vWhiskersLeft[1] = SFVec2(0.0f, -40.0f);
	m_vWhiskersLeft[1].RotateZ(fDegsToRad(-80.0f));
	m_vWhiskersRight[0] = SFVec2(0.0f, -50.0f);
	m_vWhiskersRight[0].RotateZ(fDegsToRad(35.0f));
	m_vWhiskersRight[1] = SFVec2(0.0f, -40.0f);
	m_vWhiskersRight[1].RotateZ(fDegsToRad(80.0f));
}

void CSteeringAgent::ComputeBearing()
{
	m_vBearing = m_vDestination - m_krMole.GetPosition();

	if (m_bStuck)
	{
		m_vBearing.RotateZ(m_fStuckRotation);
	}

	m_vBearing.Normalise();

	// Multiplied by two so that they are more urgent.
	m_vBearing += GetRepulsionBearingEffect() * 2.0f;
	m_vBearing += GetWhiskerBearingEffect() * 2.0f;

	m_vBearing.Normalise();
}

void CSteeringAgent::ComputeSpeeds(FLOAT32 _fTimeDelta)
{
	FLOAT32 fTargetRotationalSpeed = ComputeTargetRotationalSpeed();

	CFVec2 vHeading;
	vHeading.SetToRotationZ(m_krMole.GetHeading());
	if (vHeading.CrossProduct(m_vBearing) < 0.0f)
	{
		fTargetRotationalSpeed *= -1.0f;
	}

	FLOAT32 fRotationalAccelerationDelta = m_fRotationalAcceleration * _fTimeDelta;
	FLOAT32 fRotationalDecelerationDelta = m_fRotationalDeceleration * _fTimeDelta;
	FLOAT32 fRotationalSpeedDifference = abs(fTargetRotationalSpeed - m_fRotationalSpeed);

	if (fTargetRotationalSpeed < m_fRotationalSpeed)
	{
		if (m_fRotationalSpeed > 0.0f)
		{
			m_fRotationalSpeed -= min(fRotationalAccelerationDelta, fRotationalSpeedDifference);
		}
		else
		{
			m_fRotationalSpeed -= min(fRotationalDecelerationDelta, fRotationalSpeedDifference);
		}
	}
	else if (fTargetRotationalSpeed > m_fRotationalSpeed)
	{
		if (m_fRotationalSpeed < 0.0f)
		{
			m_fRotationalSpeed += min(fRotationalDecelerationDelta, fRotationalSpeedDifference);
		}
		else
		{
			m_fRotationalSpeed += min(fRotationalAccelerationDelta, fRotationalSpeedDifference);
		}
	}

	FLOAT32 fTargetSpeed = ComputeTargetSpeed();
	FLOAT32 fAccelerationDelta = m_fAcceleration * _fTimeDelta;
	FLOAT32 fDecelerationDelta = m_fDeceleration * _fTimeDelta;
	FLOAT32 fSpeedDifference = abs(fTargetSpeed - m_fSpeed);

	if (fTargetSpeed > m_fSpeed)
	{
		m_fSpeed += min(fSpeedDifference, fAccelerationDelta);
	}
	else if (fTargetSpeed < m_fSpeed)
	{
		m_fSpeed -= min(fSpeedDifference, fDecelerationDelta);
	}
}

FLOAT32 CSteeringAgent::ComputeTargetRotationalSpeed()
{
	// A scalar in the range 0..1 where a higher number denotes a lower angle to the bearing.
	CFVec2 vHeading;
	vHeading.SetToRotationZ(m_krMole.GetHeading());
	FLOAT32 fUnitScalarFromBearing = (cos(vHeading.AngleTo(m_vBearing)) + 1.0f) / 2.0f;

	FLOAT32 fTargetRotationalSpeed = m_fSteeringLock;
	// 0..1, 0 = Facing away from the bearing, 1 = Facing the bearing.
	fTargetRotationalSpeed *= 1.0f - fUnitScalarFromBearing;

	return fTargetRotationalSpeed;
}

FLOAT32 CSteeringAgent::ComputeTargetSpeed()
{
	// A scalar in the range 0..1 where a higher number denotes a higher rotational speed.
	FLOAT32 fUnitScalarFromRotation = (abs(m_fRotationalSpeed) / m_fSteeringLock);

	FLOAT32 fTargetSpeed = m_fSpeedMax;
	// 0..1, 0 = Collision!, 1 = Whiskers have not intersected.
	fTargetSpeed *= GetWhiskerSpeedEffect();
	// 0..1, 0 = Collision!, 1 = No repulsion zones nearby.
	fTargetSpeed *= GetRepulsionSpeedEffect();
	// 0..1, 0 = Rotating at 'steering lock', 1 = No rotation.
	fTargetSpeed *= 1.0f - fUnitScalarFromRotation;

	return fTargetSpeed;
}

void CSteeringAgent::Draw() const
{
	DrawSteering();
	DrawTargeting();
	DrawWhiskers();
}

void CSteeringAgent::DrawSteering() const
{
	UINT32 uiColour = 0xFF00FF00; // green

	CFVec2 vFront = m_krMole.GetPosition();
	CFVec2 vHeading;
	vHeading.SetToRotationZ(m_krMole.GetHeading());
	vFront += vHeading * 22.0f;
	vHeading *= m_fSpeed;

	CFVec2 vRotation = vHeading;
	vRotation.RotateZ(m_fRotationalSpeed / 10.0f);

	GDE::SVertex vertices[3];
	vertices[0].m_fX = vFront.X();
	vertices[0].m_fY = vFront.Y();
	vertices[0].m_fZ = 0.0f;
	vertices[0].m_uColour = uiColour;
	vertices[1].m_fX = vFront.X() + vHeading.X();
	vertices[1].m_fY = vFront.Y() + vHeading.Y();
	vertices[1].m_fZ = 0.0f;
	vertices[1].m_uColour = uiColour;
	vertices[2].m_fX = vFront.X() + vRotation.X();
	vertices[2].m_fY = vFront.Y() + vRotation.Y();
	vertices[2].m_fZ = 0.0f;
	vertices[2].m_uColour = uiColour;

	CMMMContext::GetInstance().GetRenderer()->DrawPrimitives(GDE::PT_TriangleList, 1, vertices);
}

void CSteeringAgent::DrawTargeting() const
{
	UINT32 uiTargetColour = 0xFF00FF00; // green
	UINT32 uiBearingColour = 0xFFFF8800; // orange

	CCircle targetCircle;
	targetCircle.Initialise(m_vDestination, 5.0f);
	CMMMContext::GetInstance().DrawCircle(targetCircle, uiTargetColour);

	CMMMContext::GetInstance().DrawLine(m_krMole.GetPosition(),
		m_vDestination, uiTargetColour);
	CMMMContext::GetInstance().DrawLine(m_krMole.GetPosition(),
		m_krMole.GetPosition() + m_vBearing * fMOLE_RADIUS, uiBearingColour);
}

void CSteeringAgent::DrawWhisker(const CFVec2& _rvWhisker) const
{
	CFVec2 vAdjustedWhisker = GetAdjustedWhisker(_rvWhisker);
	CFVec2 vAbsoluteWhiskerStart = GetAbsoluteWhiskerStart(_rvWhisker);
	CFVec2 vAbsoluteWhiskerEnd = vAbsoluteWhiskerStart + vAdjustedWhisker;

	UINT32 uiColour = 0xFF00FF00; // green

	float fWhiskerIntersectionTime =
		CBlackboard::GetInstance().GetClosestWallIntersectionTime(vAbsoluteWhiskerStart, vAdjustedWhisker);
	if (fWhiskerIntersectionTime != -1.0f)
	{
		uiColour = 0xFFFF0000; // red
	}

	CMMMContext::GetInstance().DrawLine(vAbsoluteWhiskerStart, vAbsoluteWhiskerEnd, uiColour);
}

void CSteeringAgent::DrawWhiskers() const
{
	CFVec2 vScaledWhiskerFront = m_vWhiskerFront * m_fSpeed;
	DrawWhisker(vScaledWhiskerFront);

	for (unsigned int uiIndex = 0 ; uiIndex < SIDE_WHISKER_COUNT; uiIndex++)
	{
		DrawWhisker(m_vWhiskersLeft[uiIndex]);
	}

	for (unsigned int uiIndex = 0 ; uiIndex < SIDE_WHISKER_COUNT; uiIndex++)
	{
		DrawWhisker(m_vWhiskersRight[uiIndex]);
	}
}

const CFVec2& CSteeringAgent::GetDestination() const
{
	return m_vDestination;
}

CFVec2 CSteeringAgent::GetAbsoluteWhiskerStart(const CFVec2& _krvRelativeWhisker) const
{
	CFVec2 vAbsoluteWhiskerStart = GetAdjustedWhisker(_krvRelativeWhisker);

	if (vAbsoluteWhiskerStart.Magnitude() != 0.0f)
	{
		vAbsoluteWhiskerStart.Normalise();
		vAbsoluteWhiskerStart *= fMOLE_RADIUS;
	}

	vAbsoluteWhiskerStart += m_krMole.GetPosition();

	return vAbsoluteWhiskerStart;
}

CFVec2 CSteeringAgent::GetAdjustedWhisker(const CFVec2& _krvWhisker) const
{
	CFVec2 vAdjustedWhisker = _krvWhisker;
	vAdjustedWhisker.RotateZ(m_krMole.GetHeading());

	return vAdjustedWhisker;
}

FLOAT32 CSteeringAgent::GetRotationalSpeed() const
{
	return m_fRotationalSpeed;
}

FLOAT32 CSteeringAgent::GetSpeed() const
{
	return m_fSpeed;
}

CFVec2 CSteeringAgent::GetRepulsionBearingEffect() const
{
	CFVec2 vCumulativeRepulsionEffect = fv2EMPTY;
	CFVec2 vHeading;
	vHeading.SetToRotationZ(m_krMole.GetHeading());

	const vector<CCircle>& krRepulsionZones = CBlackboard::GetInstance().GetRepulsionZones();
	for (unsigned int uiIndex = 0; uiIndex < krRepulsionZones.size(); uiIndex++)
	{
		CFVec2 vRepulsionEffect = m_krMole.GetPosition() - krRepulsionZones.at(uiIndex).Centre();
		FLOAT32 fDistanceBetweenCenters = vRepulsionEffect.Magnitude();

		FLOAT32 fDistanceBetweenBorders = fDistanceBetweenCenters - fMOLE_RADIUS -
			krRepulsionZones.at(uiIndex).Radius();

		if (vHeading.DotProduct(vRepulsionEffect) < 0.0f &&
			fDistanceBetweenBorders < m_fRepulsionBufferStatic)
		{
			FLOAT32 fDistanceIntoRepulsionBuffer = (m_fRepulsionBufferStatic - fDistanceBetweenBorders) /
				m_fRepulsionBufferStatic;
			vRepulsionEffect.Normalise();
			vRepulsionEffect *= fDistanceIntoRepulsionBuffer;
			vCumulativeRepulsionEffect += vRepulsionEffect;
		}
	}

	const vector<const CMole*>& krMoles = CBlackboard::GetInstance().GetMoles();
	for (unsigned int uiIndex = 0; uiIndex < krMoles.size(); uiIndex++)
	{
		CFVec2 vRepulsionEffect = m_krMole.GetPosition() - krMoles.at(uiIndex)->GetPosition();
		FLOAT32 fDistanceBetweenCenters = vRepulsionEffect.Magnitude();

		// We don't want the cat to repel itself...
		if (fDistanceBetweenCenters == 0.0f)
		{
			continue;
		}

		FLOAT32 fDistanceBetweenBorders = fDistanceBetweenCenters - fMOLE_RADIUS * 2.0f;

		if (vHeading.DotProduct(vRepulsionEffect) < 0.0f &&
			fDistanceBetweenBorders < m_fRepulsionBufferDynamic)
		{
			FLOAT32 fDistanceIntoRepulsionBuffer = (m_fRepulsionBufferDynamic - fDistanceBetweenBorders) /
				m_fRepulsionBufferDynamic;
			vRepulsionEffect.Normalise();
			vRepulsionEffect *= fDistanceIntoRepulsionBuffer;
			vCumulativeRepulsionEffect += vRepulsionEffect;
		}
	}

	if (vCumulativeRepulsionEffect.Magnitude() > 1.0f)
	{
		vCumulativeRepulsionEffect.Normalise();
	}

	return vCumulativeRepulsionEffect;
}

FLOAT32 CSteeringAgent::GetRepulsionSpeedEffect() const
{
	FLOAT32 fCumulativeRepulsionEffect = 1.0f;
	CFVec2 vHeading;
	vHeading.SetToRotationZ(m_krMole.GetHeading());

	const vector<CCircle>& krRepulsionZones = CBlackboard::GetInstance().GetRepulsionZones();
	for (unsigned int uiIndex = 0; uiIndex < krRepulsionZones.size(); uiIndex++)
	{
		CFVec2 vRepulsionEffect = m_krMole.GetPosition() - krRepulsionZones.at(uiIndex).Centre();
		FLOAT32 fDistanceBetweenCenters = vRepulsionEffect.Magnitude();

		FLOAT32 fDistanceBetweenBorders = fDistanceBetweenCenters - fMOLE_RADIUS -
			krRepulsionZones.at(uiIndex).Radius();

		if (vHeading.DotProduct(vRepulsionEffect) < 0.0f &&
			fDistanceBetweenBorders < m_fRepulsionBufferStatic)
		{
			fCumulativeRepulsionEffect -= 1.0f - (fDistanceBetweenBorders / m_fRepulsionBufferStatic);
		}
	}

	const vector<const CMole*>& krMoles = CBlackboard::GetInstance().GetMoles();
	for (unsigned int uiIndex = 0; uiIndex < krMoles.size(); uiIndex++)
	{
		CFVec2 vRepulsionEffect = m_krMole.GetPosition() - krMoles.at(uiIndex)->GetPosition();
		FLOAT32 fDistanceBetweenCenters = vRepulsionEffect.Magnitude();

		// We don't want the cat to repel itself...
		if (fDistanceBetweenCenters == 0.0f)
		{
			continue;
		}

		FLOAT32 fDistanceBetweenBorders = fDistanceBetweenCenters - fMOLE_RADIUS * 2.0f;

		if (vHeading.DotProduct(vRepulsionEffect) < 0.0f &&
			fDistanceBetweenBorders < m_fRepulsionBufferDynamic)
		{
			fCumulativeRepulsionEffect -= 1.0f - (fDistanceBetweenBorders / m_fRepulsionBufferDynamic);
		}
	}

	if (fCumulativeRepulsionEffect < 0.0f)
	{
		fCumulativeRepulsionEffect = 0.0f;
	}

	return fCumulativeRepulsionEffect;
}

CFVec2 CSteeringAgent::GetWhiskerBearingEffect() const
{
	CBlackboard& rBlackboard = CBlackboard::GetInstance();
	bool bRotationApplied = false;
	FLOAT32 fTotalRotation = 0.0f;

	FLOAT32 fRotation = Math::PI / 2.0f; // 90 degrees
	for (unsigned int uiIndex = 0 ; uiIndex < SIDE_WHISKER_COUNT; uiIndex++)
	{
		FLOAT32 fWhiskerTime =
			rBlackboard.GetClosestWallIntersectionTime(GetAbsoluteWhiskerStart(m_vWhiskersLeft[uiIndex]),
			GetAdjustedWhisker(m_vWhiskersLeft[uiIndex]));
		if (fWhiskerTime != -1.0f)
		{
			fTotalRotation += fRotation;
			bRotationApplied = true;
		}

		fWhiskerTime =
			rBlackboard.GetClosestWallIntersectionTime(GetAbsoluteWhiskerStart(m_vWhiskersRight[uiIndex]),
			GetAdjustedWhisker(m_vWhiskersRight[uiIndex]));
		if (fWhiskerTime != -1.0f)
		{
			fTotalRotation -= fRotation;
			bRotationApplied = true;
		}

		fRotation /= 5.0f; // TODO MAGIC
	}

	if (bRotationApplied)
	{
		// If the rotation was cancelled out (accounting for rounding errors).
		if (abs(fTotalRotation) < 0.0001f)
		{
			// The whiskers are intersecting on both sides so chances are we're heading straight for something and we
			// need to turn.
			fTotalRotation += Math::PI / 2.0f; // 90 degrees

			// If we're already turning, continue in that direction.
			if (m_fRotationalSpeed < 0.0f)
			{
				fTotalRotation *= -1.0f;
			}
		}

		CFVec2 vWhiskerBearingEffect;
		vWhiskerBearingEffect.SetToRotationZ(m_krMole.GetHeading() + fTotalRotation);

		return vWhiskerBearingEffect;
	}

	return fv2EMPTY;
}

float CSteeringAgent::GetWhiskerSpeedEffect() const
{
	FLOAT32 vWhiskerSpeedEffect = 1.0f;
	CBlackboard& rBlackboard = CBlackboard::GetInstance();
	CFVec2 vScaledWhiskerFront = m_vWhiskerFront * m_fSpeed;

	FLOAT32 fWhiskerTimeFront = rBlackboard.GetClosestWallIntersectionTime(GetAbsoluteWhiskerStart(vScaledWhiskerFront),
		GetAdjustedWhisker(vScaledWhiskerFront));

	if (fWhiskerTimeFront != -1.0f)
	{
		vWhiskerSpeedEffect -= 1.0f - fWhiskerTimeFront;
	}

	FLOAT32 fWhiskerTimeLeft = rBlackboard.GetClosestWallIntersectionTime(GetAbsoluteWhiskerStart(m_vWhiskersLeft[0]),
		GetAdjustedWhisker(m_vWhiskersLeft[0]));

	if (fWhiskerTimeLeft != -1.0f)
	{
		vWhiskerSpeedEffect -= 1.0f - fWhiskerTimeLeft;
	}

	FLOAT32 fWhiskerTimeRight = rBlackboard.GetClosestWallIntersectionTime(GetAbsoluteWhiskerStart(m_vWhiskersRight[0]),
		GetAdjustedWhisker(m_vWhiskersRight[0]));

	if (fWhiskerTimeRight != -1.0f)
	{
		vWhiskerSpeedEffect -= 1.0f - fWhiskerTimeRight;
	}

	// We don't want to go backwards as a result of whiskers intersecting.
	vWhiskerSpeedEffect = max(vWhiskerSpeedEffect, 0.0f);

	return vWhiskerSpeedEffect;
}

bool CSteeringAgent::IsStuck() const
{
	return m_bStuck;
}

void CSteeringAgent::Rest(FLOAT32 _fTimeDelta)
{
	FLOAT32 fSpeed = m_fSpeed - m_fDeceleration * _fTimeDelta;

	ComputeBearing();
	ComputeSpeeds(_fTimeDelta);

	m_fSpeed = max(0.0f, fSpeed);
}

void CSteeringAgent::Run(FLOAT32 _fTimeDelta)
{
	m_fStuckCheckDelta += _fTimeDelta;

	if (m_fStuckCheckDelta > m_fStuckTimeThreshold && !m_bStuck)
	{
		m_fStuckCheckDelta = 0.0f;
		if ((m_krMole.GetPosition() - m_vStuckCheckPosition).Magnitude() < 1.0f)
		{
			m_bStuck = true;
			m_fStuckRotation += m_fSteeringLock * m_fStuckTimeThreshold;
		}

		m_vStuckCheckPosition = m_krMole.GetPosition();
	}
	else if (m_fStuckCheckDelta > m_fStuckTimeThreshold * 2.0f && m_bStuck)
	{
		m_fStuckCheckDelta = 0.0f;
		m_bStuck = false;
	}

	ComputeBearing();
	ComputeSpeeds(_fTimeDelta);
}

void CSteeringAgent::SetDestination(const CFVec2& _krvDestination)
{
	m_vDestination = _krvDestination;
}
