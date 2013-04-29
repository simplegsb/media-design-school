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
#pragma once

#include <gazengine/physics/Body.h>
#include <gazengine/physics/Constraint.h>

class CFloorConstraint : public Constraint
{
	public:
		CFloorConstraint(float _fAltitude);

		void AddBody(Body* _pBody);

		void apply();

	private:
		std::vector<Body*> m_bodies;

		float m_fAltitude;
};
