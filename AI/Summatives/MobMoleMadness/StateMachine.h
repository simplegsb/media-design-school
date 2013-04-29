//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// Copyright (c) 2012 Media Design School
//
// Author: Gary Buyn
//
// SVN Info
// $Author: MEDIADESIGN\gary.buyn $
// $Date: 2013-01-23 09:40:30 +1300 (Wed, 23 Jan 2013) $
// $Rev: 1108 $
//
#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "Common.h"

// Must be included after Common.h or errors will ensue...
#include <stack>
#include <string>
#include <vector>

class CStateMachine
{
	public:
		enum EAction
		{
			POP_ACTION,
			POP_AND_PUSH_ACTION,
			PUSH_ACTION
		};

		enum EState
		{
			ANY_STATE,
			ATTACK_STATE,
			EXPLORE_STATE,
			PATROL_STATE,
			REST_STATE,
			SLEEP_STATE
		};

		struct TTransition
		{
			EAction action;

			std::string event;

			EState newState;

			EState oldState;
		};

		CStateMachine();

		void AddTransition(TTransition transition);

		std::string GetLastEvent() const;

		EState GetState() const;

		void Update(const std::string& _krsEvent);

	private:
		std::string m_sLastEvent;

		std::stack<EState> m_stack;

		std::vector<TTransition> m_transitions;
};

#endif /* EDGE_H_ */
