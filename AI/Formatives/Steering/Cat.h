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
#ifndef CAT_H_
#define CAT_H_

#include "SteeringAgent.h"

class CCircle;

/**
 *	This class represents a cat, which will attempt to chase the mouse.
 */
class CCat
{
	public:
		CCat();

		~CCat();

		void Draw();

		void GetBoundingCircle(CCircle& _rBounds);

		/**
		 * Retrieves the heading in radians (where the positive direction on the y axis is considered 0).
		 *
		 * @return The heading in radians.
		 */
		float GetHeadingRadians() const;

		/**
		 * Retrieves the heading as a vector.
		 *
		 * @return The heading as a vector.
		 */
		const CFVec2& GetHeadingVector() const;

		const CFVec2& GetPosition() const;

		float GetRadius() const;

		/**
		 * Retrieves the steering agent responsible for steering this cat.
		 *
		 * @return The steering agent responsible for steering this cat.
		 */
		const CSteeringAgent* GetSteeringAgent() const;

		/**
		 * Initialises this cat.
		 *
		 * @param _rvPosition The position.
		 * @param _fHeadingDegrees The heading in degrees (where the positive direction on the y axis is considered 0).
		 */
		void Initialise(CFVec2Arg _rvPosition, FLOAT32 _fHeadingDegrees);

		/**
		 * Determines if this cat intersects the given circle.
		 *
		 * @param _krCircle The circle to test for intersection with.
		 *
		 * @return True if this cat intersects the given circle, false otherwise.
		 */
		bool Intersects(const CCircle& _krCircle) const;

		/**
		 * Determines if this cat is intersected by another cat.
		 *
		 * @return True if this cat is intersected by another cat, false otherwise.
		 */
		bool IsIntersectedByCat() const;

		/**
		 * Reset intersection data so that this cat is not currently intersected by another cat.
		 */
		void ResetIntersection();

		/**
		 * Set this cat to be intersected by another cat.
		 */
		void SetIntersectedByCat();

		/**
		 * Updates the position and heading.
		 *
		 * @param _fTimeDelta The time that has elapsed since the last update.
		 */
		void Update(FLOAT32 _fTimeDelta);

	protected:
		/**
		 * Position of cat.
		 */
		CFVec2 m_vPosition;

		/**
		 * Fixed to always be 22 pixels in final version please.
		 */
		FLOAT32 m_fRadius;

		/**
		 * Angle the cat is facing (in radians).
		 */
		FLOAT32 m_fHeadingRadians;

		/**
		 * Direction the cat is facing.
		 */
		CFVec2 m_vHeadingVector;

		/**
		 * Is this intersecting anything else?
		 */
		bool m_bIntersected;

		CSteeringAgent* m_pSteeringAgent;
};

#endif /* CAT_H_ */
