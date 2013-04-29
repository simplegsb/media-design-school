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
// $Date: 2013-01-29 11:58:25 +1300 (Tue, 29 Jan 2013) $
// $Rev: 1129 $
//
#pragma once

#include <Box2D/Box2D.h>

#include <gazengine/Events.h>

namespace Box2DEvents
{
	const unsigned short BEGIN_CONTACT = Events::USER_ID_0;

	const unsigned short END_CONTACT = Events::USER_ID_0 + 1;

	const unsigned short POST_SOLVE = Events::USER_ID_0 + 2;

	const unsigned short PRE_SOLVE = Events::USER_ID_0 + 3;
}

struct TBox2DEvent
{
	const b2Contact* contact;

	const b2ContactImpulse* impulse;

	const b2Manifold* oldManifold;
};
