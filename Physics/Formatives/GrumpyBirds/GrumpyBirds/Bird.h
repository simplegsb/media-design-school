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

#include <gazengine/Component.h>

class CBird : public Component
{
	public:
		virtual ~CBird()
		{
		}

		virtual bool Fly() = 0;
};
