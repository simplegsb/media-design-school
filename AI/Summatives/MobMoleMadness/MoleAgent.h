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
#ifndef MOLEAGENT_H_
#define MOLEAGENT_H_

#include "Math.h"
#include "Messages.h"
#include "PathFindingAgent.h"
#include "SensorAgent.h"
#include "StateMachine.h"
#include "SteeringAgent.h"

class CMoleAgent : public Messages::Recipient
{
	public:
		CMoleAgent(CMole& _rMole, FLOAT32 fDisturbanceRange, FLOAT32 fSightRange);

		void Draw() const;

		FLOAT32 GetRotationalSpeed() const;

		FLOAT32 GetSpeed() const;

		void Receive(const std::string& subject, const void* message);

		void Update(FLOAT32 _fTimeDelta);

	private:
		/**
		 * The time since the last AI decision was made.
		 */
		FLOAT32 m_fAiTimeDelta;

		/**
		 * The range at which disturbances can be detected.
		 */
		FLOAT32 m_fDisturbanceRange;

		/**
		 * The time since the last disturbance was detected.
		 */
		FLOAT32 m_fDisturbanceTimeDelta;

		/**
		 * The time that the agent has been patrolling for (if it is patrolling).
		 */
		FLOAT32 m_fPatrolTimeDelta;

		/**
		 * The time that the agent has been resting for (if it is resting).
		 */
		FLOAT32 m_fRestTimeDelta;

		/**
		 * The range at which objects can be seen.
		 */
		FLOAT32 m_fSightRange;

		/**
		 * The time since the last steering decision was made.
		 */
		FLOAT32 m_fSteeringTimeDelta;

		/**
		 * The acorn this agent is patrolling (if it is patrolling one).
		 */
		const CAcorn* m_pAcorn;

		CPathFindingAgent m_pathFindingAgent;

		CMole& m_rMole;

		CSensorAgent m_sensorAgent;

		CStateMachine m_stateMachine;

		CSteeringAgent m_steeringAgent;

		void Attack(FLOAT32 _fTimeDelta);

		void Explore();

		void Explore(bool bUnexploredAreaOnly, const CFVec2& _krvTopLeft, const CFVec2& _krvBottomRight);

		CFVec2 GetExplorationDestination(bool bUnexploredAreaOnly, const CFVec2& _krvTopLeft,
			const CFVec2& _krvBottomRight) const;

		void Patrol();

		void Rest(FLOAT32 _fTimeDelta);
};

#endif /* MOLEAGENT_H_ */
