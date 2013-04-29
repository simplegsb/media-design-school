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

#include <gazengine/Messages.h>

#include "Bird.h"

class CBoostBird : public CBird, public Messages::Recipient
{
	public:
		CBoostBird();

		bool Fly();

		void receive(unsigned short subject, const void* message);

	private:
		bool m_bFlying;

		bool m_bBoosting;
};
