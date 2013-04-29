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
#include <MessageIdentifiers.h>

namespace MessageIdentifiers
{
	typedef enum EMessageIdentifiers
	{
		ID_SNAKE_DEATH = ID_USER_PACKET_ENUM + 1,

		ID_SNAKE_GAME_START = ID_USER_PACKET_ENUM + 2,

		ID_SNAKE_INPUT = ID_USER_PACKET_ENUM + 3,

		ID_SNAKE_JOIN = ID_USER_PACKET_ENUM + 4,

		ID_SNAKE_JOIN_CONFIRM = ID_USER_PACKET_ENUM + 5,

		ID_SNAKE_LEAVE = ID_USER_PACKET_ENUM + 6,

		ID_SNAKE_STATES = ID_USER_PACKET_ENUM + 7
	} EMessageIdentifiers;
}
