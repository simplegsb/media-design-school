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
// $Date: 2013-01-23 09:45:25 +1300 (Wed, 23 Jan 2013) $
// $Rev: 1110 $
//
#pragma once

#include <gazengine/Events.h>

namespace SnakeClientEvents
{
	const unsigned short NOT_CONNECTED = Events::USER_ID_0;

	const unsigned short CONNECTED = Events::USER_ID_0 + 1;

	const unsigned short DEATH = Events::USER_ID_0 + 2;

	const unsigned short DISCONNECTED = Events::USER_ID_0 + 3;

	const unsigned short GAME_STARTED = Events::USER_ID_0 + 4;

	const unsigned short JOIN = Events::USER_ID_0 + 5;

	const unsigned short LEAVE = Events::USER_ID_0 + 6;

	const unsigned short PLAYER_NAME = Events::USER_ID_0 + 7;

	const unsigned short SERVER_DETAILS = Events::USER_ID_0 + 8;
}

namespace SnakeServerEvents
{
	const unsigned short DEATH = Events::USER_ID_0 + 9;

	const unsigned short GAME_START = Events::USER_ID_0 + 10;
}
