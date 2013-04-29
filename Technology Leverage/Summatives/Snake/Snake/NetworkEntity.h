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

#include <NetworkIDObject.h>

#include <gazengine/Entity.h>

class CNetworkEntity : public Entity, public RakNet::NetworkIDObject
{
	public:
		CNetworkEntity(unsigned short _usCategory = UNCATEGORIZED, const std::string& _krName = std::string());
};
