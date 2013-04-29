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
#ifndef BLACKBOARD_H_
#define BLACKBOARD_H_

#include "Common.h"

#include "Cat.h"
#include "Circle.h"
#include "Wall.h"
#include "GDE/Core/Observer.h"

// Must be included after Common.h or errors will ensue...
#include <vector>

class CCircle;

/**
 * A container for information to be used by agents.
 */
class CBlackboard : public GDE::IObserver
{
	public:
		void AddCat(const CCat& _krCat);

		void AddRepulsionZone(const CCircle& _krRepulsionZone);

		void AddWall(const CWall& _krWall);

		const std::vector<const CCat*>& GetCats() const;

		/**
		 * Retrieves the closest intersection time on the given whisker when intersected with all of the walls.
		 * 
		 * @param _krvWhiskerStart The start point of the whisker.
		 * @param _krvWhisker The whisker.
		 * 
		 * @return The closest intersection time, or -1.0f if no intersections were found.
		 */
		FLOAT32 GetClosestWallIntersectionTime(const CFVec2& _krvWhiskerStart, const CFVec2& _krvWhisker) const;

		/**
		 * Retrieves the singleton instance.
		 * 
		 * @return The singleton instance.
		 */
		static CBlackboard& GetInstance();

		const CFVec2& GetMousePosition() const;

		const std::vector<CCircle>& GetRepulsionZones() const;

		/**
		 * Notifies this observer of any changes to subject it is registered for.
		 * 
		 * @param _pSubject The subject for which the change occured.
		 * @param _uMessage The change.
		 */
		void NotifyChange(GDE::ISubject* _pSubject, GDE::UINT32 _uiMessage);

	private:
		std::vector<const CCat*> m_cats;

		CFVec2 m_vMousePosition;

		std::vector<CCircle> m_repulsionZones;

		std::vector<CWall> m_walls;

		CBlackboard();
};

#endif /* BLACKBOARD_H_ */
