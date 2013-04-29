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
#include "Common.h"

#include "Cat.h"
#include "Circle.h"
#include "Drawing.h"
#include "SteeringContext.h"

// Must be included after Common.h or errors will ensue...
#include <algorithm>

CCat::CCat() :
	m_pSteeringAgent(NULL)
{
	m_pSteeringAgent = new CSteeringAgent(*this);
}

CCat::~CCat()
{
	delete m_pSteeringAgent;
}

void CCat::Draw()
{
	Drawing::DrawCat(*this);
	Drawing::DrawSteeringAgent(*m_pSteeringAgent);
}

void CCat::GetBoundingCircle(CCircle& _rBounds)
{
	_rBounds.Initialise(m_vPosition, m_fRadius);
}

float CCat::GetHeadingRadians() const
{
	return m_fHeadingRadians;
}

const CFVec2& CCat::GetHeadingVector() const
{
	return m_vHeadingVector;
}

const CFVec2& CCat::GetPosition() const
{
	return m_vPosition;
}

float CCat::GetRadius() const
{
	return m_fRadius;
}

const CSteeringAgent* CCat::GetSteeringAgent() const
{
	return m_pSteeringAgent;
}

void CCat::Initialise(CFVec2Arg _rvPosition, FLOAT32 _fHeadingDegrees)
{
	m_vPosition = _rvPosition;
	m_fHeadingRadians = fDegsToRad(_fHeadingDegrees);
	m_fRadius = 22.0f;
	m_bIntersected = false;
}

bool CCat::Intersects(const CCircle& _krCircle) const
{
	CCircle bounds;
	bounds.Initialise(m_vPosition, m_fRadius);

	return _krCircle.Overlaps(bounds);
}

bool CCat::IsIntersectedByCat() const
{
	return m_bIntersected;
}

void CCat::ResetIntersection()
{
	m_bIntersected = false;
}

void CCat::SetIntersectedByCat()
{
	m_bIntersected = true;
}

void CCat::Update(FLOAT32 _fTimeDelta)
{
	m_pSteeringAgent->Update(_fTimeDelta);

	// Turn.
	m_fHeadingRadians += m_pSteeringAgent->GetRotationalSpeed() * _fTimeDelta;
	m_vHeadingVector.SetToRotationZ(m_fHeadingRadians);

	// Update position.
	CFVec2 vVelocity = m_vHeadingVector * m_pSteeringAgent->GetSpeed() * _fTimeDelta;
	m_vPosition += vVelocity;
}
