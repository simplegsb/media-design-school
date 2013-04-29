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
// $Date: 2012-09-24 08:28:44 +1200 (Mon, 24 Sep 2012) $
// $Rev: 68 $
//
#ifndef STEERINGAGENT_H_
#define STEERINGAGENT_H_

#include "Common.h"

#include "FVector2.h"

class CCat;
const unsigned int SIDE_WHISKER_COUNT = 2;

/**
 * An agent capable of steering and avoiding objects.
 */
class CSteeringAgent
{
	public:
		CSteeringAgent(const CCat& _krCat);

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

		float GetApproachRadius() const;

		const CFVec2& GetBearing() const;

		const CCat& GetCat() const;

		const CFVec2& GetWhiskerFront() const;

		const CFVec2* GetWhiskersLeft() const;

		const CFVec2* GetWhiskersRight() const;

		FLOAT32 GetRotationalSpeed() const;

		FLOAT32 GetSpeed() const;

		bool IsStuck() const;

		/**
		 * Updates the bearing and speeds.
		 *
		 * @param _fTimeDelta The time that has elapsed since the last update.
		 */
		void Update(FLOAT32 _fTimeDelta);

	private:
		/**
         * The stuck state.
		 */
		bool m_bStuck;

		/**
         * The cat being steered.
		 */
		const CCat& m_krCat;

		/**
         * The acceleration.
		 */
		FLOAT32	m_fAcceleration;

		/**
         * The radius around the destination at which the agent will start 'approaching'.
		 */
		FLOAT32	m_fApproachRadius;

		/**
         * The deceleration.
		 */
		FLOAT32	m_fDeceleration;

		/**
         * The repulsion buffer to surround dynamic objects with.
		 */
		FLOAT32	m_fRepulsionBufferDynamic;

		/**
         * The repulsion buffer to surround static objects with.
		 */
		FLOAT32	m_fRepulsionBufferStatic;

		/**
         * The rotational acceleration.
		 */
		FLOAT32	m_fRotationalAcceleration;

		/**
         * The rotational deceleration.
		 */
		FLOAT32	m_fRotationalDeceleration;

		/**
         * The rotational speed.
		 */
		FLOAT32	m_fRotationalSpeed;

		/**
         * The speed.
		 */
		FLOAT32	m_fSpeed;

		/**
         * The maximum speed.
		 */
		FLOAT32	m_fSpeedMax;

		/**
         * The maximum rotational speed.
		 */
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

		/**
         * The bearing.
		 */
		CFVec2 m_vBearing;

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

		CFVec2 GetRepulsionBearingEffect() const;

		float GetRepulsionSpeedEffect() const;

		CFVec2 GetWhiskerBearingEffect() const;

		float GetWhiskerSpeedEffect() const;

		CSteeringAgent& operator=(const CSteeringAgent& _krOriginal);
};

#endif /* STEERINGAGENT_H_ */
