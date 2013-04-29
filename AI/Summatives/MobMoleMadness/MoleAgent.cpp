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
#include "GDE/Renderer/Font.h"

#include "Blackboard.h"
#include "Events.h"
#include "MMMContext.h"
#include "Mole.h"
#include "MoleAgent.h"

// Must be included after Common.h or errors will ensue...
#include <sstream>

using namespace std;

const FLOAT32 fAI_FREQUENCY = 5.0f;
const FLOAT32 fDISTURBANCE_LIFETIME = 5.0f;
const UINT32 fMAX_EXPLORE_DESTINATION_ATTEMPTS = 25;
const FLOAT32 fREST_FREQUENCY = 0.1f;
const FLOAT32 fREST_TIME = 5.0f;
const FLOAT32 fSTEERING_FREQUENCY = 30.0f;

CMoleAgent::CMoleAgent(CMole& _rMole, FLOAT32 fDisturbanceRange, FLOAT32 fSightRange) :
	m_fAiTimeDelta(0.0f),
	m_fDisturbanceRange(fDisturbanceRange),
	m_fDisturbanceTimeDelta(0.0f),
	m_fPatrolTimeDelta(0.0f),
	m_fRestTimeDelta(0.0f),
	m_fSightRange(fSightRange),
	m_fSteeringTimeDelta(0.0f),
	m_pAcorn(NULL),
	m_pathFindingAgent(_rMole),
	m_rMole(_rMole),
	m_sensorAgent(_rMole, m_fSightRange),
	m_stateMachine(),
	m_steeringAgent(_rMole)
{
	m_steeringAgent.SetDestination(m_rMole.GetDestination());

	Messages::registerRecipient(ACORN_FOUND_EVENT, this);
	Messages::registerRecipient(DESTINATION_REACHED_EVENT, this);
	Messages::registerRecipient(DISTURBANCE_EVENT, this);
	Messages::registerRecipient(MOLE_RESTED_EVENT, this);
	Messages::registerRecipient(MOLE_TIRED_EVENT, this);
	Messages::registerRecipient(SLEEP_EVENT, this);
	Messages::registerRecipient(TARGET_LOST_EVENT, this);

	CStateMachine::TTransition acornLost;
	acornLost.action = CStateMachine::POP_ACTION;
	acornLost.event = TARGET_LOST_EVENT;
	acornLost.newState = CStateMachine::ANY_STATE;
	acornLost.oldState = CStateMachine::PATROL_STATE;
	m_stateMachine.AddTransition(acornLost);

	CStateMachine::TTransition endAttackFailure;
	endAttackFailure.action = CStateMachine::POP_ACTION;
	endAttackFailure.event = TARGET_LOST_EVENT;
	endAttackFailure.newState = CStateMachine::ANY_STATE;
	endAttackFailure.oldState = CStateMachine::ATTACK_STATE;
	m_stateMachine.AddTransition(endAttackFailure);

	CStateMachine::TTransition endAttackSuccess;
	endAttackSuccess.action = CStateMachine::POP_ACTION;
	endAttackSuccess.event = DESTINATION_REACHED_EVENT;
	endAttackSuccess.newState = CStateMachine::ANY_STATE;
	endAttackSuccess.oldState = CStateMachine::ATTACK_STATE;
	m_stateMachine.AddTransition(endAttackSuccess);

	CStateMachine::TTransition endRest;
	endRest.action = CStateMachine::POP_ACTION;
	endRest.event = MOLE_RESTED_EVENT;
	endRest.newState = CStateMachine::ANY_STATE;
	endRest.oldState = CStateMachine::REST_STATE;
	m_stateMachine.AddTransition(endRest);

	CStateMachine::TTransition startAttackFromExplore;
	startAttackFromExplore.action = CStateMachine::PUSH_ACTION;
	startAttackFromExplore.event = DISTURBANCE_EVENT;
	startAttackFromExplore.newState = CStateMachine::ATTACK_STATE;
	startAttackFromExplore.oldState = CStateMachine::EXPLORE_STATE;
	m_stateMachine.AddTransition(startAttackFromExplore);

	CStateMachine::TTransition startAttackFromPatrol;
	startAttackFromPatrol.action = CStateMachine::PUSH_ACTION;
	startAttackFromPatrol.event = DISTURBANCE_EVENT;
	startAttackFromPatrol.newState = CStateMachine::ATTACK_STATE;
	startAttackFromPatrol.oldState = CStateMachine::PATROL_STATE;
	m_stateMachine.AddTransition(startAttackFromPatrol);

	CStateMachine::TTransition startPatrol;
	startPatrol.action = CStateMachine::PUSH_ACTION;
	startPatrol.event = ACORN_FOUND_EVENT;
	startPatrol.newState = CStateMachine::PATROL_STATE;
	startPatrol.oldState = CStateMachine::EXPLORE_STATE;
	m_stateMachine.AddTransition(startPatrol);

	CStateMachine::TTransition startRest;
	startRest.action = CStateMachine::PUSH_ACTION;
	startRest.event = MOLE_TIRED_EVENT;
	startRest.newState = CStateMachine::REST_STATE;
	startRest.oldState = CStateMachine::PATROL_STATE;
	m_stateMachine.AddTransition(startRest);

	// Debug transitions.
	CStateMachine::TTransition wakeUp;
	wakeUp.action = CStateMachine::POP_ACTION;
	wakeUp.event = SLEEP_EVENT;
	wakeUp.newState = CStateMachine::ANY_STATE;
	wakeUp.oldState = CStateMachine::SLEEP_STATE;
	m_stateMachine.AddTransition(wakeUp);

	CStateMachine::TTransition sleep;
	sleep.action = CStateMachine::PUSH_ACTION;
	sleep.event = SLEEP_EVENT;
	sleep.newState = CStateMachine::SLEEP_STATE;
	sleep.oldState = CStateMachine::ANY_STATE;
	m_stateMachine.AddTransition(sleep);
}

void CMoleAgent::Attack(FLOAT32 _fTimeDelta)
{
	if (m_fDisturbanceTimeDelta > fDISTURBANCE_LIFETIME)
	{
		Messages::send(TARGET_LOST_EVENT, &m_rMole);
	}
	else
	{
		m_pathFindingAgent.Update(_fTimeDelta);
		m_steeringAgent.SetDestination(m_pathFindingAgent.GetDestination());
	}
}

void CMoleAgent::Draw() const
{
	if (CMMMContext::GetInstance().IsDrawingMoleState())
	{
		CCircle disturbanceRangeCircle;
		disturbanceRangeCircle.Initialise(m_rMole.GetPosition(), m_fDisturbanceRange);
		CMMMContext::GetInstance().DrawCircle(disturbanceRangeCircle, 0xFFFF0000);

		CCircle sightRangeCircle;
		sightRangeCircle.Initialise(m_rMole.GetPosition(), m_fSightRange);
		CMMMContext::GetInstance().DrawCircle(sightRangeCircle, 0xff00a2e8); // light blue

		stringstream status;

		if (!m_stateMachine.GetLastEvent().empty())
		{
			status << m_stateMachine.GetLastEvent() << endl;
		}

		if (m_stateMachine.GetState() == CStateMachine::ATTACK_STATE)
		{
			status << "Attacking";
		}
		else if (m_stateMachine.GetState() == CStateMachine::EXPLORE_STATE)
		{
			status << "Exploring";
		}
		else if (m_stateMachine.GetState() == CStateMachine::PATROL_STATE)
		{
			status << "Patrolling";
		}
		else if (m_stateMachine.GetState() == CStateMachine::REST_STATE)
		{
			status << "Resting";
		}
		else if (m_stateMachine.GetState() == CStateMachine::SLEEP_STATE)
		{
			status << "Sleeping";
		}

		CMMMContext::GetInstance().GetFont()->Render(m_rMole.GetPosition().X(), m_rMole.GetPosition().Y() + fMOLE_RADIUS,
			GDE::FJ_Centre, status.str().c_str());
	}

	if (CMMMContext::GetInstance().IsDrawingPathFinding())
	{
		m_pathFindingAgent.Draw();
	}

	if (CMMMContext::GetInstance().IsDrawingSteering())
	{
		m_steeringAgent.Draw();

	}
}

void CMoleAgent::Explore()
{
	Explore(true, SFVec2(100.0f, 100.0f), SFVec2(700.0f, 500.0f));
}

void CMoleAgent::Explore(bool bUnexploredAreaOnly, const CFVec2& _krvTopLeft, const CFVec2& _krvBottomRight)
{
	CFVec2 vHeading;
	vHeading.SetToRotationZ(m_rMole.GetHeading());
	CFVec2 vToDestination = m_rMole.GetDestination() - m_rMole.GetPosition();

	if (vToDestination.Magnitude() < m_fSightRange ||
		vHeading.DotProduct(vToDestination) < 0.0f)
	{
		m_rMole.SetDestination(GetExplorationDestination(bUnexploredAreaOnly, _krvTopLeft, _krvBottomRight));
		m_steeringAgent.SetDestination(m_rMole.GetDestination());
	}
}

CFVec2 CMoleAgent::GetExplorationDestination(bool bUnexploredAreaOnly, const CFVec2& _krvTopLeft,
	const CFVec2& _krvBottomRight) const
{
	CFVec2 vExplorationDestination;
	FLOAT32 fDistanceFromExploredArea = 0.0f;
	const CGraph& navigationMesh = CBlackboard::GetInstance().GetNavigationMesh();
	UINT32 uiAttempts = 0;

	do
	{
		vExplorationDestination = SFVec2(
			Math::GetRandomFloat(_krvTopLeft.X(), _krvBottomRight.X()),
			Math::GetRandomFloat(_krvTopLeft.Y(), _krvBottomRight.Y()));

		if (!navigationMesh.GetAll().empty())
		{
			fDistanceFromExploredArea =
				(vExplorationDestination - navigationMesh.GetClosest(vExplorationDestination)->GetPosition()).Magnitude();
		}

		uiAttempts++;
	}
	while (bUnexploredAreaOnly &&
		uiAttempts < fMAX_EXPLORE_DESTINATION_ATTEMPTS && 
		fDistanceFromExploredArea < m_fSightRange);

	return vExplorationDestination;
}

FLOAT32 CMoleAgent::GetRotationalSpeed() const
{
	return m_steeringAgent.GetRotationalSpeed();
}

FLOAT32 CMoleAgent::GetSpeed() const
{
	return m_steeringAgent.GetSpeed();
}

void CMoleAgent::Patrol()
{
	Explore(false, m_pAcorn->GetPosition() - SFVec2(100.0f, 100.0f),
		m_pAcorn->GetPosition() + SFVec2(100.0f, 100.0f));

	m_fPatrolTimeDelta += m_fAiTimeDelta;
	if (m_fPatrolTimeDelta > 1.0f / fREST_FREQUENCY)
	{
		m_fPatrolTimeDelta = 0.0f;
		Messages::send(MOLE_TIRED_EVENT, &m_rMole);
	}
}

void CMoleAgent::Receive(const string& _sSubject, const void* _pMessage)
{
	if (_sSubject == ACORN_FOUND_EVENT)
	{
		const CMole* pMole = static_cast<const CMole*>(_pMessage);

		if (pMole == &m_rMole)
		{
			if (m_stateMachine.GetState() != CStateMachine::PATROL_STATE)
			{
				m_pAcorn = CBlackboard::GetInstance().GetAcorns().back();
				m_rMole.SetDestination(m_pAcorn->GetPosition());
			}

			m_pathFindingAgent.Reset();
			m_stateMachine.Update(_sSubject);
		}
	}
	else if (_sSubject == DISTURBANCE_EVENT)
	{
		const CCircle* disturbance = static_cast<const CCircle*>(_pMessage);
		
		if (disturbance->Radius() > 0.0f &&
			(disturbance->Centre() - m_rMole.GetPosition()).Magnitude() <
			m_fDisturbanceRange + disturbance->Radius())
		{
			m_fDisturbanceTimeDelta = 0.0f;
			m_pathFindingAgent.Reset();
			m_rMole.SetDestination(disturbance->Centre());
			m_stateMachine.Update(_sSubject);
		}
	}
	else if (_sSubject == SLEEP_EVENT)
	{
		m_pathFindingAgent.Reset();
		m_stateMachine.Update(_sSubject);
	}
	else if (_pMessage == m_pAcorn)
	{
		if (m_stateMachine.GetState() == CStateMachine::PATROL_STATE)
		{
			CBlackboard::GetInstance().RemoveAcorn(*m_pAcorn);
		}

		m_pathFindingAgent.Reset();
		m_stateMachine.Update(_sSubject);
	}
	else if (_pMessage == &m_rMole)
	{
		m_pathFindingAgent.Reset();
		m_stateMachine.Update(_sSubject);
	}
}

void CMoleAgent::Rest(FLOAT32 _fTimeDelta)
{
	m_fRestTimeDelta += _fTimeDelta;

	if (m_fRestTimeDelta > fREST_TIME)
	{
		m_fRestTimeDelta = 0.0f;
		Messages::send(MOLE_RESTED_EVENT, &m_rMole);
	}
}

void CMoleAgent::Update(FLOAT32 _fTimeDelta)
{
	m_fAiTimeDelta += _fTimeDelta;
	m_fDisturbanceTimeDelta += _fTimeDelta;
	m_fSteeringTimeDelta += _fTimeDelta;

	if (m_fAiTimeDelta > 1.0f / fAI_FREQUENCY)
	{
		if (m_stateMachine.GetState() == CStateMachine::ATTACK_STATE)
		{
			Attack(m_fAiTimeDelta);
		}
		else if (m_stateMachine.GetState() == CStateMachine::EXPLORE_STATE)
		{
			Explore();
		}
		else if (m_stateMachine.GetState() == CStateMachine::PATROL_STATE)
		{
			Patrol();
		}
		else if (m_stateMachine.GetState() == CStateMachine::REST_STATE)
		{
			Rest(m_fAiTimeDelta);
		}

		m_sensorAgent.Update();
		m_fAiTimeDelta = 0.0f;
	}

	if (m_fSteeringTimeDelta > 1.0f / fSTEERING_FREQUENCY)
	{
		if (m_stateMachine.GetState() == CStateMachine::REST_STATE ||
			m_stateMachine.GetState() == CStateMachine::SLEEP_STATE)
		{
			m_steeringAgent.Rest(m_fSteeringTimeDelta);
		}
		else
		{
			m_steeringAgent.Run(m_fSteeringTimeDelta);
		}

		m_fSteeringTimeDelta = 0.0f;
	}
}
