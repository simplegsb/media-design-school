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

#include <gazengine/engine/Engine.h>

#include "Cloth.h"

class CClothEngine : public Engine
{
	public:
		CClothEngine(CCloth* _pCloth);

		~CClothEngine();

		void addEntity(Entity* entity);

		void advance();

		void destroy();

		void init();

		void removeEntity(const Entity& entity);

	private:
		CCloth* m_pCloth;
};
