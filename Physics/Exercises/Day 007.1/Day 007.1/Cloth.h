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

#include <vector>

#include <gazengine/Messages.h>
#include <gazengine/model/Line2.h>

#include "Cloth.h"
#include "Link.h"
#include "Point.h"

class CCloth : public Messages::Recipient
{
	public:
		CCloth(unsigned int _uiHeight, unsigned int _uiWidth);

		~CCloth();

		void AddModelsToGazEngine();

		void receive(unsigned short subject, const void* message);

		void SatisfyConstraints();

		void UpdateModels();

		void VerletIntegration();

	private:
		std::vector<TLink> m_links;

		std::vector<Line2*> m_models;

		std::vector<TPoint> m_points;
};
