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

#include <map>

#include <gazengine/engine/Engine.h>
#include <gazengine/gdi/model/GDIMesh.h>
#include <gazengine/Messages.h>

class CSpringyUIEngine : public Engine, public Messages::Recipient
{
	public:
		CSpringyUIEngine();

		void addEntity(Entity* entity);

		void advance();

		void destroy();

		void init();

		void receive(unsigned short subject, const void* message);

		void removeEntity(const Entity& entity);

		void SelectButton(unsigned int _uiIndex);

	private:
		unsigned int m_uiButtonIndex;

		std::vector<GDIMesh*> m_menuItems;

		std::map<GDIMesh*, std::vector<Vector2> > m_previousPositions;
};
