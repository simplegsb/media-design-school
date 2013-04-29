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

#include <gazengine/Component.h>
#include <gazengine/input/Keyboard.h>
#include <gazengine/Messages.h>

class CSnakeCharmer : public Component, public Messages::Recipient
{
	public:
		typedef enum EDirection
		{
			DOWN,
			LEFT,
			RIGHT,
			UP
		} EDirection;

		CSnakeCharmer(float _fSpeed);

		~CSnakeCharmer();

		void charm();

		EDirection getDirection();

		void receive(unsigned short _usSubject, const void* _kpMessage);

		void setDirection(EDirection _direction);

		void speedUp();

		void updateDirection();

	private:
		Keyboard::Button m_button;

		EDirection m_direction;

		float m_fMoveDelta;

		float m_fSpeed;

		EDirection m_tempDirection;
};
