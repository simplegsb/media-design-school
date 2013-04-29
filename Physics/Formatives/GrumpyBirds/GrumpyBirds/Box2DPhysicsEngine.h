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
#include <gazengine/math/Vector2.h>

#include "Box2DBody.h"
#include "Box2DGDIDrawer.h"

class CBox2DPhysicsEngine : public Engine
{
	public:
		CBox2DPhysicsEngine(const Vector2& _krGravity, CBox2DGDIDrawer* _pDrawer = NULL);

		void advance();

		void destroy();

		void addEntity(Entity* entity);

		void init();

		void removeEntity(const Entity& entity);

	private:
		CBox2DGDIDrawer* m_pDrawer;

		b2World m_world;
};
