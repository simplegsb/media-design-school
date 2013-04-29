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
#ifndef STEERINGAGENT_H_
#define STEERINGAGENT_H_

#include "Common.h"

#include "FVector2.h"

class CMole;
const unsigned int SIDE_WHISKER_COUNT = 2;

/**
 * An agent capable of steering and avoiding objects.
 */
class CSteeringAgent
{
	public:
		CSteeringAgent(const CMole& _krMole);

		void Draw() const;

		FLOAT32 GetRotationalSpeed() const;

		FLOAT32 GetSpeed() const;

		const CFVec2& GetDestination() const;

		bool IsStuck() const;

		void Rest(FLOAT32 _fTimeDelta);

		/**
		 * Updates the bearing and speeds.
		 *
		 * @param _fTimeDelta The time that has elapsed since the last update.
		 */
		void Run(FLOAT32 _fTimeDelta);

		void SetDestination(const CFVec2& _krvDestination);

	private:
		/**
         * The stuck state.
		 */
		bool m_bStuck;

		const CMole& m_krMole;

		FLOAT32	m_fAcceleration;

		FLOAT32	m_fDeceleration;

		/**
         * The repulsion buffer to surround dynamic objects with.
		 */
		FLOAT32	m_fRepulsionBufferDynamic;

		/**
         * The repulsion buffer to surround static objects with.
		 */
		FLOAT32	m_fRepulsionBufferStatic;

		FLOAT32	m_fRotationalAcceleration;

		FLOAT32	m_fRotationalDeceleration;

		FLOAT32	m_fRotationalSpeed;

		FLOAT32	m_fSpeed;

		FLOAT32	m_fSpeedMax;

		FLOAT32	m_fSteeringLock;

		/**
         * The time since the last check for the stuck state.
		 */
		FLOAT32	m_fStuckCheckDelta;

		/**
         * The angle to turn when stuck.
		 */
		FLOAT32	m_fStuckRotation;

		/**
         * The time to wait before entering the stuck state.
		 */
		FLOAT32	m_fStuckTimeThreshold;

		CFVec2 m_vBearing;

		CFVec2 m_vDestination;

		/**
         * The position at which the last check for the stuck state was performed.
		 */
		CFVec2 m_vStuckCheckPosition;
		
		/**
         * The front pointing whisker.
		 */
		CFVec2 m_vWhiskerFront;
		
		/**
         * The left pointing whiskers.
		 */
		CFVec2 m_vWhiskersLeft[SIDE_WHISKER_COUNT];
		
		/**
         * The right pointing whiskers.
		 */
		CFVec2 m_vWhiskersRight[SIDE_WHISKER_COUNT];

		CSteeringAgent(const CSteeringAgent& _krOriginal);

		void ComputeBearing();

		void ComputeSpeeds(FLOAT32 _fTimeDelta);

		FLOAT32 ComputeTargetRotationalSpeed();

		FLOAT32 ComputeTargetSpeed();

		void DrawSteering() const;

		void DrawTargeting() const;

		void DrawWhisker(const CFVec2& _rvWhisker) const;

		void DrawWhiskers() const;

		/**
		 * Retrieves a vector to the start of the given whisker in absolute coordinates.
		 *
		 * @param _krvRelativeWhisker The relative vector that describes the whisker (relative to the cat it belongs
		 * to).
		 *
		 * @return A vector to the start of the given whisker in absolute coordinates.
		 */
		CFVec2 GetAbsoluteWhiskerStart(const CFVec2& _krvRelativeWhisker) const;

		/**
		 * Retrieves a copy of the given whisker adjusted to face in the correct direction (accounts for the heading of
		 * the cat).
		 *
		 * @param _krvWhisker The relative vector that describes the whisker (relative to the cat it belongs to).
		 *
		 * @return A vector to the start of the given whisker in absolute coordinates.
		 */
		CFVec2 GetAdjustedWhisker(const CFVec2& _krvWhisker) const;

		CFVec2 GetRepulsionBearingEffect() const;

		float GetRepulsionSpeedEffect() const;

		CFVec2 GetWhiskerBearingEffect() const;

		float GetWhiskerSpeedEffect() const;

		CSteeringAgent& operator=(const CSteeringAgent& _krOriginal);
};

#endif /* STEERINGAGENT_H_ */
