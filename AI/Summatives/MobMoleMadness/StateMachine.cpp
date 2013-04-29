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
#include "Events.h"
#include "StateMachine.h"

using namespace std;

CStateMachine::CStateMachine() :
	m_sLastEvent(),
	m_stack(),
	m_transitions()
{
	m_stack.push(EXPLORE_STATE);
}

void CStateMachine::AddTransition(TTransition transition)
{
	m_transitions.push_back(transition);
}

string CStateMachine::GetLastEvent() const
{
	return m_sLastEvent;
}

CStateMachine::EState CStateMachine::GetState() const
{
	return m_stack.top();
}

void CStateMachine::Update(const string& _krsEvent)
{
	for (UINT32 uiIndex = 0; uiIndex < m_transitions.size(); uiIndex++)
	{ 
		TTransition& transition = m_transitions.at(uiIndex);

		if (transition.oldState == GetState() || transition.oldState == ANY_STATE)
		{ 
			if (transition.event == _krsEvent || transition.event == ANY_EVENT)
			{
				m_sLastEvent = _krsEvent;

				if (transition.action == POP_ACTION)
				{
					m_stack.pop();
				}
				else if (transition.action == POP_AND_PUSH_ACTION)
				{
					m_stack.pop();
					m_stack.push(transition.newState);
				}
				else if (transition.action == PUSH_ACTION)
				{
					m_stack.push(transition.newState);
				}

				break; 
			}
		} 
	} 
}
